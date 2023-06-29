#ifndef BALLOON_MODRESOLVER_H
#define BALLOON_MODRESOLVER_H

#include <vector>
#include <memory>

#include "ModCandidate.h"
#include "ModSolver.h"

namespace balloon {
    class ModResolver final {
        friend class ModSolver;
    public:
        static ModResolver& GetInstance();

        ModResolver(const ModResolver &rhs) = delete;
        ModResolver(ModResolver &&rhs) noexcept = delete;

        ~ModResolver();

        ModResolver &operator=(const ModResolver &rhs) = delete;
        ModResolver &operator=(ModResolver &&rhs) noexcept = delete;

        ModCandidateArray Resolve(const ModCandidateSet &candidates, const ModCandidateSetMap &disabledMods);

    private:
        ModResolver();

        ModCandidateArray FindCompatibleSet(const ModCandidateSet &candidates, const ModCandidateSetMap &disabledMods);

        bool PreselectMod(const ModCandidate &mod,
                          ModCandidateArray &allModsSorted,
                          ModCandidateSetMap &modsById,
                          ModCandidateMap &selectedMods,
                          ModCandidateArray &uniqueSelectedMods);

        bool SelectMod(const ModCandidate &mod,
                       ModCandidateMap &selectedMods,
                       ModCandidateArray &uniqueSelectedMods);
    };
}

#endif // BALLOON_MODRESOLVER_H
