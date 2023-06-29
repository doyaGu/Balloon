/**
 * @file IConfig.h
 * @brief The interface of config.
 */
#ifndef BALLOON_ICONFIG_H
#define BALLOON_ICONFIG_H

#include <cstdint>

namespace balloon {
    inline namespace v1 {
        class IConfigSection;
        class IConfigEntry;

        /**
         * @interface IConfig
         * @brief The utility interface of config.
         * @warning
         * Mod should have no classes implementing this interface.
         */
        class IConfig {
        public:
            virtual int AddRef() const = 0;
            virtual int Release() const = 0;

            virtual const char *GetId() const = 0;

            virtual void Clear() = 0;
            virtual void ClearDefault() = 0;

            virtual size_t GetNumberOfEntries() const = 0;
            virtual size_t GetNumberOfSections() const = 0;

            virtual size_t GetNumberOfEntriesRecursive() const = 0;
            virtual size_t GetNumberOfSectionsRecursive() const = 0;

            virtual IConfigEntry *AddEntry(const char *parent, const char *name, bool value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, uint32_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, int32_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, uint64_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, int64_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, float value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, double value) = 0;
            virtual IConfigEntry *AddEntry(const char *parent, const char *name, const char *value) = 0;
            virtual IConfigSection *CreateSection(const char *parent, const char *name) = 0;

            virtual bool RemoveEntry(const char *parent, const char *name) = 0;
            virtual bool RemoveSection(const char *parent, const char *name) = 0;

            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, bool value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, uint32_t value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, int32_t value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, uint64_t value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, int64_t value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, float value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, double value) = 0;
            virtual IConfigEntry *AddDefaultEntry(const char *parent, const char *name, const char *value) = 0;
            virtual IConfigSection *CreateDefaultSection(const char *parent, const char *name) = 0;

            virtual IConfigEntry *GetEntry(const char *name) const = 0;
            virtual IConfigSection *GetSection(const char *name) const = 0;

            virtual bool IsEntry(size_t index) = 0;
            virtual bool IsSection(size_t index) = 0;

            virtual IConfigEntry *GetEntry(size_t index) const = 0;
            virtual IConfigSection *GetSection(size_t index) const = 0;

            virtual bool Read(char *buffer, size_t len) = 0;
            virtual char *Write(size_t *len) = 0;

            virtual void Free(void *ptr) const = 0;

        protected:
            virtual ~IConfig() = default;
        };

        enum ConfigCallbackType {
            CFG_CB_MODIFY,
        };

        typedef void (*ConfigCallback)(IConfigSection *section, IConfigEntry *entry, void *arg);

        class IConfigSection {
        public:
            virtual const char *GetName() const = 0;
            virtual IConfigSection *GetParent() const = 0;

            virtual void Clear() = 0;

            virtual size_t GetNumberOfEntries() const = 0;
            virtual size_t GetNumberOfSections() const = 0;

            virtual size_t GetNumberOfEntriesRecursive() const = 0;
            virtual size_t GetNumberOfSectionsRecursive() const = 0;

            virtual IConfigEntry *AddEntry(const char *name, bool value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, uint32_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, int32_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, uint64_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, int64_t value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, float value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, double value) = 0;
            virtual IConfigEntry *AddEntry(const char *name, const char *value) = 0;
            virtual IConfigSection *CreateSection(const char *name) = 0;

            virtual bool RemoveEntry(const char *name) = 0;
            virtual bool RemoveSection(const char *name) = 0;

            virtual IConfigEntry *GetEntry(const char *name) const = 0;
            virtual IConfigSection *GetSection(const char *name) const = 0;

            virtual bool IsEntry(size_t index) = 0;
            virtual bool IsSection(size_t index) = 0;

            virtual IConfigEntry *GetEntry(size_t index) const = 0;
            virtual IConfigSection *GetSection(size_t index) const = 0;

            virtual bool AddCallback(ConfigCallbackType type, ConfigCallback callback, void *arg) = 0;
            virtual void ClearCallbacks(ConfigCallbackType type) = 0;
        };

        enum EntryType {
            CFG_ENTRY_NONE,
            CFG_ENTRY_BOOL,
            CFG_ENTRY_UINT,
            CFG_ENTRY_INT,
            CFG_ENTRY_REAL,
            CFG_ENTRY_STR,
        };

        class IConfigEntry {
        public:
            virtual const char *GetName() const = 0;
            virtual IConfigSection *GetParent() const = 0;
            virtual EntryType GetType() const = 0;

            virtual bool GetBool() = 0;
            virtual uint32_t GetUint32() = 0;
            virtual int32_t GetInt32() = 0;
            virtual uint64_t GetUint64() = 0;
            virtual int64_t GetInt64() = 0;
            virtual float GetFloat() = 0;
            virtual double GetDouble() = 0;
            virtual const char *GetString() const = 0;

            virtual void SetValue(bool value) = 0;
            virtual void SetValue(uint32_t value) = 0;
            virtual void SetValue(int32_t value) = 0;
            virtual void SetValue(uint64_t value) = 0;
            virtual void SetValue(int64_t value) = 0;
            virtual void SetValue(float value) = 0;
            virtual void SetValue(double value) = 0;
            virtual void SetValue(const char *value) = 0;

            virtual void CopyValue(IConfigEntry *entry) = 0;
        };
    }
}

#endif // BALLOON_ICONFIG_H