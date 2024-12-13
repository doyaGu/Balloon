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
        /**
         * @class IModMetadata
         * @brief The interface for mod metadata.
         */
        class IModMetadata {
        public:
            /**
             * @brief Get the ID of the mod.
             * @return The ID of the mod.
             */
            virtual const char *GetId() const = 0;

            /**
             * @brief Get the version of the mod.
             * @return The version of the mod.
             */
            virtual const char *GetVersion() const = 0;

            /**
             * @brief Get the type of the mod.
             * @return The type of the mod.
             */
            virtual const char *GetType() const = 0;

            /**
             * @brief Get the number of dependencies for the mod.
             * @return The number of dependencies for the mod.
             */
            virtual size_t GetDependencyCount() const = 0;

            /**
             * @brief Get a dependency by index.
             * @param index The index of the dependency.
             * @return A pointer to the mod dependency.
             */
            virtual IModDependency *GetDependencyByIndex(size_t index) const = 0;

            /**
             * @brief Get the name of the mod.
             * @return The name of the mod.
             */
            virtual const char *GetName() const = 0;

            /**
             * @brief Get the description of the mod.
             * @return The description of the mod.
             */
            virtual const char *GetDescription() const = 0;

            /**
             * @brief Get the number of authors for the mod.
             * @return The number of authors for the mod.
             */
            virtual size_t GetAuthorCount() const = 0;

            /**
             * @brief Get an author by index.
             * @param index The index of the author.
             * @return The author at the given index.
             */
            virtual const char *GetAuthorByIndex(size_t index) const = 0;

            /**
             * @brief Get the number of contributors for the mod.
             * @return The number of contributors for the mod.
             */
            virtual size_t GetContributorCount() const = 0;

            /**
             * @brief Get a contributor by index.
             * @param index The index of the contributor.
             * @return The contributor at the given index.
             */
            virtual const char *GetContributorByIndex(size_t index) const = 0;

            /**
             * @brief Get the homepage URL of the mod.
             * @return The homepage URL of the mod.
             */
            virtual const char *GetHomepage() const = 0;

            /**
             * @brief Get the repository URL of the mod.
             * @return The repository URL of the mod.
             */
            virtual const char *GetRepository() const = 0;

            /**
             * @brief Get the license of the mod.
             * @return The license of the mod.
             */
            virtual const char *GetLicense() const = 0;

            /**
             * @brief Get the number of keywords for the mod.
             * @return The number of keywords for the mod.
             */
            virtual size_t GetKeywordCount() const = 0;

            /**
             * @brief Get a keyword by index.
             * @param index The index of the keyword.
             * @return The keyword at the given index.
             */
            virtual const char *GetKeywordByIndex(size_t index) const = 0;

            /**
             * @brief Get the number of categories for the mod.
             * @return The number of categories for the mod.
             */
            virtual size_t GetCategoryCount() const = 0;

            /**
             * @brief Get a category by index.
             * @param index The index of the category.
             * @return The category at the given index.
             */
            virtual const char *GetCategoryByIndex(size_t index) const = 0;

            /**
             * @brief Get user data of the specified type.
             * @param type The type of user data.
             * @return A pointer to the user data.
             */
            virtual void *GetUserData(size_t type = 0) const = 0;

            /**
             * @brief Set user data of the specified type.
             * @param data Pointer to the user data.
             * @param type The type of user data.
             * @return A pointer to the previous user data.
             */
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_IMODMETADATA_H
