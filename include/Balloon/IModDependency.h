/**
 * @file IModDependency.h
 * @brief The interface for querying mod dependency information
 */
#ifndef BALLOON_IMODDEPENDENCY_H
#define BALLOON_IMODDEPENDENCY_H

#include <cstddef>

namespace balloon {
    inline namespace v1 {

        /**
         * @enum ModDependencyType
         * @brief The type of mod dependency.
         */
        enum ModDependencyType {
            MDT_DEPEND = 0,     /**< Dependency */
            MDT_RECOMMEND = 1,  /**< Recommendation */
            MDT_SUGGEST = 2,    /**< Suggestion */
            MDT_CONFLICT = 3,   /**< Conflict */
            MDT_BREAK = 4       /**< Break */
        };

        /**
         * @class IModDependency
         * @brief The interface for mod dependency.
         */
        class IModDependency {
        public:
            /**
             * @brief Get the ID of the dependency.
             * @return The ID of the dependency.
             */
            virtual const char *GetId() const = 0;

            /**
             * @brief Get the type of the dependency.
             * @return The type of the dependency.
             */
            virtual int GetType() const = 0;

            /**
             * @brief Check if a version matches the dependency requirements.
             * @param version The version to check.
             * @return True if the version matches, false otherwise.
             */
            virtual bool Matches(const char *version) const = 0;

            /**
             * @brief Get the version requirements of the dependency.
             * @return The version requirements of the dependency.
             */
            virtual const char *GetVersionRequirements() const = 0;
        };
    }
}

#endif // BALLOON_IMODDEPENDENCY_H
