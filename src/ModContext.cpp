#include "ModContext.h"

#include "Logger.h"
#include "Config.h"

#include "ModRegistry.h"
#include "ModContainer.h"

using namespace balloon;

ModContext::ModContext(const std::shared_ptr<ModRegistry> &registry) : m_Registry(registry) {}

ModContext::~ModContext() = default;

size_t ModContext::GetModCount() const {
    return m_Registry->GetModCount();
}

IModContainer *ModContext::GetMod(size_t index) const {
    return m_Registry->GetMod(index).get();
}

IModContainer *ModContext::GetMod(const char *id) const {
    if (!id) {
        auto mod = m_CurrentMod.lock();
        return mod.get();
    }
    return m_Registry->GetMod(id).get();
}

std::shared_ptr<ModContainer> ModContext::GetMod(const std::string &id) const {
    if (id.empty())
        return m_CurrentMod.lock();
    return m_Registry->GetMod(id);
}

void ModContext::SetCurrentMod(const std::shared_ptr<ModContainer> &mod) {
    m_CurrentMod = mod;
}

bool ModContext::RegisterInterface(void *interface, const char *name, const char *owner, int version) {
    if (!interface || !name || strncmp(name, "", 1) == 0)
        return false;

    if (!owner)
        return false;

    std::string id = owner;

    auto mod = GetMod(id);
    if (!mod) {
        return false;
    }

    if (!mod->IsInstantiated() || (mod->GetFlags() & MOD_INITIALIZED) != 0) {
        return false;
    }

    InterfaceInfo key(owner, name, version);

    auto it = m_Interfaces.find(key);
    if (it != m_Interfaces.end())
        return false;

    mod->SetFlags(MOD_FIXED, 0);

    auto pit = m_InterfaceProviders.find(id);
    if (pit == m_InterfaceProviders.end())
        m_InterfaceProviders.emplace(std::move(id));

    m_Interfaces.emplace(owner, name, version, interface);
    return true;
}

bool ModContext::RegisterInterface(void *interface, const char *name, int version) {
    if (!interface || !name || strncmp(name, "", 1) == 0)
        return false;

    InterfaceInfo key("", name, version);
    auto it = m_Interfaces.find(key);
    if (it != m_Interfaces.end())
        return false;

    m_Interfaces.emplace("", name, version, interface);
    return true;
}

void *ModContext::GetInterface(const char *name, const char *owner, int version) const {
    if (!name)
        return nullptr;

    if (!owner)
        owner = "";

    InterfaceInfo key(owner, name, version);

    auto it = m_Interfaces.find(key);
    if (it == m_Interfaces.end())
        return nullptr;

    return it->interface;
}

bool ModContext::HasInterface(const char *id) const {
    if (!id)
        return false;

    auto it = m_InterfaceProviders.find(id);
    return it != m_InterfaceProviders.end();
}

bool ModContext::RegisterFactory(IObjectFactory *factory, const char *name, const char *owner, int version) {
    if (!factory || !name || strncmp(name, "", 1) == 0)
        return false;

    if (!owner)
        return false;

    std::string id = owner;

    auto mod = GetMod(id);
    if (!mod) {
        return false;
    }

    if (!mod->IsInstantiated() || (mod->GetFlags() & MOD_INITIALIZED) != 0) {
        return false;
    }

    InterfaceInfo key(owner, name, version);

    auto it = m_Factories.find(key);
    if (it != m_Factories.end())
        return false;

    mod->SetFlags(MOD_FIXED, 0);

    auto pit = m_FactoryProviders.find(id);
    if (pit == m_FactoryProviders.end())
        m_FactoryProviders.emplace(std::move(id));

    m_Factories.emplace(owner, name, version, factory);
    return true;
}

bool ModContext::RegisterFactory(IObjectFactory *factory, const char *name, int version) {
    if (!factory || !name || strncmp(name, "", 1) == 0)
        return false;

    InterfaceInfo key("", name, version);

    auto it = m_Factories.find(key);
    if (it != m_Factories.end())
        return false;

    m_Factories.emplace("", name, version, factory);
    return true;
}

bool ModContext::HasFactory(const char *id) const {
    auto it = m_FactoryProviders.find(id);
    return it != m_FactoryProviders.end();
}

IObjectFactory *ModContext::GetFactory(const char *name, const char *owner, int version) const {
    if (!name)
        return nullptr;

    if (!owner)
        owner = "";

    InterfaceInfo key(owner, name, version);

    auto it = m_Factories.find(key);
    if (it == m_Factories.end())
        return nullptr;

    return static_cast<IObjectFactory *>(it->interface);
}

ILogger *ModContext::GetLogger(const char *id) const {
    if (!id)
        return Logger::GetDefault();

    std::string mid = id;
    auto mod = GetMod(mid);
    if (!mod)
        return nullptr;

    if ((mod->GetFlags() & MOD_LOGGER_RETRIEVED) == 0)
        mod->SetFlags(MOD_LOGGER_RETRIEVED, 0);

    return Logger::Get(mid);
}

IConfig *ModContext::GetConfig(const char *id) const {
    if (!id)
        return Config::Get("");

    std::string mid = id;
    auto mod = GetMod(mid);
    if (!mod)
        return nullptr;

    if ((mod->GetFlags() & MOD_CONFIG_RETRIEVED) == 0)
        mod->SetFlags(MOD_CONFIG_RETRIEVED, 0);

    return Config::Get(mid);
}

void *ModContext::GetUserData(size_t type) const {
    return m_UserData.GetData(type);
}

void *ModContext::SetUserData(void *data, size_t type) {
    return m_UserData.SetData(data, type);
}