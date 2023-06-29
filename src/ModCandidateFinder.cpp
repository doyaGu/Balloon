#include "ModCandidateFinder.h"

#include <utility>

#include "FileSystem.h"
#include "Logger.h"
#include "StringUtils.h"
#include "PathUtils.h"

using namespace balloon;

DirectoryModCandidateFinder::DirectoryModCandidateFinder(std::string path) : m_Path(std::move(path)) {}

bool DirectoryModCandidateFinder::FindCandidates(ModCandidateConsumer &out) {
    if (m_Path.empty())
        return false;

    auto &fs = FileSystem::GetInstance();

    if (!fs.Exists(m_Path.c_str()))
        return false;

    char **rc = fs.EnumerateFiles(m_Path.c_str());
    char **i;
    for (i = rc; *i != nullptr; i++) {
        std::string path(m_Path);
        path += '/';
        path += *i;

        // A valid mod can be a directory with the required files or an archive in supported format.
        if (fs.IsDirectory(path.c_str())) {
            if (IsValidMod(path)) {
                out(path);
            }
        } else if (IsSupportedArchive(path)) {
            std::string mountPoint = utils::RemoveExtension(path, "");
            if (fs.Exists(mountPoint.c_str())) {
                LOG_WARN("Found mod directory and archive with the same name, the archive will be ignored.");
                continue;
            } else {
                // Calculate the path in platform-dependent notation
                std::string realPath = utils::JoinPaths(fs.GetDir(FS_DIR_LOADER), path);
                utils::NormalizePath(realPath);
                // Mount the mod archive
                if (fs.Mount(realPath.c_str(), mountPoint.c_str(), 1) != 0) {
                    if (IsValidMod(mountPoint)) {
                        out(path);
                    } else {
                        fs.Unmount(realPath.c_str());
                    }
                }
            }
        }
    }
    fs.FreeList(rc);

    return true;
}

bool DirectoryModCandidateFinder::IsSupportedArchive(const std::string &path) {
    auto &fs = FileSystem::GetInstance();

    std::string ext = path.substr(path.rfind('.') + 1);

    const ArchiveInfo **i;
    for (i = fs.GetSupportedArchiveTypes(); *i != nullptr; i++) {
        if (stricmp(ext.c_str(), (*i)->extension) == 0) {
            return true;
        }
    }
    return false;
}

bool DirectoryModCandidateFinder::IsValidMod(const std::string &path) {
    auto &fs = FileSystem::GetInstance();

    // A valid mod should first have a manifest file named as "balloon.mod.json"
    // this manifest file must be a valid JSON text file which is in conformance to
    // the mod metadata schema.
    std::string modManifest = path + "/balloon.mod.json";
    if (!fs.Exists(modManifest.c_str())) {
        return false;
    }

    // A valid mod should have a directory named "bin",
    // and the mod library should be put in this directory.
    std::string modBin = path + "/bin/";
    if (!fs.Exists(modBin.c_str()))
        return false;

    bool hasDll = false;
    char **rc = fs.EnumerateFiles(modBin.c_str());
    char **i;
    for (i = rc; *i != nullptr; i++) {
        std::string p(modBin);
        p += *i;
        if (utils::EndsWithCaseInsensitive(p, ".dll")) {
            hasDll = true;
            break;
        }
    }
    fs.FreeList(rc);

    return hasDll;
}

