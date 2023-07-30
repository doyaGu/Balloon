#include "Balloon.h"

#include "Hooks.h"

#include "Logger.h"
#include "Config.h"
#include "FileSystem.h"
#include "DataShare.h"
#include "EventManager.h"
#include "Hook.h"
#include "DataStack.h"
#include "WeakRefFlag.h"
#include "StringUtils.h"
#include "PathUtils.h"

#define BALLOON_HOOK_CODE 0x00424D4C
#define BALLOON_HOOK_VERSION 0x00000001

#define BALLOON_CONFIG_FILE "/configs/Balloon.json"

using namespace balloon;

Balloon &Balloon::GetInstance() {
    static Balloon instance;
    return instance;
}

Balloon::~Balloon() {
    Shutdown();
}

bool Balloon::Init() {
    if (IsInited())
        return true;

    if (!InitFileSystem())
        return false;

    InitLogger();

    if (!Hook::GetInstance().Init()) {
        LOG_ERROR("Failed to initialize hook api.");
        return false;
    }

    LOG_INFO("Initializing Mod Loader...");

    m_Config = m_Context->GetConfig(nullptr);
    if (!m_Config) {
        LOG_ERROR("Failed to create config for mod loader.");
        return false;
    }

    LoadConfig(m_Config, BALLOON_CONFIG_FILE);

    m_Registry = std::make_shared<ModRegistry>();
    m_Loader = std::make_shared<ModLoader>(m_Registry);
    m_Context = std::make_shared<ModContext>(m_Registry);

    RegisterBuiltinInterfaces();
    RegisterBuiltinFactories();

    m_Flag |= BALLOON_INITED;
    return true;
}

void Balloon::Shutdown() {
    if (IsInited()) {
        DisconnectMods();
        ShutdownMods();
        UnloadMods();

        m_Context = nullptr;
        m_Loader = nullptr;
        m_Registry = nullptr;

        auto &fs = FileSystem::GetInstance();
        fs.SetWriteDir(fs.GetDir(FS_DIR_LOADER));
        SaveConfig(m_Config, BALLOON_CONFIG_FILE);
        fs.SetWriteDir(nullptr);
        m_Config->Release();
        m_Config = nullptr;

        if (!Hook::GetInstance().Shutdown()) {
            LOG_ERROR("Failed to shutdown hook api.");
        }

        ShutdownLogger();
        ShutdownFileSystem();
    }
    m_Flag &= ~BALLOON_INITED;
}

bool Balloon::LoadMods() {
    if (AreModsLoaded())
        return true;

    auto &fs = FileSystem::GetInstance();

    std::vector<std::string> paths;

    // Mods Path 1: Game Directory
    if (fs.Exists("/game/Mods")) {
        paths.emplace_back("/game/Mods");
    }

    // Mods Path 2: Loader Directory
    if (fs.Exists("/mods")) {
        paths.emplace_back("/mods");
    }

    // Mods Path 3: AppData Directory
    if (fs.Exists("/user/mods")) {
        paths.emplace_back("/user/mods");
    }

    if (!m_Loader->Load(paths)) {
        return false;
    }

    std::weak_ptr<ModLoader> loader = m_Loader;
    m_Registry->AddCallback(MR_CB_POSTREMOVE, [loader](const std::shared_ptr<ModContainer> &mod) {
        auto l = loader.lock();
        if (l) {
            l->Unload(mod->GetId());
        }
        return 1;
    });

    m_Flag |= BALLOON_MODS_LOADED;
    return true;
}

void Balloon::UnloadMods() {
    if (!AreModsLoaded())
        return;

    m_Loader->Revive();

    auto ids = m_Registry->GetAllModIds();
    for (auto rit = ids.rbegin(); rit != ids.rend(); ++rit) {
        m_Registry->RemoveMod(*rit);
    }

    m_Flag &= ~BALLOON_MODS_LOADED;
}

bool Balloon::InitMods() {
    if (AreModsInited())
        return true;

    bool success = m_Registry->IterateMods([this](const std::shared_ptr<ModContainer> &mod) -> int {
        uint32_t flags = 0;

        if (!mod->Instantiate()) {
            LOG_FATAL("Mod [%s] can not be instantiated.", mod->GetId());
            return -1;
        }

        auto logger = Logger::Get(mod->GetId());
        if (logger)
            CreateLogFile(logger);

        LoadModConfig(mod);

        m_Context->SetCurrentMod(mod);
        IMod *ptr = mod->GetInstance();
        if (!ptr->Init(m_Context.get(), &flags)) {
            m_Context->SetCurrentMod(nullptr);

            if ((mod->GetFlags() & MOD_FIXED) != 0) {
                LOG_FATAL("Mod [%s] encounters an error during initialization.", mod->GetId());
                return -1;
            } else {
                return 0;
            }
        }
        m_Context->SetCurrentMod(nullptr);

        mod->SetFlags(flags & 0xFFFF, 0);
        mod->SetFlags(MOD_INITIALIZED, 0);

        return 1;
    });

    if (!success)
        return false;

    m_Flag |= BALLOON_MODS_INITED;
    return true;
}

