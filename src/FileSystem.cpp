#include "FileSystem.h"

#include <cassert>
#include <cstdlib>
#include <sstream>

#include <direct.h>

using namespace balloon;

namespace {
    size_t GetDriveDelim(const std::string &input) {
        for (size_t i = 0; i < input.size(); ++i)
            if (input[i] == '/' || input[i] == '\\')
                return i;
        return 0;
    }

    std::string GetDriveRoot(const std::string &input) {
        return input.substr(0, GetDriveDelim(input) + 1);
    }

    std::string SkipDriveRoot(const std::string &input) {
        return input.substr(GetDriveDelim(input) + 1);
    }

    std::string Normalize(const std::string &input) {
        std::stringstream out;
        bool seenSep = false, isSep = false;
        for (char i: input) {
            isSep = (i == BALLOON_PATH_SEPARATOR[0]);
            if (!isSep || !seenSep)
                out << i;
            seenSep = isSep;
        }
        return out.str();
    }
}

PHYSFS_Allocator FileSystem::s_Allocator = {
    nullptr,
    nullptr,
    [](PHYSFS_uint64 size) { return malloc(size); },
    [](void *ptr, PHYSFS_uint64 size) { return realloc(ptr, size); },
    [](void *ptr) { return free(ptr); }
};

FileSystem &FileSystem::GetInstance() {
    static FileSystem instance;
    return instance;
}

FileSystem::~FileSystem() = default;

bool FileSystem::IsInited() const {
    return PHYSFS_isInit() != 0;
}

bool FileSystem::Init(const char *arg) {
    if (!PHYSFS_init(arg))
        return false;

    // Enable symlinks by default.
    EnableSymbolicLinks(true);
    return true;
}

bool FileSystem::Shutdown() {
    if (IsInited())
        return PHYSFS_deinit() != 0;
    return true;
}

void FileSystem::FreeList(void *listVar) const {
    assert(IsInited());
    PHYSFS_freeList(listVar);
}

FileSystemErrorCode FileSystem::GetLastErrorCode() const {
    assert(IsInited());
    return static_cast<FileSystemErrorCode>(PHYSFS_getLastErrorCode());
}

void FileSystem::SetErrorCode(FileSystemErrorCode code) {
    assert(IsInited());
    PHYSFS_setErrorCode(static_cast<PHYSFS_ErrorCode>(code));
}

bool FileSystem::AreSymbolicLinksEnabled() const {
    assert(IsInited());
    return PHYSFS_symbolicLinksPermitted() != 0;
}

void FileSystem::EnableSymbolicLinks(bool allow) {
    assert(IsInited());
    PHYSFS_permitSymbolicLinks(allow);
}

const char *FileSystem::GetDirSeparator() const {
    return PHYSFS_getDirSeparator();
}

const char *FileSystem::GetDir(DirectoryType type) const {
    assert(IsInited());

    switch (type) {
        case FS_DIR_BASE:
            return PHYSFS_getBaseDir();
        case FS_DIR_WORKING:
            if (m_WorkingDir.empty()) {
                wchar_t cwd[BALLOON_MAX_PATH];
		        _wgetcwd(cwd, BALLOON_MAX_PATH);
                char buf[1024];
                Utf8FromUtf16(reinterpret_cast<const uint16_t *>(cwd), buf, 1024);
                m_WorkingDir = buf;
                m_WorkingDir.append(GetDirSeparator());
            }
            return m_WorkingDir.c_str();
        case FS_DIR_USER:
            if (m_UserDir.empty()) {
                m_UserDir = Normalize(PHYSFS_getUserDir());
            }
            return m_UserDir.c_str();
        case FS_DIR_APPDATA:
            if (m_AppDataDir.empty()) {
                wchar_t *appdata = _wgetenv(L"APPDATA");
                char buf[1024];
                Utf8FromUtf16(reinterpret_cast<const uint16_t *>(appdata), buf, 1024);
                m_AppDataDir = buf;
                m_AppDataDir.append(GetDirSeparator());
            }
            return m_AppDataDir.c_str();
        case FS_DIR_GAME:
            return m_GameDir.c_str();
        case FS_DIR_LOADER:
            return m_LoaderDir.c_str();
    }

    return nullptr;
}

bool FileSystem::SetGameDir(const char *dir) {
    assert(IsInited());

    if (!dir || utf8len(dir) == 0)
        return false;

    if (!IsInited())
        return false;

    if (!m_GameDir.empty())
        return false;

    m_GameDir = Normalize(dir);
    if (!Mount(m_GameDir.c_str(), "/game", 1)) {
        m_GameDir.clear();
        return false;
    }

    return true;
}

