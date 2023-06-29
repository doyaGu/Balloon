#ifndef BALLOON_FILESYSTEM_H
#define BALLOON_FILESYSTEM_H

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

#include "Balloon/IFileSystem.h"

#include <physfs.h>
#include <utf8.h>

#define BALLOON_PATH_SEPARATOR "/"
#define BALLOON_MAX_PATH 260

namespace balloon {
    class FileSystem final : public IFileSystem {
    public:
        static FileSystem &GetInstance();

        FileSystem(const FileSystem &rhs) = delete;
        FileSystem(FileSystem &&rhs) noexcept = delete;

        ~FileSystem();

        FileSystem &operator=(const FileSystem &rhs) = delete;
        FileSystem &operator=(FileSystem &&rhs) noexcept = delete;

        bool IsInited() const;
        bool Init(const char *arg);
        bool Shutdown();

        void FreeList(void *listVar) const override;

        FileSystemErrorCode GetLastErrorCode() const override;
        void SetErrorCode(FileSystemErrorCode code) override;

        bool AreSymbolicLinksEnabled() const override;
        void EnableSymbolicLinks(bool allow) override;

        const char *GetDirSeparator() const override;
        const char *GetDir(DirectoryType type) const override;

        bool SetGameDir(const char *dir);
        bool SetLoaderDir(const char *dir);

        const char *GetWriteDir() const override;
        int SetWriteDir(const char *newDir) override;

        char **GetSearchPath() const override;
        void GetSearchPathCallback(StringCallback callback, void *data) const override;

        const ArchiveInfo **GetSupportedArchiveTypes() const override;

        bool Mount(const char *newDir, const char *mountPoint, int appendToPath) override;
        bool MountMemory(const void *buf, uint64_t len, void (*del)(void *), const char *newDir, const char *mountPoint, int appendToPath) override;
        bool MountHandle(FileHandle handle, const char *newDir, const char *mountPoint, int appendToPath) override;
        bool MountIO(IOInterface *io, const char *newDir, const char *mountPoint, int appendToPath) override;

        bool Unmount(const char *oldDir) override;

        const char *GetMountPoint(const char *dir) const override;

        bool MakeDir(const char *dir) override;
        bool Delete(const char *filename) override;
        bool Copy(const char *src, const char *dest) override;

        const char *GetRealDir(const char *filename) const override;

        int Enumerate(const char *dir, EnumerateCallback callback, void *data) const override;
        char **EnumerateFiles(const char *dir) const override;

        bool Exists(const char *filename) const override;
        bool IsDirectory(const char *filename) const override;
        bool IsSymbolicLink(const char *filename) const override;

        bool Stat(const char *filename, StatInfo *stat) const override;

        FileHandle OpenWrite(const char *filename) const override;
        FileHandle OpenAppend(const char *filename) const override;
        FileHandle OpenRead(const char *filename) const override;

        void Close(FileHandle handle) const override;

        bool EndOfFile(FileHandle handle) const override;
        int64_t Tell(FileHandle handle) const override;
        void Seek(FileHandle handle, uint64_t pos) const override;
        int64_t GetFileSize(FileHandle handle) const override;
        bool SetBuffer(FileHandle handle, uint64_t size) const override;
        void Flush(FileHandle handle) const override;

        int64_t ReadBytes(FileHandle handle, void *buffer, uint64_t len) const override;
        int64_t WriteBytes(FileHandle handle, const void *buffer, uint64_t len) const override;

        int ReadSLE16(FileHandle handle, int16_t *value) const override;
        int ReadULE16(FileHandle handle, uint16_t *value) const override;

        int ReadSLE32(FileHandle handle, int32_t *value) const override;
        int ReadULE32(FileHandle handle, uint32_t *value) const override;

        int ReadSLE64(FileHandle handle, int64_t *value) const override;
        int ReadULE64(FileHandle handle, uint64_t *value) const override;

        int ReadSBE16(FileHandle handle, int16_t *value) const override;
        int ReadUBE16(FileHandle handle, uint16_t *value) const override;

        int ReadSBE32(FileHandle handle, int32_t *value) const override;
        int ReadUBE32(FileHandle handle, uint32_t *value) const override;

        int ReadSBE64(FileHandle handle, int64_t *value) const override;
        int ReadUBE64(FileHandle handle, uint64_t *value) const override;

        int WriteSLE16(FileHandle handle, int16_t value) const override;
        int WriteULE16(FileHandle handle, uint16_t value) const override;

        int WriteSLE32(FileHandle handle, int32_t value) const override;
        int WriteULE32(FileHandle handle, uint32_t value) const override;

        int WriteSLE64(FileHandle handle, int64_t value) const override;
        int WriteULE64(FileHandle handle, uint64_t value) const override;

        int WriteSBE16(FileHandle handle, int16_t value) const override;
        int WriteUBE16(FileHandle handle, uint16_t value) const override;

        int WriteSBE32(FileHandle handle, int32_t value) const override;
        int WriteUBE32(FileHandle handle, uint32_t value) const override;

        int WriteSBE64(FileHandle handle, int64_t value) const override;
        int WriteUBE64(FileHandle handle, uint64_t value) const override;

        int16_t SwapSLE16(int16_t value) const override;
        uint16_t SwapULE16(uint16_t value) const override;

        int32_t SwapSLE32(int32_t value) const override;
        uint32_t SwapULE32(uint32_t value) const override;

        int64_t SwapSLE64(int64_t value) const override;
        uint64_t SwapULE64(uint64_t value) const override;

        int16_t SwapSBE16(int16_t value) const override;
        uint16_t SwapUBE16(uint16_t value) const override;

        int32_t SwapSBE32(int32_t value) const override;
        uint32_t SwapUBE32(uint32_t value) const override;

        int64_t SwapSBE64(int64_t value) const override;
        uint64_t SwapUBE64(uint64_t value) const override;

        void Utf8FromUtf16(const uint16_t *src, char *dst, uint64_t len) const override;
        void Utf8ToUtf16(const char *src, uint16_t *dst, uint64_t len) const override;

        void Utf8FromUcs4(const uint32_t *src, char *dst, uint64_t len) const override;
        void Utf8ToUcs4(const char *src, uint32_t *dst, uint64_t len) const override;

        void Utf8FromUcs2(const uint16_t *src, char *dst, uint64_t len) const override;
        void Utf8ToUcs2(const char *src, uint16_t *dst, uint64_t len) const override;

        void Utf8FromLatin1(const char *src, char *dst, uint64_t len) const override;

        int CaseFold(uint32_t from, uint32_t *to) const override;

        int Utf8Stricmp(const char *str1, const char *str2) const override;
        int Utf16Stricmp(const uint16_t *str1, const uint16_t *str2) const override;
        int Ucs4Stricmp(const uint32_t *str1, const uint32_t *str2) const override;

    private:
        FileSystem();

        mutable std::string m_WorkingDir;
        mutable std::string m_UserDir;
        mutable std::string m_AppDataDir;
        std::string m_GameDir;
        std::string m_LoaderDir;

        static PHYSFS_Allocator s_Allocator;
    };
}


#endif // BALLOON_FILESYSTEM_H