void Balloon::ShutdownMods() {
    if (!AreModsInited())
        return;

    m_Registry->IterateMods([this](const std::shared_ptr<ModContainer> &mod) -> int {
        m_Context->SetCurrentMod(mod);

        SaveModConfig(mod);

        IMod *ptr = mod->GetInstance();
        ptr->Shutdown();
        m_Context->SetCurrentMod(nullptr);

        mod->DestroyInstance();
        mod->SetFlags(0, MOD_FIXED | MOD_INITIALIZED);

        return 1;
    }, true);

    m_Flag &= ~BALLOON_MODS_INITED;
}

bool Balloon::ConnectMods() {
    if (AreModsConnected())
        return true;

    bool success = m_Registry->IterateMods([this](const std::shared_ptr<ModContainer> &mod) -> int {
        IMod *ptr = mod->GetInstance();
        if (!ptr)
            return 0;

        m_Context->SetCurrentMod(mod);
        if (!ptr->Connect()) {
            ptr->Shutdown();
            m_Context->SetCurrentMod(nullptr);

            if ((mod->GetFlags() & MOD_FIXED) != 0) {
                LOG_FATAL("Mod [%s] is fail to pass connection process.", mod->GetId());
                LOG_FATAL("A mod that provides interfaces must be available all the time.");
                return -1;
            } else {
                return 0;
            }
        }
        m_Context->SetCurrentMod(nullptr);

        mod->SetFlags(MOD_CONNECTED, 0);

        if ((mod->GetFlags() & MOD_HAS_ONUPDATE) != 0)
            m_ModsOnUpdate.push_back(ptr);

        if ((mod->GetFlags() & MOD_HAS_ONLATEUPDATE) != 0)
            m_ModsOnLateUpdate.push_back(ptr);

        return 1;
    });

    if (!success)
        return false;

    m_Loader->Freeze();

    m_Flag |= BALLOON_MODS_CONNECTED;
    return true;
}

void Balloon::DisconnectMods() {
    if (!AreModsConnected())
        return;

    m_Registry->IterateMods([this](const std::shared_ptr<ModContainer> &mod) -> int {
        IMod *ptr = mod->GetInstance();

        if ((mod->GetFlags() & MOD_HAS_ONUPDATE) != 0)
            m_ModsOnUpdate.erase(std::remove(m_ModsOnUpdate.begin(), m_ModsOnUpdate.end(), ptr), m_ModsOnUpdate.end());

        if ((mod->GetFlags() & MOD_HAS_ONLATEUPDATE) != 0)
            m_ModsOnLateUpdate.erase(std::remove(m_ModsOnLateUpdate.begin(), m_ModsOnLateUpdate.end(), ptr), m_ModsOnLateUpdate.end());

        m_Context->SetCurrentMod(mod);
        ptr->Disconnect();
        m_Context->SetCurrentMod(nullptr);

        mod->SetFlags(0, MOD_CONNECTED);
        return 1;
    }, true);

    m_Flag &= ~BALLOON_MODS_CONNECTED;
}

void Balloon::OnProcess() {
    for (auto *mod: m_ModsOnUpdate) {
        mod->OnUpdate();
    }

    for (auto *mod: m_ModsOnLateUpdate) {
        mod->OnLateUpdate();
    }
}

Balloon::Balloon() = default;

