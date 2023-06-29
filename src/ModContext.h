#ifndef BALLOON_MODCONTEXT_H
#define BALLOON_MODCONTEXT_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Balloon/IModContext.h"
#include "Balloon/IMod.h"
#include "Balloon/ModInfo.h"
#include "Balloon/DataBox.h"
#include "ModRegistry.h"
#include "InterfaceInfo.h"

namespace balloon {
    class ModContext final : public IModContext {
    public:
        explicit ModContext(const std::shared_ptr<ModRegistry> &registry);

        ModContext(const ModContext &rhs) = delete;
        ModContext(ModContext &&rhs) noexcept = delete;

        ~ModContext();

        ModContext &operator=(const ModContext &rhs) = delete;
        ModContext &operator=(ModContext &&rhs) noexcept = delete;

        size_t GetModCount() const override;
        IModContainer *GetMod(size_t index) const override;
        IModContainer *GetMod(const char *id) const override;
        std::shared_ptr<ModContainer> GetMod(const std::string &id) const;

        void SetCurrentMod(const std::shared_ptr<ModContainer> &mod);

        bool RegisterInterface(void *interface, const char *name, const char *owner, int version) override;
        bool RegisterInterface(void *interface, const char *name, int version);
        bool HasInterface(const char *id) const override;
        void *GetInterface(const char *name, const char *owner, int version) const override;

        bool RegisterFactory(IObjectFactory *factory, const char *name, const char *owner, int version) override;
        bool RegisterFactory(IObjectFactory *factory, const char *name, int version);
        bool HasFactory(const char *id) const override;
        IObjectFactory *GetFactory(const char *name, const char *owner, int version) const override;

        ILogger *GetLogger(const char *id) const override;
        IConfig *GetConfig(const char *id) const override;

        // User data
        void *GetUserData(size_t type) const override;
        void *SetUserData(void *data, size_t type) override;

    private:
        std::weak_ptr<ModContainer> m_CurrentMod;
        std::shared_ptr<ModRegistry> m_Registry;
        std::unordered_set<std::string> m_InterfaceProviders;
        std::unordered_set<InterfaceInfo> m_Interfaces;
        std::unordered_set<std::string> m_FactoryProviders;
        std::unordered_set<InterfaceInfo> m_Factories;
        DataBox m_UserData;
    };
}

#endif // BALLOON_MODCONTEXT_H