bool FileSystem::SetLoaderDir(const char *dir) {
    assert(IsInited());

    if (!dir || utf8len(dir) == 0)
        return false;

    if (!IsInited())
        return false;

    if (!m_LoaderDir.empty())
        return false;

    m_LoaderDir = Normalize(dir);
    if (!Mount(m_LoaderDir.c_str(), nullptr, 1)) {
        m_LoaderDir.clear();
        return false;
    }

    return true;
}

const char *FileSystem::GetWriteDir() const {
    assert(IsInited());
    return PHYSFS_getWriteDir();
}

int FileSystem::SetWriteDir(const char *newDir) {
    assert(IsInited());
    return PHYSFS_setWriteDir(newDir);
}

char **FileSystem::GetSearchPath() const {
    assert(IsInited());
    return PHYSFS_getSearchPath();
}

void FileSystem::GetSearchPathCallback(StringCallback callback, void *data) const {
    assert(IsInited());
    PHYSFS_getSearchPathCallback(callback, data);
}

const ArchiveInfo **FileSystem::GetSupportedArchiveTypes() const {
    assert(IsInited());
    return reinterpret_cast<const ArchiveInfo **>(PHYSFS_supportedArchiveTypes());
}

bool FileSystem::Mount(const char *newDir, const char *mountPoint, int appendToPath) {
    assert(IsInited());
    return PHYSFS_mount(newDir, mountPoint, appendToPath) != 0;
}

bool FileSystem::MountMemory(const void *buf, uint64_t len, void (*del)(void *), const char *newDir, const char *mountPoint, int appendToPath) {
    assert(IsInited());
    return PHYSFS_mountMemory(buf, len, del, newDir, mountPoint, appendToPath) != 0;
}

bool FileSystem::MountHandle(FileHandle handle, const char *newDir, const char *mountPoint, int appendToPath) {
    assert(IsInited());
    return PHYSFS_mountHandle(reinterpret_cast<PHYSFS_File *>(handle), newDir, mountPoint, appendToPath) != 0;
}

bool FileSystem::MountIO(IOInterface *io, const char *newDir, const char *mountPoint, int appendToPath) {
    assert(IsInited());
    return PHYSFS_mountIo(reinterpret_cast<PHYSFS_Io *>(io), newDir, mountPoint, appendToPath) != 0;
}

bool FileSystem::Unmount(const char *oldDir) {
    assert(IsInited());
    return PHYSFS_unmount(oldDir) != 0;
}

const char *FileSystem::GetMountPoint(const char *dir) const {
    assert(IsInited());
    return PHYSFS_getMountPoint(dir);
}

bool FileSystem::MakeDir(const char *dir) {
    assert(IsInited());
    return PHYSFS_mkdir(dir) != 0;
}

bool FileSystem::Delete(const char *filename) {
    assert(IsInited());
    return PHYSFS_delete(filename) != 0;
}

bool FileSystem::Copy(const char *src, const char *dest) {
    assert(IsInited());

    auto srcFile = OpenRead(src);
    if (!srcFile)
        return false;

    auto size = GetFileSize(srcFile);
    auto buf = new uint8_t[size];
    if (ReadBytes(srcFile, buf, size) != size) {
        Close(srcFile);
        delete[] buf;
        return false;
    }
    Close(srcFile);

    auto destFile = OpenWrite(dest);
    if (!destFile) {
        delete[] buf;
        return false;
    }

    if (WriteBytes(destFile, buf, size) != size) {
        delete[] buf;
        Close(destFile);
        return false;
    }

    delete[] buf;
    Close(destFile);
    return true;
}

const char *FileSystem::GetRealDir(const char *filename) const {
    assert(IsInited());
    return PHYSFS_getRealDir(filename);
}

int FileSystem::Enumerate(const char *dir, EnumerateCallback callback, void *data) const {
    assert(IsInited());
    return PHYSFS_enumerate(dir, reinterpret_cast<PHYSFS_EnumerateCallback>(callback), data);
}

char **FileSystem::EnumerateFiles(const char *dir) const {
    assert(IsInited());
    return PHYSFS_enumerateFiles(dir);
}

bool FileSystem::Exists(const char *filename) const {
    assert(IsInited());
    return PHYSFS_exists(filename) != 0;
}

bool FileSystem::IsDirectory(const char *filename) const {
    assert(IsInited());
    return PHYSFS_isDirectory(filename) != 0;
}

