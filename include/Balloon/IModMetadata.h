/**
 * @file IModMetadata.h
 * @brief The interface for querying mod metadata information
 */
#ifndef BALLOON_IMODMETADATA_H
#define BALLOON_IMODMETADATA_H

#include <cstdint>

#include "Balloon/IModDependency.h"

namespace balloon {
    inline namespace v1 {
        class IModMetadata {
        public:
            virtual const char *GetId() const = 0;
            virtual const char *GetVersion() const = 0;
            virtual const char *GetType() const = 0;

            virtual size_t GetDependencyCount() const = 0;
            virtual IModDependency *GetDependencyByIndex(size_t index) const = 0;

            virtual const char *GetName() const = 0;
            virtual const char *GetDescription() const = 0;

            virtual size_t GetAuthorCount() const = 0;
            virtual const char *GetAuthorByIndex(size_t index) const = 0;

            virtual size_t GetContributorCount() const = 0;
            virtual const char *GetContributorByIndex(size_t index) const = 0;

            virtual const char *GetHomepage() const = 0;
            virtual const char *GetRepository() const = 0;
            virtual const char *GetLicense() const = 0;

            virtual size_t GetKeywordCount() const = 0;
            virtual const char *GetKeywordByIndex(size_t index) const = 0;

            virtual size_t GetCategoryCount() const = 0;
            virtual const char *GetCategoryByIndex(size_t index) const = 0;

            virtual void *GetUserData(size_t type = 0) const = 0;
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_IMODMETADATA_H
