#include "Config.h"

#include <cassert>
#include <utility>

#include <yyjson.h>

#include "Logger.h"
#include "StringUtils.h"

using namespace balloon;

std::unordered_map<std::string, Config *> Config::s_Configs;

Config *Config::Create(const std::string &id) {
    return new Config(id);
}

Config *Config::Get(const std::string &id) {
    auto it = s_Configs.find(id);
    if (it == s_Configs.end())
        return Create(id);

    auto *config = it->second;
    config->AddRef();
    return config;
}

Config::~Config() {
    Clear();
    ClearDefault();
    s_Configs.erase(m_Id);
}

int Config::AddRef() const {
    return m_RefCount.AddRef();
}

int Config::Release() const {
    int r = m_RefCount.Release();
    if (r == 0) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete const_cast<Config *>(this);
    }
    return r;
}

void Config::Clear() {
    if (m_Root)
        m_Root->Clear();
}

void Config::ClearDefault() {
    if (m_DefaultRoot)
        m_DefaultRoot->Clear();
}

size_t Config::GetNumberOfEntries() const {
    if (!m_Root)
        return 0;
    return m_Root->GetNumberOfEntries();
}

size_t Config::GetNumberOfSections() const {
    if (!m_Root)
        return 0;
    return m_Root->GetNumberOfSections();
}

size_t Config::GetNumberOfEntriesRecursive() const {
    if (!m_Root)
        return 0;
    return m_Root->GetNumberOfEntriesRecursive();
}

size_t Config::GetNumberOfSectionsRecursive() const {
    if (!m_Root)
        return 0;
    return m_Root->GetNumberOfSectionsRecursive();
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, bool value) {
    return AddEntryT(parent, name, value);
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, uint32_t value) {
    return AddEntryT(parent, name, static_cast<uint64_t>(value));
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, int32_t value) {
    return AddEntryT(parent, name, static_cast<int64_t>(value));
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, uint64_t value) {
    return AddEntryT(parent, name, value);
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, int64_t value) {
    return AddEntryT(parent, name, value);
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, float value) {
    return AddEntryT(parent, name, static_cast<double>(value));
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, double value) {
    return AddEntryT(parent, name, value);
}

IConfigEntry *Config::AddEntry(const char *parent, const char *name, const char *value) {
    return AddEntryT(parent, name, value);
}

IConfigSection *Config::CreateSection(const char *parent, const char *name) {
    if (!parent)
        return m_Root->CreateSection(name);
    auto *section = (ConfigSection *) m_Root->GetSection(parent);
    if (section)
        return section->CreateSection(name);
    return nullptr;
}

bool Config::RemoveEntry(const char *parent, const char *name) {
    if (!parent)
        return m_Root->RemoveEntry(name);
    IConfigSection *section = m_Root->GetSection(parent);
    if (section)
        return section->RemoveEntry(name);
    return false;
}

bool Config::RemoveSection(const char *parent, const char *name) {
    if (!parent)
        return m_Root->RemoveSection(name);
    IConfigSection *section = m_Root->GetSection(parent);
    if (section)
        return section->RemoveSection(name);
    return false;
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, bool value) {
    return AddDefaultEntryT(parent, name, value);
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, uint32_t value) {
    return AddDefaultEntryT(parent, name, static_cast<uint64_t>(value));
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, int32_t value) {
    return AddDefaultEntryT(parent, name, static_cast<int64_t>(value));
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, uint64_t value) {
    return AddDefaultEntryT(parent, name, value);
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, int64_t value) {
    return AddDefaultEntryT(parent, name, value);
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, float value) {
    return AddDefaultEntryT(parent, name, static_cast<double>(value));
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, double value) {
    return AddDefaultEntryT(parent, name, value);
}

IConfigEntry *Config::AddDefaultEntry(const char *parent, const char *name, const char *value) {
    return AddDefaultEntryT(parent, name, value);
}

IConfigSection *Config::CreateDefaultSection(const char *parent, const char *name) {
    if (!parent) {
        if (!m_DefaultRoot)
            m_DefaultRoot = new ConfigSection("default", nullptr);
        return m_DefaultRoot->CreateSection(name);
    }
    if (m_DefaultRoot) {
        IConfigSection *section = m_DefaultRoot->GetSection(parent);
        if (section)
            return section->CreateSection(name);
    }
    return nullptr;
}

