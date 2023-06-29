#include "ModResolver.h"

#include <algorithm>
#include <chrono>

#include "Logger.h"

using namespace balloon;

ModResolver &ModResolver::GetInstance() {
    static ModResolver instance;
    return instance;
}

ModResolver::~ModResolver() = default;

ModCandidateArray ModResolver::Resolve(const ModCandidateSet &candidates, const ModCandidateSetMap &disabledMods) {
    auto startTimePoint = std::chrono::steady_clock::now();

    auto result = FindCompatibleSet(candidates, disabledMods);

    auto endTimePoint = std::chrono::steady_clock::now();
    auto timeSpan = std::chrono::duration_cast<std::chrono::microseconds>(endTimePoint - startTimePoint);
    LOG_DEBUG("Mod resolution time: %.1f ms", timeSpan.count());

    return result;
}

ModResolver::ModResolver() = default;

ModCandidateArray ModResolver::FindCompatibleSet(const ModCandidateSet &candidates, const ModCandidateSetMap &disabledMods) {
    // Sort all mods
    ModCandidateArray allModsSorted(candidates.begin(), candidates.end());
    std::sort(allModsSorted.begin(), allModsSorted.end());

    // Group all mods by id
    ModCandidateSetMap modsById;
    for (auto &mod: allModsSorted) {
        bool inserted;
        ModCandidateSetMap::iterator it;
        std::tie(it, inserted) = modsById.insert({mod.GetId(), {mod}});
        if (!inserted)
            it->second.insert(mod);
    }

    // Preselect mods, check for builtin mod collisions
    ModCandidateArray preselectedMods;
    for (auto &pair: modsById) {
        auto &mods = pair.second;

        ModCandidate builtinMod;
        for (auto &mod: mods) {
            if (mod.IsBuiltin()) {
                builtinMod = mod;
                break;
            }
        }

        if (!builtinMod.IsValid())
            continue;

        if (mods.size() > 1) {
            mods.erase(builtinMod);
            LOG_ERROR("Mods share ID with builtin mod %s: %s", builtinMod.GetId(), pair.first.c_str());
            break;
        }

        preselectedMods.emplace_back(std::move(builtinMod));
    }

    ModCandidateMap selectedMods;
    ModCandidateArray uniqueSelectedMods;

    for (auto &mod: preselectedMods) {
        PreselectMod(mod, allModsSorted, modsById, selectedMods, uniqueSelectedMods);
    }

    {
        auto solver = std::make_unique<ModSolver>(this);
        if (!solver->Solve(allModsSorted, modsById, selectedMods, uniqueSelectedMods)) {
            LOG_ERROR("Failed to solve mods");
            LOG_ERROR(solver->GetUnsatisfiedModsError().c_str());
        }
    }

    return uniqueSelectedMods;
}

bool ModResolver::PreselectMod(const ModCandidate &mod, ModCandidateArray &allModsSorted, ModCandidateSetMap &modsById, ModCandidateMap &selectedMods,
                               ModCandidateArray &uniqueSelectedMods) {
    if (!SelectMod(mod, selectedMods, uniqueSelectedMods))
        return false;

    auto it = modsById.find(mod.GetId());
    if (it != modsById.end()) {
        for (auto &m: it->second) {
            allModsSorted.erase(std::remove(allModsSorted.begin(), allModsSorted.end(), m), allModsSorted.end());
        }
        modsById.erase(it);
    }

    return true;
}

bool ModResolver::SelectMod(const ModCandidate &mod, ModCandidateMap &selectedMods, ModCandidateArray &uniqueSelectedMods) {
    bool inserted;
    ModCandidateMap::iterator it;
    std::tie(it, inserted) = selectedMods.insert({mod.GetId(), mod});
    if (!inserted) {
        LOG_ERROR("Found duplicate mod %s", mod.GetId());
        return false;
    }

    uniqueSelectedMods.push_back(mod);
    return true;
}