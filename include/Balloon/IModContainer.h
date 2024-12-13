/**
 * @file IModContainer.h
 * @brief The interface for querying mod primary information
 */
#ifndef BALLOON_IMODCONTAINER_H
#define BALLOON_IMODCONTAINER_H

#include "Balloon/IModMetadata.h"

namespace balloon {
    inline namespace v1 {
        class IModContainer {
        public:
            /**
              * @brief Get the ID of the mod.
              *
              * @return The ID of the mod as a null-terminated string.
              */
            virtual const char *GetId() const = 0;

            /**
             * @brief Get the version of the mod.
             *
             * @return The version of the mod as a null-terminated string.
             */
            virtual const char *GetVersion() const = 0;

            /**
             * @brief Get the flags associated with the mod.
             *
             * @return The flags associated with the mod as an unsigned integer.
             */
            virtual uint32_t GetFlags() const = 0;

            /**
             * @brief Get the metadata associated with the mod.
             *
             * @return A pointer to the mod's metadata.
             */
            virtual IModMetadata *GetMetadata() const = 0;

            /**
             * @brief Get the library associated with the mod.
             *
             * @return A pointer to the mod's library.
             */
            virtual void *GetLibrary() const = 0;

            /**
             * @brief Get the root path of the mod.
             *
             * @return The root path of the mod as a null-terminated string.
             */
            virtual const char *GetRootPath() const = 0;

            /**
             * @brief Get the source path of the mod.
             *
             * @return The source path of the mod as a null-terminated string.
             */
            virtual const char *GetSourcePath() const = 0;

            /**
             * @brief Get user data associated with the mod.
             *
             * @param type The type of user data to retrieve (optional).
             * @return A pointer to the user data associated with the mod.
             */
            virtual void *GetUserData(size_t type = 0) const = 0;

            /**
             * @brief Set user data associated with the mod.
             *
             * @param data A pointer to the user data to set.
             * @param type The type of user data to set (optional).
             * @return A pointer to the previously associated user data.
             */
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_IMODCONTAINER_H
