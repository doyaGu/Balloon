#ifndef BALLOON_MODREGISTRY_H
#define BALLOON_MODREGISTRY_H

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Balloon/ModInfo.h"
#include "ModContainer.h"

namespace balloon {
    typedef enum ModCallbackType {
        MR_CB_PREADD = 0,
        MR_CB_POSTADD = 1,
        MR_CB_PREREMOVE = 2,
        MR_CB_POSTREMOVE = 3,
        MR_CB_COUNT
    } ModCallbackType;

    typedef std::function<int(const std::shared_ptr<ModContainer> &mod)> ModCallback;

    class ModRegistry final {
    public:
        ModRegistry();

        ModRegistry(const ModRegistry &rhs) = delete;
        ModRegistry(ModRegistry &&rhs) noexcept = delete;

        ~ModRegistry();

        ModRegistry &operator=(const ModRegistry &rhs) = delete;
        ModRegistry &operator=(ModRegistry &&rhs) noexcept = delete;

        bool AddMod(const std::string &rootPath, const std::string &sourcePath, const std::shared_ptr<ModMetadata>& metadata, const std::shared_ptr<void>& library, const ModInfo *info);
        bool RemoveMod(const std::string &id);
        bool HasMod(const std::string &id) const;

        size_t GetModCount() const;
        std::shared_ptr<ModContainer> GetMod(size_t index) const;
        std::shared_ptr<ModContainer> GetMod(const std::string &id) const;

        std::vector<std::string> GetAllModIds() const;
        bool IterateMods(const ModCallback &func, bool reversed = false);

        void AddCallback(ModCallbackType type, ModCallback callback);
        void ClearCallbacks(ModCallbackType type);

    private:
        int TriggerCallbacks(ModCallbackType type, const std::shared_ptr<ModContainer> &mod);

        std::vector<std::shared_ptr<ModContainer>> m_Mods;
        std::unordered_map<std::string, std::shared_ptr<ModContainer>> m_ModMap;
        std::vector<ModCallback> m_Callbacks[MR_CB_COUNT];
    };
}

#endif // BALLOON_MODREGISTRY_H
