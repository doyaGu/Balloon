/**
 * @file IWeakRefFlag.h
 * @brief The interface of week reference flag
 */
#ifndef BALLOON_IWEAKREFFLAG_H
#define BALLOON_IWEAKREFFLAG_H

#include "Balloon/Export.h"

namespace balloon {
    inline namespace v1 {
        /**
         * @interface IWeakRefFlag
         * @brief The interface of week reference flag
         */
        class IWeakRefFlag {
        public:
            // Memory management
            virtual int AddRef() const = 0;
            virtual int Release() const = 0;

            // Value
            virtual bool Get() const = 0;
            virtual void Set(bool value) = 0;

            // Thread management
            virtual void Lock() const = 0;
            virtual void Unlock() const = 0;

        protected:
            virtual ~IWeakRefFlag() = default;
        };
    }
}

#endif // BALLOON_IWEAKREFFLAG_H
