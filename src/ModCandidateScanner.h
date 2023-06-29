#ifndef BALLOON_MODCANDIDATESCANNER_H
#define BALLOON_MODCANDIDATESCANNER_H

#include <memory>
#include <string>

#include "ModCandidate.h"

namespace balloon {
    class ModCandidateScanner final {
    public:
        ModCandidateScanner();

        ModCandidateScanner(const ModCandidateScanner &rhs) = delete;
        ModCandidateScanner(ModCandidateScanner &&rhs) noexcept = delete;

        ~ModCandidateScanner();

        ModCandidateScanner &operator=(const ModCandidateScanner &rhs) = delete;
        ModCandidateScanner &operator=(ModCandidateScanner &&rhs) noexcept = delete;

        bool ScanCandidate(const std::string &path, ModCandidate &out);

    private:
        bool ScanDirectory(const std::string &path, ModCandidate &out);
        bool ScanArchive(const std::string &path, ModCandidate &out);
    };
}

#endif // BALLOON_MODCANDIDATESCANNER_H
