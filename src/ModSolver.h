#ifndef BALLOON_MODSOLVER_H
#define BALLOON_MODSOLVER_H

#include <vector>
#include <list>
#include <map>
#include <set>
#include <memory>

#include "ModCandidate.h"

namespace balloon {
    class ModResolver;

    class ModSolver final {
    public:
        explicit ModSolver(ModResolver *resolver);

        ModSolver(const ModSolver &rhs) = delete;
        ModSolver(ModSolver &&rhs) noexcept = delete;

        ~ModSolver();

        ModSolver &operator=(const ModSolver &rhs) = delete;
        ModSolver &operator=(ModSolver &&rhs) noexcept = delete;

        bool Solve(ModCandidateArray &allModsSorted,
                   ModCandidateSetMap &modsById,
                   ModCandidateMap &selectedMods,
                   ModCandidateArray &uniqueSelectedMods);

        bool IsConsistent() const { return m_UnsatisfiedMods.empty(); }

        const ModCandidateArray &getUnsatisfiedMods() const {
            return m_UnsatisfiedMods;
        }

        std::string GetUnsatisfiedModsError();

    private:
        void ResolveDependencies();

        ModResolver *m_Resolver;
        ModCandidateArray m_PreselectedMods;
        ModCandidateArray m_SortedMods;
        ModCandidateArray m_UnsatisfiedMods;
        std::unordered_map<std::string, std::vector<std::shared_ptr<ModDependency>>> m_UnsatisfiedDependenciesMap;
    };
}

#endif // BALLOON_MODSOLVER_H
