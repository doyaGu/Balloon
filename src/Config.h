#ifndef BALLOON_CONFIG_H
#define BALLOON_CONFIG_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Balloon/IConfig.h"
#include "Balloon/RefCount.h"
#include "Variant.h"

extern "C" {
struct yyjson_mut_doc;
struct yyjson_val;
struct yyjson_mut_val;
};

namespace balloon {
    class ConfigSection;
    class ConfigEntry;

    class Config final : public IConfig {
    public:
        static Config *Create(const std::string &id);
        static Config *Get(const std::string &id);

        Config(const Config &rhs) = delete;
        Config(Config &&rhs) noexcept = delete;

        ~Config() override;

        Config &operator=(const Config &rhs) = delete;
        Config &operator=(Config &&rhs) noexcept = delete;

        int AddRef() const override;
        int Release() const override;

        const char *GetId() const override { return m_Id.c_str(); }

        void Clear() override;
        void ClearDefault() override;

        size_t GetNumberOfEntries() const override;
        size_t GetNumberOfSections() const override;

        size_t GetNumberOfEntriesRecursive() const override;
        size_t GetNumberOfSectionsRecursive() const override;

        template<typename T>
        IConfigEntry *AddEntryT(const char *parent, const char *name, T value) {
            if (parent == nullptr)
                return m_Root->AddEntry(name, value);
            IConfigSection *section = GetSection(parent);
            if (section || (section = CreateSection(m_Root, parent)))
                return section->AddEntry(name, value);
            return nullptr;
        }

        IConfigEntry *AddEntry(const char *parent, const char *name, bool value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, uint32_t value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, int32_t value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, uint64_t value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, int64_t value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, float value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, double value) override;
        IConfigEntry *AddEntry(const char *parent, const char *name, const char *value) override;
        IConfigSection *CreateSection(const char *parent, const char *name) override;

        bool RemoveEntry(const char *parent, const char *name) override;
        bool RemoveSection(const char *parent, const char *name) override;

        template<typename T>
        IConfigEntry *AddDefaultEntryT(const char *parent, const char *name, T value) {
            if (!m_DefaultRoot)
                m_DefaultRoot = new ConfigSection("default", nullptr);
            if (parent == nullptr)
                return m_DefaultRoot->AddEntry(name, value);
            IConfigSection *section = GetSection(parent);
            if (section || (section = CreateSection(m_DefaultRoot, parent)))
                return section->AddEntry(name, value);
            return nullptr;
        }

        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, bool value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, uint32_t value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, int32_t value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, uint64_t value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, int64_t value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, float value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, double value) override;
        IConfigEntry *AddDefaultEntry(const char *parent, const char *name, const char *value) override;
        IConfigSection *CreateDefaultSection(const char *parent, const char *name) override;

        IConfigEntry *GetEntry(const char *name) const override;
        IConfigSection *GetSection(const char *name) const override;

        bool IsEntry(size_t index) override;
        bool IsSection(size_t index) override;

        IConfigEntry *GetEntry(size_t index) const override;
        IConfigSection *GetSection(size_t index) const override;

        bool Read(char *buffer, size_t len) override;
        char *Write(size_t *len) override;

        void Free(void *ptr) const override;

    private:
        explicit Config(std::string id);

        ConfigSection *CreateSection(ConfigSection *root, const char *name) const;
        ConfigSection *GetSection(ConfigSection *root, const char *name) const;

        void ConvertObjectToSection(yyjson_val *obj, ConfigSection *section);
        void ConvertArrayToSection(yyjson_val *arr, ConfigSection *section);

        mutable RefCount m_RefCount;
        std::string m_Id;
        ConfigSection *m_Root;
        ConfigSection *m_DefaultRoot;

