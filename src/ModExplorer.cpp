#include "ModExplorer.h"

#include <future>
#include <string>
#include <unordered_set>
#include <chrono>
#include <functional>

#include "Logger.h"

using namespace balloon;

ModExplorer::ModExplorer() : m_Scanner(std::make_unique<ModCandidateScanner>()) {}

ModExplorer::~ModExplorer() = default;

ModCandidateSet ModExplorer::ExploreMods(ModLoader *loader, ModCandidateSetMap& envDisabledModsOut) {
    auto startTimePoint = std::chrono::steady_clock::now();

    std::unordered_set<std::string> paths;

    using ModCandidateScanner = std::function<bool(ModCandidate &)>;
    std::vector<ModCandidateScanner> scanners;

    ModCandidateConsumer taskSubmitter = [&](const std::string &modPath) {
        auto result = paths.insert(modPath);
        if (result.second) {
            scanners.emplace_back([modPath, this](ModCandidate &out) { return m_Scanner->ScanCandidate(modPath, out); });
        }
    };

    for (const auto &finder: m_Finders) {
        finder->FindCandidates(taskSubmitter);
    }

    ModCandidateSet candidates;

    for (auto &scanner : scanners) {
        ModCandidate candidate;
        if (scanner(candidate)) {
            candidates.emplace(std::move(candidate));
        }
    }

    auto endTimePoint = std::chrono::steady_clock::now();
    auto timeSpan = std::chrono::duration_cast<std::chrono::microseconds>(endTimePoint - startTimePoint);
    LOG_DEBUG("Mod explore time: %.1f ms", timeSpan.count());

    return candidates;
}

void ModExplorer::AddCandidateFinder(const std::shared_ptr<ModCandidateFinder> &finder) {
    m_Finders.push_back(finder);
}