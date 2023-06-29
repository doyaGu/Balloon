#ifndef BALLOON_MODLOADER_H
#define BALLOON_MODLOADER_H

#include <cstdio>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Balloon/DataBox.h"
#include "ModRegistry.h"
#include "ModCandidate.h"

namespace balloon {
    class ModLoader final {
    public:
        explicit ModLoader(const std::shared_ptr<ModRegistry> &registry);

        ModLoader(const ModLoader &rhs) = delete;
        ModLoader(ModLoader &&rhs) noexcept = delete;

        ~ModLoader();

        ModLoader &operator=(const ModLoader &rhs) = delete;
        ModLoader &operator=(ModLoader &&rhs) noexcept = delete;

        bool IsFrozen() const { return m_Frozen; }
        void Freeze() { if (!m_Frozen) m_Frozen = true; }
        void Revive() { if (m_Frozen) m_Frozen = false; }

        bool Load(const std::vector<std::string> &paths);
        bool Unload(const std::string &id);

        void *SetUserData(void *data, size_t type);
        void *GetUserData(size_t type) const;

    private:
        bool LoadMod(const ModCandidate &mod);
        std::shared_ptr<void> LoadLib(const std::string &path);
        bool ForgetLib(void *dllHandle);

        static void DumpModList(const ModCandidateArray &mods);
        static const ModInfo *LoadModInfo(const std::shared_ptr<void> &dllHandle, const std::string &path);

        bool m_Frozen = false;

        std::shared_ptr<ModRegistry> m_Registry;

        typedef std::unordered_map<std::string, std::shared_ptr<void>> ModToDllHandleMap;
        ModToDllHandleMap m_ModToDllHandleMap;

        typedef std::unordered_map<void *, std::string> DllHandleToModMap;
        DllHandleToModMap m_DllHandleToModMap;

        typedef std::unordered_map<void *, std::weak_ptr<void>> DllHandleMap;
        DllHandleMap m_DllHandleMap;

        DataBox m_UserData;
    };
}

#endif // BALLOON_MODLOADER_H