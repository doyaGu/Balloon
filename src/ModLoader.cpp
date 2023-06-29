#include "ModLoader.h"

#include <cassert>
#include <memory>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "Logger.h"
#include "FileSystem.h"
#include "PathUtils.h"
#include "ModExplorer.h"
#include "ModResolver.h"
#include "ModCandidateFinder.h"

using namespace balloon;

ModLoader::ModLoader(const std::shared_ptr<ModRegistry> &registry) : m_Registry(registry) {}

ModLoader::~ModLoader() = default;

bool ModLoader::Load(const std::vector<std::string> &paths) {
    if (paths.empty())
        return false;

    if (IsFrozen())
        return false;

    // Gather mods
    auto explorer = std::make_unique<ModExplorer>();
    for (auto &path: paths) {
        if (!path.empty())
            explorer->AddCandidateFinder(std::make_shared<DirectoryModCandidateFinder>(path));
    }

    ModCandidateSetMap envDisabledMods;
    auto modCandidates = explorer->ExploreMods(this, envDisabledMods);

    // Resolve mods
    ModCandidateArray mods = ModResolver::GetInstance().Resolve(modCandidates, envDisabledMods);

    DumpModList(mods);

    for (auto &mod: mods) {
        LoadMod(mod);
    }
    return true;
}

bool ModLoader::Unload(const std::string &id) {
    if (IsFrozen())
        return false;

    auto it = m_ModToDllHandleMap.find(id);
    if (it == m_ModToDllHandleMap.end())
        return false;

    if (!ForgetLib(it->second.get())) {
        LOG_ERROR("Can not release the library of mod [%s]", id.c_str());
        return false;
    }

    return true;
}

void *ModLoader::SetUserData(void *data, size_t type) {
    return m_UserData.SetData(data, type);
}

void *ModLoader::GetUserData(size_t type) const {
    return m_UserData.GetData(type);
}

bool ModLoader::LoadMod(const ModCandidate &mod) {
    auto sourcePath = mod.GetPath();
    if (sourcePath.empty())
        return false;

    auto &fs = FileSystem::GetInstance();
    bool isArchive = !fs.IsDirectory(sourcePath.c_str());

    std::string rootPath = isArchive ? utils::RemoveExtension(sourcePath, "") + "/" : sourcePath;

    // First, check for the existence of mod library
    // the filename of mod dll should end with ".dll"
    std::string modBin = rootPath + "/bin/";
    std::string modDll = modBin + mod.GetId();
    modDll += ".dll";
    if (!fs.Exists(modDll.c_str())) {
        LOG_WARN("Mod library %s is not found.", modDll.c_str());
        return false;
    }

    // For mod archive, because loader can not load dll file from memory directly,
    // it will first extract all files in bin folders to a cache directory.
    if (isArchive) {
        fs.SetWriteDir(fs.GetDir(FS_DIR_LOADER));

        if (!fs.Exists("/cache"))
            fs.MakeDir("/cache");

        std::string modCache = "/cache/";
        modCache += mod.GetId();
        modCache += "/";
        if (!fs.Exists(modCache.c_str()))
            fs.MakeDir(modCache.c_str());
   
        std::string modCacheBin = modCache + "bin/";
        if (!fs.Exists(modCacheBin.c_str()))
            fs.MakeDir(modCacheBin.c_str());

        bool success = true;
        char **rc = fs.EnumerateFiles(modBin.c_str());
        char **i;
        for (i = rc; *i != nullptr; i++) {
            std::string src(modBin);
            src += *i;

            std::string dest(modCacheBin);
            dest += *i;

            if (!fs.Copy(src.c_str(), dest.c_str())) {
                LOG_ERROR("Failed to copy %s to cache directory.", src.c_str());
                success = false;
                break;
            }
        }
        fs.FreeList(rc);
        fs.SetWriteDir(nullptr);

        if (!success)
            return false;

        modDll = modCacheBin + mod.GetId();
        modDll += ".dll";
    }

    // Calculate the mod dll path in platform-dependent notation
    std::string realModDll = utils::JoinPaths(fs.GetDir(FS_DIR_LOADER), modDll);
    utils::NormalizePath(realModDll);

    auto dllHandle = LoadLib(realModDll);
    if (!dllHandle) {
        LOG_ERROR("Failed to load library for mod %s.", mod.GetId());
        return false;
    }

    const auto *info = LoadModInfo(dllHandle, realModDll);
    if (!info) {
        LOG_ERROR("Failed to load mod info for mod %s.", mod.GetId());
        return false;
    }

    if (strcmp(info->id, mod.GetId()) != 0) {
        LOG_ERROR("The id [%s] in the mod info is inconsistent with the id [%s] in the metadata", info->id, mod.GetId());
        return false;
    }

    if (strcmp(info->version, mod.GetVersion()) != 0) {
        LOG_ERROR("The version [%s] in the mod info is inconsistent with the version [%s] in the metadata", info->version, mod.GetVersion());
        return false;
    }

    if (!m_Registry->AddMod(rootPath, sourcePath, mod.GetMetadata(), dllHandle, info)) {
        LOG_ERROR("Failed to add mod %s to the registry.", mod.GetId());
        return false;
    }

    m_Registry->AddCallback(MR_CB_POSTREMOVE, [sourcePath](const std::shared_ptr<ModContainer> &mod) {
        auto &fs = FileSystem::GetInstance();
        if (!fs.IsDirectory(sourcePath.c_str())) {
            // Calculate the mod path in platform-dependent notation
            std::string oldDir = utils::JoinPaths(fs.GetDir(FS_DIR_LOADER), sourcePath);
            utils::NormalizePath(oldDir);
            fs.Unmount(oldDir.c_str());
        }
        return 1;
    });

    m_ModToDllHandleMap[info->id] = dllHandle;
    m_DllHandleToModMap[dllHandle.get()] = info->id;
    return true;
}