bool Balloon::InitFileSystem() {
    auto &fs = FileSystem::GetInstance();

    if (!fs.IsInited() && !fs.Init(nullptr))
        return false;

    std::string path;
    {
        char szPath[MAX_PATH];
        char drive[4];
        char dir[MAX_PATH];
        ::GetModuleFileNameA(static_cast<HMODULE>(GetDllHandle()), szPath, MAX_PATH);
        _splitpath(szPath, drive, dir, nullptr, nullptr);
        sprintf(szPath, "%s%s", drive, dir);
        path = utils::RemoveTrailingPathSeparator(szPath);
    }

    std::string bin = "bin";
    std::string sep = fs.GetDirSeparator();
    std::string parentPath = utils::RemoveFileName(path);

    // Set up loader directory
    if (utils::EndsWithCaseInsensitive(path, bin)) {
        fs.SetLoaderDir(parentPath.c_str());
    } else if (utils::EndsWithCaseInsensitive(path, bin + sep + "Debug") ||
               utils::EndsWithCaseInsensitive(path, bin + sep + "Release")) {
        path = utils::RemoveTrailingPathSeparator(parentPath);
        fs.SetLoaderDir(utils::RemoveFileName(path).c_str());
    } else {
        path.append(fs.GetDirSeparator());
        fs.SetLoaderDir(path.c_str());
    }

    // Set up game directory
    std::string basePath(utils::RemoveTrailingPathSeparator(fs.GetDir(FS_DIR_BASE)));
    fs.SetGameDir(utils::RemoveFileName(basePath).c_str());

    // Set up user directory
    std::string userPath = fs.GetDir(FS_DIR_APPDATA);
    userPath += "Balloon";
    userPath += fs.GetDirSeparator();
    if (!utils::IsDirectoryExist(userPath)) {
        utils::CreateDir(userPath);
    }
    fs.Mount(userPath.c_str(), "/user", 1);

    fs.SetWriteDir(fs.GetDir(FS_DIR_LOADER));

    // Set up configs directory
    std::string configPath = "/configs";
    if (!fs.Exists(configPath.c_str())) {
        fs.MakeDir(configPath.c_str());
    }

    // Set up logs directory
    std::string logPath = "/logs";
    if (!fs.Exists(logPath.c_str())) {
        fs.MakeDir(logPath.c_str());
    }

    fs.SetWriteDir(nullptr);

//    path = fs.GetDir(FS_DIR_BASE);
//    path = fs.GetDir(FS_DIR_WORKING);
//    path = fs.GetDir(FS_DIR_USER);
//    path = fs.GetDir(FS_DIR_APPDATA);
//    path = fs.GetDir(FS_DIR_GAME);
//    path = fs.GetDir(FS_DIR_LOADER);

    return true;
}

bool Balloon::ShutdownFileSystem() {
    if (!FileSystem::GetInstance().Shutdown()) {
        return false;
    }
    return true;
}

void Balloon::InitLogger() {
    if (IsLoggerInited())
        return;

    auto logger = Logger::Create("Balloon");
    Logger::RegisterDefaultLogger(logger);
    CreateLogFile(logger);

#ifdef _DEBUG
    AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

    m_Flag |= BALLOON_LOGGER_INITED;
}

void Balloon::ShutdownLogger() {
    if (!IsLoggerInited())
        return;

    auto logger = Logger::Get("Balloon");
    logger->ClearCallbacks();

    for (auto it = m_LogFiles.rbegin(); it != m_LogFiles.rend(); ++it)
        if (*it) {
            fclose(*it);
            *it = nullptr;
        }
    m_LogFiles.clear();

    m_Flag &= ~BALLOON_LOGGER_INITED;
}

void Balloon::CreateLogFile(ILogger *logger) {
    if (!logger)
        return;

    auto &fs = FileSystem::GetInstance();

    std::string logFile = fs.GetDir(FS_DIR_LOADER);
    logFile += "\\logs\\";
    logFile += logger->GetId();
    logFile += ".log";

    FILE *fp = fopen(logFile.c_str(), "w");
    if (fp) {
        logger->AddCallback([](LogInfo *info) {
            char buf[64];
            buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", info->time)] = '\0';
            fprintf((FILE *) info->userdata, "[%s] [%s/%s]: ", buf, info->self->GetId(), info->self->GetLevelString(info->level));
            vfprintf((FILE *) info->userdata, info->format, info->ap);
            fprintf((FILE *) info->userdata, "\n");
            fflush((FILE *) info->userdata);
        }, fp, LOG_LEVEL_TRACE);

        m_LogFiles.push_back(fp);
    }
}

bool Balloon::LoadConfig(IConfig *config, const std::string &path) {
    if (!config || path.empty())
        return false;

    auto &fs = FileSystem::GetInstance();
    if (!fs.Exists(path.c_str())) {
        // LOG_ERROR("Config file %s does not exist.", path.c_str());
        return false;
    }

    auto file = fs.OpenRead(path.c_str());
    if (!file) {
        LOG_ERROR("Failed to open %s.", path.c_str());
        return false;
    }

    size_t len = fs.GetFileSize(file);
    if (len == 0) {
        LOG_ERROR("File %s is empty.", path.c_str());
        return false;
    }

    char *json = new char[len + 1];
    if (fs.ReadBytes(file, json, len) != len) {
        LOG_ERROR("Failed to read content from %s.", path.c_str());
        delete[] json;
        fs.Close(file);
        return false;
    };
    json[len] = '\0';
    fs.Close(file);

    if (!config->Read(json, len)) {
        delete[] json;
        return false;
    }

    delete[] json;
    return true;
}

