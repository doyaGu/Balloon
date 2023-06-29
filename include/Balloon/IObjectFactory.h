/**
 * @file IObjectFactory.h
 * @brief The interface of object factory
 */
#ifndef BALLOON_IOBJECTFACTORY_H
#define BALLOON_IOBJECTFACTORY_H

#include <cstddef>

namespace balloon {
    inline namespace v1 {
        /**
         * @interface IObjectFactory
         * @brief The interface of object factory
         *
         * Object factory classes should implement this interface.
         *
         * Instances of object factories can be registered via mod context.
         *
         * Instances of object factories should not be destroyed until the mod is shutdown.
         */
        class IObjectFactory {
        public:
            /**
             * @brief Create an instance.
             * @param data The parameters required to construct a new object, if no parameter, pass nullptr.
             * @return The raw pointer to the new object instance.
             */
            virtual void *CreateInstance(void *data) const = 0;

            /**
             * @brief Destroy an instance.
             * @param ptr The raw pointer to the object instance.
             */
            virtual void DestroyInstance(void *ptr) = 0;
        };
    }
}

#endif // BALLOON_IOBJECTFACTORY_Hs