bool FileSystem::IsSymbolicLink(const char *filename) const {
    assert(IsInited());
    return PHYSFS_isSymbolicLink(filename) != 0;
}

bool FileSystem::Stat(const char *filename, StatInfo *stat) const {
    assert(IsInited());
    return PHYSFS_stat(filename, reinterpret_cast<PHYSFS_Stat *>(stat)) != 0;
}

FileHandle FileSystem::OpenWrite(const char *filename) const {
    assert(IsInited());
    return PHYSFS_openWrite(filename);
}

FileHandle FileSystem::OpenAppend(const char *filename) const {
    assert(IsInited());
    return PHYSFS_openAppend(filename);
}

FileHandle FileSystem::OpenRead(const char *filename) const {
    assert(IsInited());
    return PHYSFS_openRead(filename);
}

void FileSystem::Close(FileHandle handle) const {
    assert(IsInited());
    PHYSFS_close(reinterpret_cast<PHYSFS_File *>(handle));
}

bool FileSystem::EndOfFile(FileHandle handle) const {
    assert(IsInited());
    return PHYSFS_eof(reinterpret_cast<PHYSFS_File *>(handle));
}

int64_t FileSystem::Tell(FileHandle handle) const {
    assert(IsInited());
    return PHYSFS_tell(reinterpret_cast<PHYSFS_File *>(handle));
}

void FileSystem::Seek(FileHandle handle, uint64_t pos) const {
    assert(IsInited());
    PHYSFS_seek(reinterpret_cast<PHYSFS_File *>(handle), pos);
}

int64_t FileSystem::GetFileSize(FileHandle handle) const {
    assert(IsInited());
    return PHYSFS_fileLength(reinterpret_cast<PHYSFS_File *>(handle));
}

bool FileSystem::SetBuffer(FileHandle handle, uint64_t size) const {
    assert(IsInited());
    return PHYSFS_setBuffer(reinterpret_cast<PHYSFS_File *>(handle), size);
}

void FileSystem::Flush(FileHandle handle) const {
    assert(IsInited());
    PHYSFS_flush(reinterpret_cast<PHYSFS_File *>(handle));
}

int64_t FileSystem::ReadBytes(FileHandle handle, void *buffer, uint64_t len) const {
    assert(IsInited());
    return PHYSFS_readBytes(reinterpret_cast<PHYSFS_File *>(handle), buffer, len);
}

int64_t FileSystem::WriteBytes(FileHandle handle, const void *buffer, uint64_t len) const {
    assert(IsInited());
    return PHYSFS_writeBytes(reinterpret_cast<PHYSFS_File *>(handle), buffer, len);
}

