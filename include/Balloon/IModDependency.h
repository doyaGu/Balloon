/**
 * @file IModDependency.h
 * @brief The interface for querying mod dependency information
 */
#ifndef BALLOON_IMODDEPENDENCY_H
#define BALLOON_IMODDEPENDENCY_H

#include <cstddef>

namespace balloon {
    inline namespace v1 {
        enum ModDependencyType {
            MDT_DEPEND = 0,
            MDT_RECOMMEND = 1,
            MDT_SUGGEST = 2,
            MDT_CONFLICT = 3,
            MDT_BREAK = 4,
        };

        class IModDependency {
        public:
            virtual const char *GetId() const = 0;
            virtual int GetType() const = 0;
            virtual bool Matches(const char *version) const = 0;

            virtual const char *GetVersionRequirements() const = 0;
        };
    }
}

#endif // BALLOON_IMODDEPENDENCY_H