std::shared_ptr<void> ModLoader::LoadLib(const std::string &path) {
    if (path.empty())
        return nullptr;

    std::shared_ptr<void> dllHandlePtr;

    HMODULE dllHandle = ::LoadLibraryEx(path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!dllHandle)
        return nullptr;

    bool inserted;
    DllHandleMap::iterator it;
    std::tie(it, inserted) = m_DllHandleMap.insert({dllHandle, std::weak_ptr<void>()});

    if (!inserted) {
        dllHandlePtr = it->second.lock();
        if (dllHandlePtr) {
            ::FreeLibrary(dllHandle);
        }
    }

    if (!dllHandlePtr) {
        dllHandlePtr = std::shared_ptr<void>(dllHandle, [](void *ptr) {
            ::FreeLibrary(reinterpret_cast<HMODULE>(ptr));
        });
        it->second = dllHandlePtr;
    }

    return dllHandlePtr;
}

bool ModLoader::ForgetLib(void *dllHandle) {
    auto it = m_DllHandleToModMap.find(dllHandle);
    if (it == m_DllHandleToModMap.end())
        return false;

    m_ModToDllHandleMap.erase(it->second);
    m_DllHandleToModMap.erase(it);
    return true;
}

void ModLoader::DumpModList(const ModCandidateArray &mods) {
    const size_t count = mods.size();
    if (count == 1) {
        LOG_INFO("Loading 1 mod: %s %s", mods[0].GetId(), mods[0].GetVersion());
    } else {
        LOG_INFO("Loading %d mods: ", count);
        for (auto &mod: mods) {
            LOG_INFO("\t%s %s", mod.GetId(), mod.GetVersion());
        }
    }
}

const ModInfo *ModLoader::LoadModInfo(const std::shared_ptr<void> &dllHandle, const std::string &path) {
    assert(dllHandle != nullptr);

    constexpr const char *ENTRY_SYMBOL = "GetModInfo";
    typedef void (*ModLoadFunction)(const void *, const void **const, int *, size_t *, size_t *);

    auto func = reinterpret_cast<ModLoadFunction>(::GetProcAddress(reinterpret_cast<HMODULE>(dllHandle.get()), ENTRY_SYMBOL));
    if (!func) {
        LOG_ERROR("Library %s does not export the required symbol: %s.", path.c_str(), ENTRY_SYMBOL);
        return nullptr;
    }

    int version = INFO_API_VERSION;
    size_t size = sizeof(ModInfo);
    size_t alignment = alignof(ModInfo);
    const ModInfo *info = nullptr;

    func(nullptr, reinterpret_cast<const void **>(&info), &version, &size, &alignment);

    if (version != INFO_API_VERSION) {
        LOG_ERROR("Library %s is using an incompatible version %d of the API. The supported version is %d.", path.c_str(), version, INFO_API_VERSION);
        return nullptr;
    }

    if (size != sizeof(ModInfo) || alignment != alignof(ModInfo)) {
        LOG_ERROR("The ModInfo size or alignment are inconsistent with the expected values for the library %s.", path.c_str());
        LOG_ERROR(" -- size: expected %d | received %d\n -- alignment: expected %d | received %d.", sizeof(ModInfo), size, alignof(ModInfo), alignment);
        return nullptr;
    }

    if (!info) {
        LOG_ERROR("Library %s failed to provide ModInfo.", path.c_str());
        return nullptr;
    }

    return info;
}