int FileSystem::ReadSLE16(FileHandle handle, int16_t *value) const {
    assert(IsInited());
    return PHYSFS_readSLE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadULE16(FileHandle handle, uint16_t *value) const {
    assert(IsInited());
    return PHYSFS_readULE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadSLE32(FileHandle handle, int32_t *value) const {
    assert(IsInited());
    return PHYSFS_readSLE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadULE32(FileHandle handle, uint32_t *value) const {
    assert(IsInited());
    return PHYSFS_readULE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadSLE64(FileHandle handle, int64_t *value) const {
    assert(IsInited());
    return PHYSFS_readSLE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadULE64(FileHandle handle, uint64_t *value) const {
    assert(IsInited());
    return PHYSFS_readULE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadSBE16(FileHandle handle, int16_t *value) const {
    assert(IsInited());
    return PHYSFS_readSBE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadUBE16(FileHandle handle, uint16_t *value) const {
    assert(IsInited());
    return PHYSFS_readUBE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadSBE32(FileHandle handle, int32_t *value) const {
    assert(IsInited());
    return PHYSFS_readSBE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadUBE32(FileHandle handle, uint32_t *value) const {
    assert(IsInited());
    return PHYSFS_readUBE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadSBE64(FileHandle handle, int64_t *value) const {
    assert(IsInited());
    return PHYSFS_readSBE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::ReadUBE64(FileHandle handle, uint64_t *value) const {
    assert(IsInited());
    return PHYSFS_readUBE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteSLE16(FileHandle handle, int16_t value) const {
    assert(IsInited());
    return PHYSFS_writeSLE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteULE16(FileHandle handle, uint16_t value) const {
    assert(IsInited());
    return PHYSFS_writeULE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteSLE32(FileHandle handle, int32_t value) const {
    assert(IsInited());
    return PHYSFS_writeSLE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteULE32(FileHandle handle, uint32_t value) const {
    assert(IsInited());
    return PHYSFS_writeULE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteSLE64(FileHandle handle, int64_t value) const {
    assert(IsInited());
    return PHYSFS_writeSLE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteULE64(FileHandle handle, uint64_t value) const {
    assert(IsInited());
    return PHYSFS_writeULE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteSBE16(FileHandle handle, int16_t value) const {
    assert(IsInited());
    return PHYSFS_writeSBE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteUBE16(FileHandle handle, uint16_t value) const {
    assert(IsInited());
    return PHYSFS_writeUBE16(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteSBE32(FileHandle handle, int32_t value) const {
    assert(IsInited());
    return PHYSFS_writeSBE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteUBE32(FileHandle handle, uint32_t value) const {
    assert(IsInited());
    return PHYSFS_writeUBE32(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteSBE64(FileHandle handle, int64_t value) const {
    return PHYSFS_writeSBE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int FileSystem::WriteUBE64(FileHandle handle, uint64_t value) const {
    assert(IsInited());
    return PHYSFS_writeUBE64(reinterpret_cast<PHYSFS_File *>(handle), value);
}

int16_t FileSystem::SwapSLE16(int16_t value) const {
    return PHYSFS_swapSLE16(value);
}

uint16_t FileSystem::SwapULE16(uint16_t value) const {
    return PHYSFS_swapULE16(value);
}

int32_t FileSystem::SwapSLE32(int32_t value) const {
    return PHYSFS_swapSLE32(value);
}

uint32_t FileSystem::SwapULE32(uint32_t value) const {
    return PHYSFS_swapULE32(value);
}

int64_t FileSystem::SwapSLE64(int64_t value) const {
    return PHYSFS_swapSLE64(value);
}

uint64_t FileSystem::SwapULE64(uint64_t value) const {
    return PHYSFS_swapULE64(value);
}

int16_t FileSystem::SwapSBE16(int16_t value) const {
    return PHYSFS_swapSBE16(value);
}

uint16_t FileSystem::SwapUBE16(uint16_t value) const {
    return PHYSFS_swapUBE16(value);
}

int32_t FileSystem::SwapSBE32(int32_t value) const {
    return PHYSFS_swapSBE32(value);
}

uint32_t FileSystem::SwapUBE32(uint32_t value) const {
    return PHYSFS_swapUBE32(value);
}

int64_t FileSystem::SwapSBE64(int64_t value) const {
    return PHYSFS_swapSBE64(value);
}

uint64_t FileSystem::SwapUBE64(uint64_t value) const {
    return PHYSFS_swapUBE64(value);
}

void FileSystem::Utf8FromUtf16(const uint16_t *src, char *dst, uint64_t len) const {
    return PHYSFS_utf8FromUtf16(src, dst, len);
}

void FileSystem::Utf8ToUtf16(const char *src, uint16_t *dst, uint64_t len) const {
    return PHYSFS_utf8ToUtf16(src, dst, len);
}

void FileSystem::Utf8FromUcs4(const uint32_t *src, char *dst, uint64_t len) const {
    return PHYSFS_utf8FromUcs4(src, dst, len);
}

void FileSystem::Utf8ToUcs4(const char *src, uint32_t *dst, uint64_t len) const {
    return PHYSFS_utf8ToUcs4(src, dst, len);
}

void FileSystem::Utf8FromUcs2(const uint16_t *src, char *dst, uint64_t len) const {
    return PHYSFS_utf8FromUcs2(src, dst, len);
}

void FileSystem::Utf8ToUcs2(const char *src, uint16_t *dst, uint64_t len) const {
    return PHYSFS_utf8ToUcs2(src, dst, len);
}

void FileSystem::Utf8FromLatin1(const char *src, char *dst, uint64_t len) const {
    return PHYSFS_utf8FromLatin1(src, dst, len);
}

int FileSystem::CaseFold(uint32_t from, uint32_t *to) const {
    return PHYSFS_caseFold(from, to);
}

int FileSystem::Utf8Stricmp(const char *str1, const char *str2) const {
    return PHYSFS_utf8stricmp(str1, str2);
}

int FileSystem::Utf16Stricmp(const uint16_t *str1, const uint16_t *str2) const {
    return PHYSFS_utf16stricmp(str1, str2);
}

int FileSystem::Ucs4Stricmp(const uint32_t *str1, const uint32_t *str2) const {
    return PHYSFS_ucs4stricmp(str1, str2);
}

FileSystem::FileSystem() {
    PHYSFS_setAllocator(&s_Allocator);
}