/**
 * @file IModContext.h
 * @brief The all-in-one context for mod.
 */
#ifndef BALLOON_IMODCONTEXT_H
#define BALLOON_IMODCONTEXT_H

#include "Balloon/IObjectFactory.h"
#include "Balloon/IModContainer.h"
#include "Balloon/IFileSystem.h"
#include "Balloon/IDataShare.h"
#include "Balloon/IEventManager.h"
#include "Balloon/IDataStack.h"
#include "Balloon/IWeakRefFlag.h"
#include "Balloon/ILogger.h"
#include "Balloon/IConfig.h"

namespace balloon {
    inline namespace v1 {
        /**
         * @interface IModContext
         * @brief The interface of mod context.
         */
        class IModContext {
        public:
            // Mod
            virtual size_t GetModCount() const = 0;
            virtual IModContainer *GetMod(size_t index) const = 0;
            virtual IModContainer *GetMod(const char *id) const = 0;

            // Interface
            virtual bool RegisterInterface(void *interface, const char *name, const char *owner, int version) = 0;
            virtual bool HasInterface(const char *id) const = 0;
            virtual void *GetInterface(const char *name, const char *owner, int version) const = 0;

            IFileSystem *GetFileSystem() const {
                return static_cast<IFileSystem *>(GetInterface("fs", nullptr, 1));
            }

            IDataShare *GetDataShare() const {
                return static_cast<IDataShare *>(GetInterface("ds", nullptr, 1));
            }

            IEventManager *GetEventManager() const {
                return static_cast<IEventManager *>(GetInterface("em", nullptr, 1));
            }

            // Object Factory
            virtual bool RegisterFactory(IObjectFactory *factory, const char *name, const char *owner, int version) = 0;
            virtual bool HasFactory(const char *id) const = 0;
            virtual IObjectFactory *GetFactory(const char *name, const char *owner, int version) const = 0;

            void *CreateObject(void *data, const char *name, const char *owner, int version) const {
                auto *factory = GetFactory(name, owner, version);
                if (factory)
                    return factory->CreateInstance(data);
                return nullptr;
            }

            IDataStack *CreateDataStack() const {
                return static_cast<IDataStack *>(CreateObject(nullptr, "DataStack", nullptr, 1));
            }

            IWeakRefFlag *CreateWeakRefFlag() const {
                return static_cast<IWeakRefFlag *>(CreateObject(nullptr, "WeakRefFlag", nullptr, 1));
            }

            virtual ILogger *GetLogger(const char *id) const = 0;
            virtual IConfig *GetConfig(const char *id) const = 0;

            // User data
            virtual void *GetUserData(size_t type = 0) const = 0;
            virtual void *SetUserData(void *data, size_t type = 0) = 0;
        };
    }
}

#endif // BALLOON_IMODCONTEXT_H
