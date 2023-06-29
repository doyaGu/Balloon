#include "ModRegistry.h"

#include <cassert>

using namespace balloon;

ModRegistry::ModRegistry() = default;

ModRegistry::~ModRegistry() = default;

bool ModRegistry::AddMod(const std::string &rootPath, const std::string &sourcePath, const std::shared_ptr<ModMetadata> &metadata,
                         const std::shared_ptr<void> &library, const ModInfo *info) {
    if (rootPath.empty() || sourcePath.empty() || !metadata || !library || !info)
        return false;

    if (strcmp(metadata->GetId(), info->id) != 0)
        return false;

    auto mod = ModContainer::Create(rootPath, sourcePath, metadata, library, info);
    if (!mod)
        return false;

    if (TriggerCallbacks(MR_CB_PREADD, mod) == 0) {
        return false;
    }

    m_Mods.push_back(mod);
    m_ModMap.emplace(info->id, mod);

    if (TriggerCallbacks(MR_CB_POSTADD, mod) == 0) {
        m_Mods.pop_back();
        m_ModMap.erase(info->id);
        return false;
    }

    return true;
}

bool ModRegistry::RemoveMod(const std::string &id) {
    auto it = m_ModMap.find(id);
    if (it == m_ModMap.end())
        return false;

    auto &mod = it->second;

    if (TriggerCallbacks(MR_CB_PREREMOVE, mod) == 0) {
        return false;
    }

    m_Mods.erase(std::remove(m_Mods.begin(), m_Mods.end(), mod), m_Mods.end());
    m_ModMap.erase(it);

    TriggerCallbacks(MR_CB_POSTREMOVE, mod);
    return true;
}

bool ModRegistry::HasMod(const std::string &id) const {
    auto it = m_ModMap.find(id);
    return (it != m_ModMap.end());
}

size_t ModRegistry::GetModCount() const {
    return m_Mods.size();
}

std::shared_ptr<ModContainer> ModRegistry::GetMod(size_t index) const {
    if (index >= m_Mods.size())
        return nullptr;
    return m_Mods[index];
}

std::shared_ptr<ModContainer> ModRegistry::GetMod(const std::string &id) const {
    const auto it = m_ModMap.find(id);
    if (m_ModMap.end() == it)
        return nullptr;
    return it->second;
}

std::vector<std::string> ModRegistry::GetAllModIds() const {
    std::vector<std::string> result;
    result.reserve(m_Mods.size());
    for (const auto &mod: m_Mods)
        result.emplace_back(mod->GetId());
    return result;
}

bool ModRegistry::IterateMods(const ModCallback &func, bool reversed) {
    bool ret = true;

    if (!reversed) {
        auto it = m_Mods.begin();
        while (it != m_Mods.end()) {
            int r = func(*it);
            if (r == 1) {
                ++it;
            } else if (r == 0) {
                it = m_Mods.erase(it);
                m_ModMap.erase((*it)->GetId());
            } else if (r == -1) {
                ret = false;
                break;
            }
        }
    } else {
        auto rit = m_Mods.rbegin();
        while (rit != m_Mods.rend()) {
            int r = func(*rit);
            if (r == 1) {
                ++rit;
            } else if (r == 0) {
                rit = decltype(m_Mods)::reverse_iterator(m_Mods.erase((++rit).base()));
                m_ModMap.erase((*rit)->GetId());
            } else if (r == -1) {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

void ModRegistry::AddCallback(ModCallbackType type, ModCallback callback) {
    assert(type >= 0 && type < MR_CB_COUNT);
    m_Callbacks[type].emplace_back(std::move(callback));
}

void ModRegistry::ClearCallbacks(ModCallbackType type) {
    assert(type >= 0 && type < MR_CB_COUNT);
    m_Callbacks[type].clear();
}

int ModRegistry::TriggerCallbacks(ModCallbackType type, const std::shared_ptr<ModContainer> &mod) {
    int ret;
    for (auto &cb: m_Callbacks[type]) {
        if ((ret = cb(mod)) != 0)
            return ret;
    }
    return 1;
}