IConfigEntry *Config::GetEntry(const char *name) const {
    IConfigEntry *entry = m_Root->GetEntry(name);
    if (!entry && m_DefaultRoot)
        return m_DefaultRoot->GetEntry(name);
    return entry;
}

IConfigSection *Config::GetSection(const char *name) const {
    IConfigSection *section = GetSection(m_Root, name);
    if (!section && m_DefaultRoot)
        return GetSection(m_DefaultRoot, name);
    return section;
}

bool Config::IsEntry(size_t index) {
    return m_Root->IsEntry(index);
}

bool Config::IsSection(size_t index) {
    return m_Root->IsSection(index);
}

IConfigEntry *Config::GetEntry(size_t index) const {
    return m_Root->GetEntry(index);
}

IConfigSection *Config::GetSection(size_t index) const {
    return m_Root->GetSection(index);
}

bool Config::Read(char *buffer, size_t len) {
    yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_INF_AND_NAN;
    yyjson_read_err err;
    yyjson_doc *doc = yyjson_read_opts(buffer, len, flg, nullptr, &err);
    if (!doc) {
        LOG_ERROR("Config read error: %s code: %u at position: %zu\n", err.msg, err.code, err.pos);
        return false;
    }

    Clear();

    yyjson_val *obj = yyjson_doc_get_root(doc);
    ConvertObjectToSection(obj, m_Root);
    yyjson_doc_free(doc);

    return true;
}

char *Config::Write(size_t *len) {
    yyjson_mut_doc *doc = yyjson_mut_doc_new(nullptr);
    if (!doc) {
        *len = 0;
        return nullptr;
    }

    yyjson_mut_val *root = nullptr;
    if (m_Root->GetNumberOfEntries() != 0 || m_Root->GetNumberOfSections() != 0) {
        root = m_Root->ToJsonObject(doc);
    } else if (m_DefaultRoot) {
        root = m_DefaultRoot->ToJsonObject(doc);
    }

    if (!root || yyjson_mut_obj_size(root) == 0) {
        *len = 0;
        return nullptr;
    }
    yyjson_mut_doc_set_root(doc, root);

    yyjson_write_flag flg = YYJSON_WRITE_PRETTY | YYJSON_WRITE_ESCAPE_UNICODE;
    yyjson_write_err err;
    char *json = yyjson_mut_write_opts(doc, flg, nullptr, len, &err);
    if (!json) {
        LOG_ERROR("Config write error: %s code: %u\n", err.msg, err.code);
    }
    return json;
}

void Config::Free(void *ptr) const {
    free(ptr);
}

Config::Config(std::string id)
    : m_Id(std::move(id)),
      m_Root(new ConfigSection("root", nullptr)),
      m_DefaultRoot(nullptr) {
    AddRef();
    s_Configs[m_Id] = this;
}

ConfigSection *Config::CreateSection(ConfigSection *root, const char *name) const {
    return (ConfigSection *) root->CreateSection(name);
}

ConfigSection *Config::GetSection(ConfigSection *root, const char *name) const {
    return (ConfigSection *) root->GetSection(name);
}

void Config::ConvertObjectToSection(yyjson_val *obj, ConfigSection *section) {
    if (!yyjson_is_obj(obj))
        return;

    yyjson_val *key, *val;
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(obj, &iter);
    while ((key = yyjson_obj_iter_next(&iter))) {
        val = yyjson_obj_iter_get_val(key);
        switch (yyjson_get_tag(val)) {
            case YYJSON_TYPE_OBJ | YYJSON_SUBTYPE_NONE:
                ConvertObjectToSection(val, (ConfigSection *) section->CreateSection(yyjson_get_str(key)));
                break;
            case YYJSON_TYPE_BOOL | YYJSON_SUBTYPE_TRUE:
            case YYJSON_TYPE_BOOL | YYJSON_SUBTYPE_FALSE:
                section->AddEntry(yyjson_get_str(key), yyjson_get_bool(val));
                break;
            case YYJSON_TYPE_NUM | YYJSON_SUBTYPE_UINT:
                section->AddEntry(yyjson_get_str(key), yyjson_get_uint(val));
                break;
            case YYJSON_TYPE_NUM | YYJSON_SUBTYPE_SINT:
                section->AddEntry(yyjson_get_str(key), yyjson_get_sint(val));
                break;
            case YYJSON_TYPE_NUM | YYJSON_SUBTYPE_REAL:
                section->AddEntry(yyjson_get_str(key), yyjson_get_real(val));
                break;
            case YYJSON_TYPE_STR | YYJSON_SUBTYPE_NONE:
                section->AddEntry(yyjson_get_str(key), yyjson_get_str(val));
                break;
            case YYJSON_TYPE_NULL | YYJSON_SUBTYPE_NONE:
                LOG_TRACE("Config read: null will be ignored.");
                break;
            case YYJSON_TYPE_ARR | YYJSON_SUBTYPE_NONE:
                ConvertArrayToSection(val, (ConfigSection *) section->CreateSection(yyjson_get_str(key)));
                break;
            default:
                break;
        }
    }
}