        static std::unordered_map<std::string, Config *> s_Configs;
    };

    class ConfigSection final : public IConfigSection {
    public:
        ConfigSection(const char *name, ConfigSection *parent);

        ConfigSection(const ConfigSection &rhs) = delete;
        ConfigSection(ConfigSection &&rhs) noexcept = delete;

        ~ConfigSection();

        ConfigSection &operator=(const ConfigSection &rhs) = delete;
        ConfigSection &operator=(ConfigSection &&rhs) noexcept = delete;

        const char *GetName() const override { return m_Name.c_str(); }
        IConfigSection *GetParent() const override { return m_Parent; }

        void Clear() override;

        size_t GetNumberOfEntries() const override;
        size_t GetNumberOfSections() const override;

        size_t GetNumberOfEntriesRecursive() const override;
        size_t GetNumberOfSectionsRecursive() const override;

        template<typename T>
        IConfigEntry *AddEntryT(const char *name, T value) {
            if (!name)
                return nullptr;
            auto *entry = new ConfigEntry(this, name, value);
            m_Items.emplace_back(0, entry);
            m_Entries[entry->GetName()] = entry;
            return entry;
        }

        IConfigEntry *AddEntry(const char *name, bool value) override;
        IConfigEntry *AddEntry(const char *name, uint32_t value) override;
        IConfigEntry *AddEntry(const char *name, int32_t value) override;
        IConfigEntry *AddEntry(const char *name, uint64_t value) override;
        IConfigEntry *AddEntry(const char *name, int64_t value) override;
        IConfigEntry *AddEntry(const char *name, float value) override;
        IConfigEntry *AddEntry(const char *name, double value) override;
        IConfigEntry *AddEntry(const char *name, const char *value) override;
        IConfigSection *CreateSection(const char *name) override;

        bool RemoveEntry(const char *name) override;
        bool RemoveSection(const char *name) override;

        IConfigEntry *GetEntry(const char *name) const override;
        IConfigSection *GetSection(const char *name) const override;

        bool IsEntry(size_t index) override;
        bool IsSection(size_t index) override;

        IConfigEntry *GetEntry(size_t index) const override;
        IConfigSection *GetSection(size_t index) const override;

        yyjson_mut_val *ToJsonKey(yyjson_mut_doc *doc);
        yyjson_mut_val *ToJsonObject(yyjson_mut_doc *doc);

        bool AddCallback(ConfigCallbackType type, ConfigCallback callback, void *arg) override;
        void ClearCallbacks(ConfigCallbackType type) override;
        void InvokeCallbacks(ConfigCallbackType type, IConfigEntry *entry);

    private:
        struct Callback {
            ConfigCallback callback;
            void *arg;

            Callback(ConfigCallback cb, void *data) : callback(cb), arg(data) {}

            bool operator==(const Callback &rhs) const {
                return callback == rhs.callback &&
                       arg == rhs.arg;
            }

            bool operator!=(const Callback &rhs) const {
                return !(rhs == *this);
            }
        };

        ConfigSection *m_Parent;
        std::string m_Name;
        std::vector<std::tuple<uint8_t, void *>> m_Items;
        std::unordered_map<std::string, ConfigSection *> m_Sections;
        std::unordered_map<std::string, ConfigEntry *> m_Entries;
        std::vector<Callback> m_Callbacks[1];
    };

    class ConfigEntry final : public IConfigEntry {
    public:
        ConfigEntry(ConfigSection *parent, const char *name, bool value);
        ConfigEntry(ConfigSection *parent, const char *name, uint32_t value);
        ConfigEntry(ConfigSection *parent, const char *name, int32_t value);
        ConfigEntry(ConfigSection *parent, const char *name, uint64_t value);
        ConfigEntry(ConfigSection *parent, const char *name, int64_t value);
        ConfigEntry(ConfigSection *parent, const char *name, float value);
        ConfigEntry(ConfigSection *parent, const char *name, double value);
        ConfigEntry(ConfigSection *parent, const char *name, const char *value);

        ConfigEntry(const ConfigEntry &rhs) = delete;
        ConfigEntry(ConfigEntry &&rhs) noexcept = delete;

        ~ConfigEntry();

        ConfigEntry &operator=(const ConfigEntry &rhs) = delete;
        ConfigEntry &operator=(ConfigEntry &&rhs) noexcept = delete;

        const char *GetName() const override { return m_Name.c_str(); }
        IConfigSection *GetParent() const override { return m_Parent; }
        EntryType GetType() const override;

        bool GetBool() override { return m_Value.GetBool(); }
        uint32_t GetUint32() override { return static_cast<uint32_t>(m_Value.GetUint64()); }
        int32_t GetInt32() override { return static_cast<int32_t>(m_Value.GetInt64()); }
        uint64_t GetUint64() override { return m_Value.GetUint64(); }
        int64_t GetInt64() override { return m_Value.GetInt64(); }
        float GetFloat() override { return static_cast<float>(m_Value.GetFloat64()); }
        double GetDouble() override { return m_Value.GetFloat64(); }
        const char *GetString() const override {return m_Value.GetString(); }

        void SetValue(bool value) override {
            m_Value = value;
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(uint32_t value) override {
            m_Value = static_cast<uint64_t>(value);
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(int32_t value) override {
            m_Value = static_cast<int64_t>(value);
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(uint64_t value) override {
            m_Value = value;
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(int64_t value) override {
            m_Value = value;
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(float value) override {
            m_Value = static_cast<double>(value);
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(double value) override {
            m_Value = value;
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }
        void SetValue(const char *value) override {
            m_Value = value;
            m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
        }

        void CopyValue(IConfigEntry *entry) override;

        yyjson_mut_val *ToJsonKey(yyjson_mut_doc *doc);
        yyjson_mut_val *ToJsonValue(yyjson_mut_doc *doc);

    private:
        ConfigSection *m_Parent;
        std::string m_Name;
        Variant m_Value;
    };
}
#endif // BALLOON_CONFIG_H