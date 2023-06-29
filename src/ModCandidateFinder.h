#ifndef BALLOON_MODCANDIDATEFINDER_H
#define BALLOON_MODCANDIDATEFINDER_H

#include <string>
#include <set>
#include <functional>

#include "ModLoader.h"

namespace balloon {
    using ModCandidateConsumer = std::function<void(const std::string&)>;

    class ModCandidateFinder {
    public:
        virtual ~ModCandidateFinder() = default;

        virtual bool FindCandidates(ModCandidateConsumer &out) = 0;
    };

    class DirectoryModCandidateFinder : public ModCandidateFinder {
    public:
        explicit DirectoryModCandidateFinder(std::string path);

        ~DirectoryModCandidateFinder() override = default;

        bool FindCandidates(ModCandidateConsumer &out) override;

    private:
        static bool IsSupportedArchive(const std::string &path);
        static bool IsValidMod(const std::string &path);

        std::string m_Path;
    };
}

#endif // BALLOON_MODCANDIDATEFINDER_H