void Config::ConvertArrayToSection(yyjson_val *arr, ConfigSection *section) {
    if (!yyjson_is_arr(arr))
        return;

    char buf[32];
    size_t idx = 0;
    yyjson_val *val;
    yyjson_arr_iter iter = yyjson_arr_iter_with(arr);
    while ((val = yyjson_arr_iter_next(&iter))) {
        utils::ToString(buf, idx);
        switch (yyjson_get_tag(val)) {
            case YYJSON_TYPE_OBJ | YYJSON_SUBTYPE_NONE:
                ConvertObjectToSection(val, (ConfigSection *) section->CreateSection(buf));
                break;
            case YYJSON_TYPE_BOOL | YYJSON_SUBTYPE_TRUE:
            case YYJSON_TYPE_BOOL | YYJSON_SUBTYPE_FALSE:
                section->AddEntry(buf, yyjson_get_bool(val));
                break;
            case YYJSON_TYPE_NUM | YYJSON_SUBTYPE_UINT:
                section->AddEntry(buf, yyjson_get_uint(val));
                break;
            case YYJSON_TYPE_NUM | YYJSON_SUBTYPE_SINT:
                section->AddEntry(buf, yyjson_get_sint(val));
                break;
            case YYJSON_TYPE_NUM | YYJSON_SUBTYPE_REAL:
                section->AddEntry(buf, yyjson_get_real(val));
                break;
            case YYJSON_TYPE_STR | YYJSON_SUBTYPE_NONE:
                section->AddEntry(buf, yyjson_get_str(val));
                break;
            case YYJSON_TYPE_NULL | YYJSON_SUBTYPE_NONE:
                LOG_TRACE("Config read: null will be ignored.");
                break;
            case YYJSON_TYPE_ARR | YYJSON_SUBTYPE_NONE:
                ConvertArrayToSection(val, (ConfigSection *) section->CreateSection(buf));
                break;
            default:
                break;
        }
        ++idx;
    }
}

ConfigSection::ConfigSection(const char *name, ConfigSection *parent)
    : m_Parent(parent), m_Name(name) {
    assert(name != nullptr);
}

ConfigSection::~ConfigSection() {
    Clear();
}

void ConfigSection::Clear() {
    for (auto &pair: m_Entries) {
        auto *entry = pair.second;
        delete entry;
    }
    m_Entries.clear();

    for (auto &pair: m_Sections) {
        auto *section = pair.second;
        delete section;
    }
    m_Sections.clear();
}

size_t ConfigSection::GetNumberOfEntries() const {
    return m_Entries.size();
}

size_t ConfigSection::GetNumberOfSections() const {
    return m_Sections.size();
}

size_t ConfigSection::GetNumberOfEntriesRecursive() const {
    size_t count = 0;
    for (auto &pair: m_Sections) {
        auto *section = pair.second;
        count += section->GetNumberOfEntriesRecursive();
    }
    return count + GetNumberOfEntries();
}

size_t ConfigSection::GetNumberOfSectionsRecursive() const {
    size_t count = 0;
    for (auto &pair: m_Sections) {
        auto *section = pair.second;
        count += section->GetNumberOfSectionsRecursive();
    }
    return count + GetNumberOfSections();
}

IConfigEntry *ConfigSection::AddEntry(const char *name, bool value) {
    return AddEntryT(name, value);
}

IConfigEntry *ConfigSection::AddEntry(const char *name, uint32_t value) {
    return AddEntryT(name, static_cast<uint64_t>(value));
}

IConfigEntry *ConfigSection::AddEntry(const char *name, int32_t value) {
    return AddEntryT(name, static_cast<int64_t>(value));
}