bool Balloon::SaveConfig(IConfig *config, const std::string &path) {
    if (!config || path.empty())
        return false;

    size_t len = 0;
    char *json = config->Write(&len);
    if (!json || len == 0) {
        // LOG_WARN("Failed to generate json for config.");
        config->Free(json);
        return false;
    }

    auto &fs = FileSystem::GetInstance();
    auto file = fs.OpenWrite(path.c_str());
    if (!file) {
        LOG_WARN("Failed to open %s.", path.c_str());
        config->Free(json);
        return false;
    }

    if (fs.WriteBytes(file, json, len) != len) {
        LOG_WARN("Failed to write json to %s.", path.c_str());
        config->Free(json);
        fs.Close(file);
        return false;
    }

    config->Free(json);
    fs.Close(file);
    return true;
}

bool Balloon::LoadModConfig(const std::shared_ptr<ModContainer> &mod) {
    IConfig *config = Config::Get(mod->GetId());
    if (!config) {
        LOG_WARN("Can not create config for mod [%s].", mod->GetId());
        return false;
    }

    auto &fs = FileSystem::GetInstance();

    std::string configPath = "/configs/";
    configPath += mod->GetId();
    configPath += ".json";
    if (!fs.Exists(configPath.c_str())) {
        configPath = mod->GetRootPath();
        if (!fs.IsDirectory(configPath.c_str())) {
            configPath = utils::RemoveExtension(configPath, "");
        }

        configPath += "/";
        configPath += mod->GetId();
        configPath += ".json";

        if (!fs.Exists(configPath.c_str())) {
            config->Release();
            return false;
        }
    }

    bool ret = LoadConfig(config, configPath);
    config->Release();
    return ret;
}

bool Balloon::SaveModConfig(const std::shared_ptr<ModContainer> &mod) {
    // If a mod does not retrieve a config, no config will be saved.
    if ((mod->GetFlags() & MOD_CONFIG_RETRIEVED) == 0)
        return false;

    Config *config = Config::Get(mod->GetId());
    if (!config) {
        LOG_WARN("Can not retrieve config for mod [%s].", mod->GetId());
        return false;
    }

    auto &fs = FileSystem::GetInstance();
    fs.SetWriteDir(fs.GetDir(FS_DIR_LOADER));

    std::string configPath = "/configs/";
    configPath += mod->GetId();
    configPath += ".json";

    bool ret = SaveConfig(config, configPath);
    fs.SetWriteDir(nullptr);
    config->Release();
    return ret;
}

void Balloon::RegisterBuiltinInterfaces() {
    m_Context->RegisterInterface(&FileSystem::GetInstance(), "fs", 1);
    m_Context->RegisterInterface(&DataShare::GetInstance(), "ds", 1);
    m_Context->RegisterInterface(&EventManager::GetInstance(), "em", 1);
    m_Context->RegisterInterface(&Hook::GetInstance(), "hook", 1);
}

void Balloon::RegisterBuiltinFactories() {
    m_Context->RegisterFactory(&DataStackFactory::GetInstance(), "DataStack", 1);
    m_Context->RegisterFactory(&WeakRefFlagFactory::GetInstance(), "WeakRefFlag", 1);
}

static int OnError(HookModuleErrorCode code, void *data1, void *data2) {
    return HMR_OK;
}

static int OnQuery(HookModuleQueryCode code, void *data1, void *data2) {
    switch (code) {
        case HMQC_ABI:
            *reinterpret_cast<int *>(data2) = HOOKS_ABI_VERSION;
            break;
        case HMQC_CODE:
            *reinterpret_cast<int *>(data2) = BALLOON_HOOK_CODE;
            break;
        case HMQC_VERSION:
            *reinterpret_cast<int *>(data2) = BALLOON_HOOK_VERSION;
            break;
        case HMQC_CK2:
            *reinterpret_cast<int *>(data2) =
                CKHF_PostProcess |
                CKHF_PreClearAll |
                CKHF_OnCKInit |
                CKHF_OnCKEnd |
                CKHF_OnCKPostReset;
            break;
        default:
            return HMR_SKIP;
    }

    return HMR_OK;
}

