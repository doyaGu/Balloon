#ifndef BALLOON_MODEXPLORER_H
#define BALLOON_MODEXPLORER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "ModLoader.h"
#include "ModCandidate.h"
#include "ModCandidateFinder.h"
#include "ModCandidateScanner.h"

namespace balloon {
    class ModExplorer final {
    public:
        ModExplorer();

        ModExplorer(const ModExplorer &rhs) = delete;
        ModExplorer(ModExplorer &&rhs) noexcept = delete;

        ~ModExplorer();
        
        ModExplorer &operator=(const ModExplorer &rhs) = delete;
        ModExplorer &operator=(ModExplorer &&rhs) noexcept = delete;

        ModCandidateSet ExploreMods(ModLoader *loader, ModCandidateSetMap& envDisabledModsOut);

        void AddCandidateFinder(const std::shared_ptr<ModCandidateFinder> &finder);

    private:
        std::vector<std::shared_ptr<ModCandidateFinder>> m_Finders;
        std::unique_ptr<ModCandidateScanner> m_Scanner;
    };
}

#endif // BALLOON_MODEXPLORER_H