IConfigEntry *ConfigSection::AddEntry(const char *name, uint64_t value) {
    return AddEntryT(name, value);
}

IConfigEntry *ConfigSection::AddEntry(const char *name, int64_t value) {
    return AddEntryT(name, value);
}

IConfigEntry *ConfigSection::AddEntry(const char *name, float value) {
    return AddEntryT(name, static_cast<double>(value));
}

IConfigEntry *ConfigSection::AddEntry(const char *name, double value) {
    return AddEntryT(name, value);
}

IConfigEntry *ConfigSection::AddEntry(const char *name, const char *value) {
    return AddEntryT(name, value);
}

IConfigSection *ConfigSection::CreateSection(const char *name) {
    if (!name) return nullptr;
    auto *section = new ConfigSection(name, this);
    m_Items.emplace_back(1, section);
    m_Sections[section->GetName()] = section;
    return section;
}

bool ConfigSection::RemoveEntry(const char *name) {
    if (!name)
        return false;

    auto it = m_Entries.find(name);
    if (it == m_Entries.end())
        return false;

    auto *entry = it->second;
    auto del = std::remove_if(m_Items.begin(), m_Items.end(),
                              [entry](const std::tuple<int, void *> &e) { return std::get<1>(e) == entry; });
    m_Items.erase(del, m_Items.end());
    m_Entries.erase(it);
    delete entry;
    return true;
}

bool ConfigSection::RemoveSection(const char *name) {
    if (!name)
        return false;

    auto it = m_Sections.find(name);
    if (it == m_Sections.end())
        return false;

    auto *section = it->second;
    auto del = std::remove_if(m_Items.begin(), m_Items.end(),
                              [section](const std::tuple<int, void *> &e) { return std::get<1>(e) == section; });
    m_Items.erase(del, m_Items.end());
    m_Sections.erase(it);
    delete section;
    return true;
}

IConfigEntry *ConfigSection::GetEntry(const char *name) const {
    auto it = m_Entries.find(name);
    if (it == m_Entries.end())
        return nullptr;
    return it->second;
}

IConfigSection *ConfigSection::GetSection(const char *name) const {
    auto it = m_Sections.find(name);
    if (it == m_Sections.end())
        return nullptr;
    return it->second;
}

bool ConfigSection::IsEntry(size_t index) {
    if (index >= m_Items.size())
        return false;
    return std::get<0>(m_Items[index]) == 0;
}

bool ConfigSection::IsSection(size_t index) {
    if (index >= m_Items.size())
        return false;
    return std::get<0>(m_Items[index]) == 1;
}

IConfigEntry *ConfigSection::GetEntry(size_t index) const {
    if (index >= m_Items.size())
        return nullptr;

    auto item = m_Items[index];
    if (std::get<0>(item) == 0)
        return static_cast<IConfigEntry *>(std::get<1>(item));
    else
        return nullptr;
}

IConfigSection *ConfigSection::GetSection(size_t index) const {
    if (index >= m_Items.size())
        return nullptr;

    auto item = m_Items[index];
    if (std::get<0>(item) == 1)
        return static_cast<IConfigSection *>(std::get<1>(item));
    else
        return nullptr;
}

yyjson_mut_val *ConfigSection::ToJsonKey(yyjson_mut_doc *doc) {
    if (!doc)
        return nullptr;
    return yyjson_mut_str(doc, m_Name.c_str());
}

yyjson_mut_val *ConfigSection::ToJsonObject(yyjson_mut_doc *doc) {
    if (!doc)
        return nullptr;

    yyjson_mut_val *obj = yyjson_mut_obj(doc);
    if (!obj)
        return nullptr;

    for (auto &item: m_Items) {
        switch (std::get<0>(item)) {
            case 0: {
                auto *entry = static_cast<ConfigEntry *>(std::get<1>(item));
                if (entry) {
                    yyjson_mut_val *key = entry->ToJsonKey(doc);
                    yyjson_mut_val *val = entry->ToJsonValue(doc);
                    yyjson_mut_obj_add(obj, key, val);
                }
            }
                break;
            case 1: {
                auto *section = static_cast<ConfigSection *>(std::get<1>(item));
                if (section) {
                    yyjson_mut_val *key = section->ToJsonKey(doc);
                    yyjson_mut_val *val = section->ToJsonObject(doc);
                    yyjson_mut_obj_add(obj, key, val);
                }
            }
                break;
            default:
                break;
        }
    }

    return obj;
}

