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

            /**
             * @brief Get the number of mods.
             * @return The number of mods.
             */
            virtual size_t GetModCount() const = 0;

            /**
             * @brief Get a mod by index.
             * @param index The index of the mod.
             * @return A pointer to the mod container.
             */
            virtual IModContainer *GetMod(size_t index) const = 0;

            /**
             * @brief Get a mod by ID.
             * @param id The ID of the mod.
             * @return A pointer to the mod container.
             */
            virtual IModContainer *GetMod(const char *id) const = 0;

            // Interface

            /**
             * @brief Register an interface.
             *
             * This function registers an interface with the given parameters.
             *
             * @param interface A pointer to the interface object.
             * @param name The name of the interface.
             * @param owner The owner of the interface.
             * @param version The version of the interface.
             * @return True if the registration was successful, false otherwise.
             */
            virtual bool RegisterInterface(void *interface, const char *name, const char *owner, int version) = 0;

            /**
              * @brief Check if an interface exists.
              * @param id The ID of the interface.
              * @return True if the interface exists, false otherwise.
              */
            virtual bool HasInterface(const char *id) const = 0;

            /**
             * @brief Get an interface by name, owner, and version.
             * @param name The name of the interface.
             * @param owner The owner of the interface.
             * @param version The version of the interface.
             * @return A pointer to the interface object.
             */
            virtual void *GetInterface(const char *name, const char *owner, int version) const = 0;

            /**
             * @brief Get the file system interface.
             * @return A pointer to the file system interface.
             */
            IFileSystem *GetFileSystem() const {
                return static_cast<IFileSystem *>(GetInterface("fs", nullptr, 1));
            }

            /**
             * @brief Get the data share interface.
             * @return A pointer to the data share interface.
             */
            IDataShare *GetDataShare() const {
                return static_cast<IDataShare *>(GetInterface("ds", nullptr, 1));
            }

            /**
             * @brief Get the event manager interface.
             * @return A pointer to the event manager interface.
             */
            IEventManager *GetEventManager() const {
                return static_cast<IEventManager *>(GetInterface("em", nullptr, 1));
            }

            // Object Factory

            /**
             * @brief Register an object factory.
             *
             * This function registers an object factory with the given parameters.
             *
             * @param factory A pointer to the object factory.
             * @param name The name of the factory.
             * @param owner The owner of the factory.
             * @param version The version of the factory.
             * @return True if the registration was successful, false otherwise.
             */
            virtual bool RegisterFactory(IObjectFactory *factory, const char *name, const char *owner, int version) = 0;

            /**
             * @brief Check if a factory exists.
             * @param id The ID of the factory.
             * @return True if the factory exists, false otherwise.
             */
            virtual bool HasFactory(const char *id) const = 0;

            /**
             * @brief Get a factory by name, owner, and version.
             * @param name The name of the factory.
             * @param owner The owner of the factory.
             * @param version The version of the factory.
             * @return A pointer to the object factory.
             */
            virtual IObjectFactory *GetFactory(const char *name, const char *owner, int version) const = 0;

            /**
             * @brief Create an object using the specified factory.
             *
             * This function creates an object using the specified factory with the given parameters.
             *
             * @param data Pointer to additional data used during object creation.
             * @param name The name of the object.
             * @param owner The owner of the object.
             * @param version The version of the object.
             * @return A pointer to the created object.
             */
            void *CreateObject(void *data, const char *name, const char *owner, int version) const {
                auto *factory = GetFactory(name, owner, version);
                if (factory)
                    return factory->CreateInstance(data);
                return nullptr;
            }

            /**
             * @brief Create a data stack object.
             * @return A pointer to the created data stack object.
             */
            IDataStack *CreateDataStack() const {
                return static_cast<IDataStack *>(CreateObject(nullptr, "DataStack", nullptr, 1));
            }

            /**
             * @brief Create a weak reference flag object.
             * @return A pointer to the created weak reference flag object.
             */
            IWeakRefFlag *CreateWeakRefFlag() const {
                return static_cast<IWeakRefFlag *>(CreateObject(nullptr, "WeakRefFlag", nullptr, 1));
            }

            /**
             * @brief Get the logger by ID.
             * @param id The ID of the logger.
             * @return A pointer to the logger.
             */
            virtual ILogger *GetLogger(const char *id) const = 0;

            /**
             * @brief Get the config by ID.
             * @param id The ID of the config.
             * @return A pointer to the config.
             */
            virtual IConfig *GetConfig(const char *id) const = 0;

            // User data

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

#endif // BALLOON_IMODCONTEXT_H
