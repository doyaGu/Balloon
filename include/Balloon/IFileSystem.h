/**
 * @file IFileSystem.h
 * @brief The interface of file system.
 */
#ifndef BALLOON_IFILESYSTEM_H
#define BALLOON_IFILESYSTEM_H

namespace balloon {
    inline namespace v1 {
        typedef void *FileHandle; /**< An opaque pointer representing the file handle. */

        typedef enum FileSystemErrorCode {
            FS_ERROR_OK,               /**< Success; no error.                    */
            FS_ERROR_OTHER_ERROR,      /**< Error not otherwise covered here.     */
            FS_ERROR_OUT_OF_MEMORY,    /**< Memory allocation failed.             */
            FS_ERROR_NOT_INITIALIZED,  /**< FileSystem is not initialized.        */
            FS_ERROR_IS_INITIALIZED,   /**< FileSystem is already initialized.    */
            FS_ERROR_ARGV0_IS_NULL,    /**< Needed argv[0], but it is NULL.       */
            FS_ERROR_UNSUPPORTED,      /**< Operation or feature unsupported.     */
            FS_ERROR_PAST_EOF,         /**< Attempted to access past end of file. */
            FS_ERROR_FILES_STILL_OPEN, /**< Files still open.                     */
            FS_ERROR_INVALID_ARGUMENT, /**< Bad parameter passed to an function.  */
            FS_ERROR_NOT_MOUNTED,      /**< Requested archive/dir not mounted.    */
            FS_ERROR_NOT_FOUND,        /**< File (or whatever) not found.         */
            FS_ERROR_SYMLINK_FORBIDDEN,/**< Symlink seen when not permitted.      */
            FS_ERROR_NO_WRITE_DIR,     /**< No write dir has been specified.      */
            FS_ERROR_OPEN_FOR_READING, /**< Wrote to a file opened for reading.   */
            FS_ERROR_OPEN_FOR_WRITING, /**< Read from a file opened for writing.  */
            FS_ERROR_NOT_A_FILE,       /**< Needed a file, got a directory (etc). */
            FS_ERROR_READ_ONLY,        /**< Wrote to a read-only filesystem.      */
            FS_ERROR_CORRUPT,          /**< Corrupted data encountered.           */
            FS_ERROR_SYMLINK_LOOP,     /**< Infinite symbolic link loop.          */
            FS_ERROR_IO,               /**< i/o error (hardware failure, etc).    */
            FS_ERROR_PERMISSION,       /**< Permission denied.                    */
            FS_ERROR_NO_SPACE,         /**< No space (disk full, over quota, etc) */
            FS_ERROR_BAD_FILENAME,     /**< Filename is bogus/insecure.           */
            FS_ERROR_BUSY,             /**< Tried to modify a file the OS needs.  */
            FS_ERROR_DIR_NOT_EMPTY,    /**< Tried to delete dir with files in it. */
            FS_ERROR_OS_ERROR,         /**< Unspecified OS-level error.           */
            FS_ERROR_DUPLICATE,        /**< Duplicate entry.                      */
            FS_ERROR_BAD_PASSWORD,     /**< Bad password.                         */
            FS_ERROR_APP_CALLBACK      /**< Application callback reported error.  */
        } FileSystemErrorCode;

        typedef enum FileType {
            FS_FILE_REGULAR,     /**< a normal file */
            FS_FILE_DIRECTORY,   /**< a directory */
            FS_FILE_SYMLINK,     /**< a symlink */
            FS_FILE_OTHER        /**< something completely different like a device */
        } FileType;

        typedef enum DirectoryType {
            FS_DIR_BASE = 0,       /**< Base directory in platform-dependent notation. */
            FS_DIR_WORKING = 1,    /**< Current working directory in platform-dependent notation. */
            FS_DIR_USER = 2,       /**< User directory in platform-dependent notation. */
            FS_DIR_APPDATA = 3,    /**< AppData directory in platform-dependent notation. */
            FS_DIR_GAME = 4,       /**< Game root directory in platform-dependent notation. */
            FS_DIR_LOADER = 5,     /**< Loader root directory in platform-dependent notation. */
        } DirectoryType;

        typedef enum FileSystemEnumerateCallbackResult {
            FS_RET_ERROR = -1,   /**< Stop enumerating, report error to app. */
            FS_RET_STOP = 0,     /**< Stop enumerating, report success to app. */
            FS_RET_OK = 1        /**< Keep enumerating, no problems */
        } FileSystemEnumerateCallbackResult;

        struct ArchiveInfo {
            const char *extension;   /**< Archive file extension: "ZIP", for example. */
            const char *description; /**< Human-readable archive description. */
            const char *author;      /**< Person who did support for this archive. */
            const char *url;         /**< URL related to this archive */
            int supportsSymlinks;    /**< non-zero if archive offers symbolic links. */
        };

        struct IOInterface {
            uint32_t version;
            void *opaque;
            int64_t (*read)(struct IOInterface *io, void *buf, uint64_t len);
            int64_t (*write)(struct IOInterface *io, const void *buffer, uint64_t len);
            int (*seek)(struct IOInterface *io, uint64_t offset);
            int64_t (*tell)(struct IOInterface *io);
            int64_t (*length)(struct IOInterface *io);
            struct IOInterface *(*duplicate)(struct IOInterface *io);
            int (*flush)(struct IOInterface *io);
            void (*destroy)(struct IOInterface *io);
        };

        struct StatInfo {
            int64_t filesize;     /**< size in bytes, -1 for non-files and unknown */
            int64_t modtime;      /**< last modification time */
            int64_t createtime;   /**< like modtime, but for file creation time */
            int64_t accesstime;   /**< like modtime, but for file access time */
            FileType filetype;    /**< File? Directory? Symlink? */
            int readonly;         /**< non-zero if read only, zero if writable. */
        };

        typedef void (*StringCallback)(void *, const char *);
        typedef FileSystemEnumerateCallbackResult (*EnumerateCallback)(void *,const char *,const char *);

        class IFileSystem {
        public:
            virtual void FreeList(void *listVar) const = 0;

            virtual FileSystemErrorCode GetLastErrorCode() const = 0;
            virtual void SetErrorCode(FileSystemErrorCode code) = 0;

            virtual bool AreSymbolicLinksEnabled() const = 0;
            virtual void EnableSymbolicLinks(bool allow) = 0;

            virtual const char *GetDirSeparator() const = 0;
            virtual const char *GetDir(DirectoryType type) const = 0;

            virtual const char *GetWriteDir() const = 0;
            virtual int SetWriteDir(const char *newDir) = 0;

            virtual char **GetSearchPath() const = 0;
            virtual void GetSearchPathCallback(StringCallback callback, void *data) const = 0;

            virtual const ArchiveInfo **GetSupportedArchiveTypes() const = 0;

            virtual bool Mount(const char *newDir, const char *mountPoint, int appendToPath) = 0;
            virtual bool MountMemory(const void *buf, uint64_t len, void(*del)(void *), const char *newDir, const char *mountPoint, int appendToPath) = 0;
            virtual bool MountHandle(FileHandle handle, const char *newDir, const char *mountPoint, int appendToPath) = 0;
            virtual bool MountIO(IOInterface *io, const char *newDir, const char *mountPoint, int appendToPath) = 0;

            virtual bool Unmount(const char *oldDir) = 0;

            virtual const char *GetMountPoint(const char *dir) const = 0;

            virtual bool MakeDir(const char *dir) = 0;
            virtual bool Delete(const char *filename) = 0;
            virtual bool Copy(const char *src, const char *dest) = 0;

            virtual const char *GetRealDir(const char *filename) const = 0;

            virtual int Enumerate(const char *dir, EnumerateCallback callback, void *data) const = 0;
            virtual char **EnumerateFiles(const char *dir) const = 0;

            virtual bool Exists(const char *filename) const = 0;
            virtual bool IsDirectory(const char *filename) const = 0;
            virtual bool IsSymbolicLink(const char *filename) const = 0;

            virtual bool Stat(const char *filename, StatInfo *stat) const = 0;

            virtual FileHandle OpenWrite(const char *filename) const = 0;
            virtual FileHandle OpenAppend(const char *filename) const = 0;
            virtual FileHandle OpenRead(const char *filename) const = 0;

            virtual void Close(FileHandle handle) const = 0;

            virtual bool EndOfFile(FileHandle handle) const = 0;
            virtual int64_t Tell(FileHandle handle) const = 0;
            virtual void Seek(FileHandle handle, uint64_t pos) const = 0;
            virtual int64_t GetFileSize(FileHandle handle) const = 0;
            virtual bool SetBuffer(FileHandle handle, uint64_t size) const = 0;
            virtual void Flush(FileHandle handle) const = 0;

            virtual int64_t ReadBytes(FileHandle handle, void *buffer, uint64_t len) const = 0;
            virtual int64_t WriteBytes(FileHandle handle, const void *buffer, uint64_t len) const = 0;

            virtual int ReadSLE16(FileHandle handle, int16_t *value) const = 0;
            virtual int ReadULE16(FileHandle handle, uint16_t *value) const = 0;

            virtual int ReadSLE32(FileHandle handle, int32_t *value) const = 0;
            virtual int ReadULE32(FileHandle handle, uint32_t *value) const = 0;

            virtual int ReadSLE64(FileHandle handle, int64_t *value) const = 0;
            virtual int ReadULE64(FileHandle handle, uint64_t *value) const = 0;

            virtual int ReadSBE16(FileHandle handle, int16_t *value) const = 0;
            virtual int ReadUBE16(FileHandle handle, uint16_t *value) const = 0;

            virtual int ReadSBE32(FileHandle handle, int32_t *value) const = 0;
            virtual int ReadUBE32(FileHandle handle, uint32_t *value) const = 0;

            virtual int ReadSBE64(FileHandle handle, int64_t *value) const = 0;
            virtual int ReadUBE64(FileHandle handle, uint64_t *value) const = 0;

            virtual int WriteSLE16(FileHandle handle, int16_t value) const = 0;
            virtual int WriteULE16(FileHandle handle, uint16_t value) const = 0;

            virtual int WriteSLE32(FileHandle handle, int32_t value) const = 0;
            virtual int WriteULE32(FileHandle handle, uint32_t value) const = 0;

            virtual int WriteSLE64(FileHandle handle, int64_t value) const = 0;
            virtual int WriteULE64(FileHandle handle, uint64_t value) const = 0;

            virtual int WriteSBE16(FileHandle handle, int16_t value) const = 0;
            virtual int WriteUBE16(FileHandle handle, uint16_t value) const = 0;

            virtual int WriteSBE32(FileHandle handle, int32_t value) const = 0;
            virtual int WriteUBE32(FileHandle handle, uint32_t value) const = 0;

            virtual int WriteSBE64(FileHandle handle, int64_t value) const = 0;
            virtual int WriteUBE64(FileHandle handle, uint64_t value) const = 0;

            virtual int16_t SwapSLE16(int16_t value) const = 0;
            virtual uint16_t SwapULE16(uint16_t value) const = 0;

            virtual int32_t SwapSLE32(int32_t value) const = 0;
            virtual uint32_t SwapULE32(uint32_t value) const = 0;

            virtual int64_t SwapSLE64(int64_t value) const = 0;
            virtual uint64_t SwapULE64(uint64_t value) const = 0;

            virtual int16_t SwapSBE16(int16_t value) const = 0;
            virtual uint16_t SwapUBE16(uint16_t value) const = 0;

            virtual int32_t SwapSBE32(int32_t value) const = 0;
            virtual uint32_t SwapUBE32(uint32_t value) const = 0;

            virtual int64_t SwapSBE64(int64_t value) const = 0;
            virtual uint64_t SwapUBE64(uint64_t value) const = 0;

            virtual void Utf8FromUtf16(const uint16_t *src, char *dst, uint64_t len) const = 0;
            virtual void Utf8ToUtf16(const char *src, uint16_t *dst, uint64_t len) const = 0;

            virtual void Utf8FromUcs4(const uint32_t *src, char *dst, uint64_t len) const = 0;
            virtual void Utf8ToUcs4(const char *src, uint32_t *dst, uint64_t len) const = 0;

            virtual void Utf8FromUcs2(const uint16_t *src, char *dst, uint64_t len) const = 0;
            virtual void Utf8ToUcs2(const char *src, uint16_t *dst, uint64_t len) const = 0;

            virtual void Utf8FromLatin1(const char *src, char *dst, uint64_t len) const = 0;

            virtual int CaseFold(uint32_t from, uint32_t *to) const = 0;

            virtual int Utf8Stricmp(const char *str1, const char *str2) const = 0;
            virtual int Utf16Stricmp(const uint16_t *str1, const uint16_t *str2) const = 0;
            virtual int Ucs4Stricmp(const uint32_t *str1, const uint32_t *str2) const = 0;
        };
    }
}

#endif // BALLOON_IFILESYSTEM_H