bool ConfigSection::AddCallback(ConfigCallbackType type, ConfigCallback callback, void *arg) {
    if (type < 0 || type > CFG_CB_MODIFY)
        return false;

    if (!callback)
        return false;

    auto &callbacks = m_Callbacks[type];
    Callback cb(callback, arg);
    auto it = std::find(callbacks.begin(), callbacks.end(), cb);
    if (it != callbacks.end())
        return false;

    callbacks.emplace_back(cb);
    return true;
}

void ConfigSection::ClearCallbacks(ConfigCallbackType type) {
    if (type >= 0 && type <= CFG_CB_MODIFY)
        m_Callbacks[type].clear();
}

void ConfigSection::InvokeCallbacks(ConfigCallbackType type, IConfigEntry *entry) {
    assert(type >= 0 && type <= CFG_CB_MODIFY);
    assert(entry != nullptr);
    for (auto &cb: m_Callbacks[type]) {
        cb.callback(this, entry, cb.arg);
    }
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, bool value)
    : m_Parent(parent), m_Name(name), m_Value(value) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, uint32_t value)
    : m_Parent(parent), m_Name(name), m_Value(static_cast<uint64_t>(value)) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, int32_t value)
    : m_Parent(parent), m_Name(name), m_Value(static_cast<int64_t>(value)) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, uint64_t value)
    : m_Parent(parent), m_Name(name), m_Value(value) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, int64_t value)
    : m_Parent(parent), m_Name(name), m_Value(value) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, float value)
    : m_Parent(parent), m_Name(name), m_Value(static_cast<double>(value)) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, double value)
    : m_Parent(parent), m_Name(name), m_Value(value) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::ConfigEntry(ConfigSection *parent, const char *name, const char *value)
    : m_Parent(parent), m_Name(name), m_Value(value) {
    assert(parent != nullptr);
    assert(name != nullptr);
}

ConfigEntry::~ConfigEntry() = default;

EntryType ConfigEntry::GetType() const {
    switch (m_Value.GetType()) {
        case VAR_TYPE_BOOL:
            return CFG_ENTRY_BOOL;
        case VAR_TYPE_NUM:
            switch (m_Value.GetSubtype()) {
                case VAR_SUBTYPE_UINT64:
                    return CFG_ENTRY_UINT;
                case VAR_SUBTYPE_INT64:
                    return CFG_ENTRY_INT;
                case VAR_SUBTYPE_FLOAT64:
                    return CFG_ENTRY_REAL;
                default:
                    break;
            }
        case VAR_TYPE_STR:
            return CFG_ENTRY_STR;
        default:
            break;
    }
    return CFG_ENTRY_NONE;
}

void ConfigEntry::CopyValue(IConfigEntry *entry) {
    switch (entry->GetType()) {
        case CFG_ENTRY_BOOL:
            SetValue(entry->GetBool());
            break;
        case CFG_ENTRY_UINT:
            SetValue(entry->GetUint64());
            break;
        case CFG_ENTRY_INT:
            SetValue(entry->GetInt64());
            break;
        case CFG_ENTRY_REAL:
            SetValue(entry->GetDouble());
            break;
        case CFG_ENTRY_STR:
            SetValue(entry->GetString());
            break;
        default:
            break;
    }
    m_Parent->InvokeCallbacks(CFG_CB_MODIFY, this);
}

yyjson_mut_val *ConfigEntry::ToJsonKey(yyjson_mut_doc *doc) {
    if (!doc)
        return nullptr;
    return yyjson_mut_str(doc, m_Name.c_str());
}

yyjson_mut_val *ConfigEntry::ToJsonValue(yyjson_mut_doc *doc) {
    if (!doc)
        return nullptr;

    switch (GetType()) {
        case CFG_ENTRY_BOOL:
            return yyjson_mut_bool(doc, GetBool());
        case CFG_ENTRY_UINT:
            return yyjson_mut_uint(doc, GetUint64());
        case CFG_ENTRY_INT:
            return yyjson_mut_int(doc, GetInt64());
        case CFG_ENTRY_REAL:
            return yyjson_mut_real(doc, GetDouble());
        case CFG_ENTRY_STR:
            return yyjson_mut_str(doc, GetString());
        default:
            return nullptr;
    }
}