static int OnPost(HookModulePostCode code, void *data1, void *data2) {
    switch (code) {
        case HMPC_WINDOW:
            Balloon::GetInstance().SetWindowHandle(reinterpret_cast<HWND>(data2));
            break;
        default:
            return HMR_SKIP;
    }
    return HMR_OK;
}

static int OnLoad(size_t code, void * /* handle */) {
    if (!Balloon::GetInstance().Init())
        return HMR_FAIL;
    return HMR_OK;
}

static int OnUnload(size_t code, void * /* handle */) {
    Balloon::GetInstance().Shutdown();
    return HMR_OK;
}

long PostProcess(void *arg) {
    Balloon::GetInstance().OnProcess();

    return 0;
}

long PreClearAll(void *arg) {
    auto &balloon = Balloon::GetInstance();
    balloon.DisconnectMods();

    return 0;
}

long OnCKInit(void *arg) {
    auto &balloon = Balloon::GetInstance();

    if (!balloon.LoadMods()) {
        LOG_ERROR("Failed to load mods!");
    }

    if (!balloon.InitMods()) {
        LOG_ERROR("Failed to init mods!");
    }

    return 0;
}

long OnCKEnd(void *arg) {
    auto &balloon = Balloon::GetInstance();
    balloon.ShutdownMods();
    balloon.UnloadMods();

    return 0;
}

long OnCKPostReset(void *arg) {
    if (!Balloon::GetInstance().ConnectMods()) {
        LOG_ERROR("Error occurred during connecting mods!");
    }

    return 0;
}

static int OnSet(size_t code, void **pcb, void **parg) {
    switch (code) {
        case CKHFI_PostProcess:
            *pcb = reinterpret_cast<void *>(PostProcess);
            *parg = nullptr;
            break;
        case CKHFI_PreClearAll:
            *pcb = reinterpret_cast<void *>(PreClearAll);
            *parg = nullptr;
            break;
        case CKHFI_OnCKInit:
            *pcb = reinterpret_cast<void *>(OnCKInit);
            *parg = nullptr;
            break;
        case CKHFI_OnCKEnd:
            *pcb = reinterpret_cast<void *>(OnCKEnd);
            *parg = nullptr;
            break;
        case CKHFI_OnCKPostReset:
            *pcb = reinterpret_cast<void *>(OnCKPostReset);
            *parg = nullptr;
            break;
        default:
            break;
    }
    return HMR_OK;
}

static int OnUnset(size_t code, void **pcb, void **parg) {
    switch (code) {
        case CKHFI_PostProcess:
            *pcb = reinterpret_cast<void *>(PostProcess);
            *parg = nullptr;
            break;
        case CKHF_PreClearAll:
            *pcb = reinterpret_cast<void *>(PreClearAll);
            *parg = nullptr;
            break;
        case CKHFI_OnCKInit:
            *pcb = reinterpret_cast<void *>(OnCKInit);
            *parg = nullptr;
            break;
        case CKHFI_OnCKEnd:
            *pcb = reinterpret_cast<void *>(OnCKEnd);
            *parg = nullptr;
            break;
        case CKHFI_OnCKPostReset:
            *pcb = reinterpret_cast<void *>(OnCKPostReset);
            *parg = nullptr;
            break;
        default:
            break;
    }
    return HMR_OK;
}

HOOKS_EXPORT int BalloonHandler(size_t action, size_t code, uintptr_t param1, uintptr_t param2) {
    switch (action) {
        case HMA_ERROR:
            return OnError(static_cast<HookModuleErrorCode>(code), reinterpret_cast<void *>(param1), reinterpret_cast<void *>(param2));
        case HMA_QUERY:
            return OnQuery(static_cast<HookModuleQueryCode>(code), reinterpret_cast<void *>(param1), reinterpret_cast<void *>(param2));
        case HMA_POST:
            return OnPost(static_cast<HookModulePostCode>(code), reinterpret_cast<void *>(param1), reinterpret_cast<void *>(param2));
        case HMA_LOAD:
            return OnLoad(code, reinterpret_cast<void *>(param2));
        case HMA_UNLOAD:
            return OnUnload(code, reinterpret_cast<void *>(param2));
        case HMA_SET:
            return OnSet(code, reinterpret_cast<void **>(param1), reinterpret_cast<void **>(param2));
        case HMA_UNSET:
            return OnUnset(code, reinterpret_cast<void **>(param1), reinterpret_cast<void **>(param2));
        default:
            return HMR_SKIP;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            Balloon::GetInstance().SetDllHandle(hModule);
            break;
        case DLL_PROCESS_DETACH:
            Balloon::GetInstance().SetDllHandle(nullptr);
            break;
        default:
            break;
    }
    return TRUE;
}