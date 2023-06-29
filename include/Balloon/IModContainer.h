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
            virtual const char *GetId() const = 0;
            virtual const char *GetVersion() const = 0;
            virtual uint32_t GetFlags() const = 0;

            virtual IModMetadata *GetMetadata() const = 0;
            virtual void *GetLibrary() const = 0;

            virtual const char *GetRootPath() const = 0;
            virtual const char *GetSourcePath() const = 0;

            virtual void *GetUserData(size_t type = 0) const = 0;
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_IMODCONTAINER_H
