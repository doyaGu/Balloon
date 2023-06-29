#include "ModCandidateScanner.h"

#include "FileSystem.h"
#include "Logger.h"
#include "PathUtils.h"
#include "ModMetadataParser.h"

using namespace balloon;

ModCandidateScanner::ModCandidateScanner() = default;

ModCandidateScanner::~ModCandidateScanner() = default;

bool ModCandidateScanner::ScanCandidate(const std::string &path, ModCandidate &out) {
    auto &fs = FileSystem::GetInstance();
    StatInfo stat;
    fs.Stat(path.c_str(), &stat);
    if (stat.filetype == FS_FILE_DIRECTORY) {
        return ScanDirectory(path, out);
    } else if (stat.filetype == FS_FILE_REGULAR) {
        return ScanArchive(path, out);
    }

    return false;
}

bool ModCandidateScanner::ScanDirectory(const std::string &path, ModCandidate &out) {
    auto &fs = FileSystem::GetInstance();

    std::string modManifest = path + "/balloon.mod.json";
    FileHandle file = fs.OpenRead(modManifest.c_str());
    if (!file) {
        LOG_ERROR("Failed to open %s", modManifest.c_str());
        return false;
    }

    size_t size = fs.GetFileSize(file);
    auto buf = std::shared_ptr<char>(new char[size]);
    if (fs.ReadBytes(file, buf.get(), size) != size) {
        fs.Close(file);
        return false;
    }
    fs.Close(file);

    auto metadata = ModMetadataParser::GetInstance().Parse(buf.get(), size);
    if (!metadata) {
        LOG_ERROR("Failed to parse %s", modManifest.c_str());
        return false;
    }

    out = std::move(ModCandidate(path, metadata));
    return true;
}

bool ModCandidateScanner::ScanArchive(const std::string &path, ModCandidate &out) {
    std::string dir = utils::RemoveExtension(path, "");
    if (!ScanDirectory(dir, out))
        return false;
    out.SetPath(path);
    return true;
}
