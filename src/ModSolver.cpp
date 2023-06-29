#include "ModSolver.h"

#include <sstream>

#include "ModResolver.h"

using namespace balloon;

ModSolver::ModSolver(ModResolver *resolver) : m_Resolver(resolver) {}

ModSolver::~ModSolver() = default;

bool ModSolver::Solve(ModCandidateArray &allModsSorted, ModCandidateSetMap &modsById, ModCandidateMap &selectedMods, ModCandidateArray &uniqueSelectedMods) {
    m_UnsatisfiedMods.assign(uniqueSelectedMods.begin(), uniqueSelectedMods.end());
    ResolveDependencies();
    if (!IsConsistent())
        return false;

    uniqueSelectedMods.clear();
    uniqueSelectedMods.assign(m_SortedMods.begin(), m_SortedMods.end());
    m_SortedMods.clear();

    m_PreselectedMods.assign(uniqueSelectedMods.begin(), uniqueSelectedMods.end());
    m_UnsatisfiedMods.assign(allModsSorted.begin(), allModsSorted.end());
    ResolveDependencies();
    for (auto &mod : m_SortedMods) {
        m_Resolver->SelectMod(mod, selectedMods, uniqueSelectedMods);
    }

    return true;
}

std::string ModSolver::GetUnsatisfiedModsError() {
    std::ostringstream error;
    error << "Some mods have unsatisfied dependencies:" << std::endl;

    for (const auto &mod : m_UnsatisfiedMods) {
        std::string id = mod.GetId();
        error << " - " << id << "is missing:";

        for (const auto &dep : m_UnsatisfiedDependenciesMap[id])
            error << " " << dep->GetId();
        error << "\n";
    }

    error << "\nInstall and enable the required mods, or disable the mods causing errors.\n"
          << "Note: this may be caused by a dependency cycle, in which case try updating the mods.";

    return error.str();
}

void ModSolver::ResolveDependencies() {
    if (m_UnsatisfiedMods.empty())
        return;

    ModCandidateArray satisfied;
    ModCandidateList unsatisfied;

    for (auto &mod : m_PreselectedMods) {
        satisfied.push_back(mod);
    }

    for (auto &mod : m_UnsatisfiedMods) {
        auto depends = mod.GetDependencies();
        std::vector<std::shared_ptr<ModDependency>> deps;
        for (auto &dep : depends) {
            if (dep->GetType() == MDT_DEPEND) {
                deps.push_back(dep);
            }
        }
        if (deps.empty()) {
            satisfied.push_back(mod);
        } else {
            unsatisfied.push_back(mod);
            m_UnsatisfiedDependenciesMap[mod.GetId()] = deps;
        }
    }

    while (!satisfied.empty()) {
        auto mod = satisfied.back();
        m_SortedMods.push_back(mod);
        satisfied.pop_back();
        for (auto it = unsatisfied.begin(); it != unsatisfied.end();) {
            auto &mod2 = *it;
            auto &deps = m_UnsatisfiedDependenciesMap[mod2.GetId()];
            auto t = std::find_if(deps.begin(), deps.end(), [mod](const std::shared_ptr<ModDependency> &dep) -> bool {
                if (strcmp(dep->GetId(), mod.GetId()) != 0)
                    return false;
                if (!dep->Matches(mod.GetVersion()))
                    return false;
                return true;
            });
            if (t != deps.end())
                deps.erase(t);
            if (deps.empty()) {
                satisfied.push_back(mod2);
                it = unsatisfied.erase(it);
            } else {
                ++it;
            }
        }
    }

    m_UnsatisfiedMods.assign(unsatisfied.begin(), unsatisfied.end());
}
