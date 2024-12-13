/**
 * @file IEvent.h
 * @brief The interface of event.
 */
#ifndef BALLOON_IEVENT_H
#define BALLOON_IEVENT_H

#include <cstddef>

#include "Balloon/IDataStack.h"

namespace balloon {
    inline namespace v1 {
        typedef size_t EventType;

        /**
         * @brief The interface for event objects.
         */
        class IEvent {
        public:
            /**
             * @brief Increments the reference count of the event.
             * @return The updated reference count.
             */
            virtual int AddRef() const = 0;

            /**
             * @brief Decrements the reference count of the event.
             * @return The updated reference count.
             */
            virtual int Release() const = 0;

            /**
             * @brief Gets the type of the event.
             * @return The event type.
             */
            virtual EventType GetType() const = 0;

            /**
             * @brief Sets the type of the event.
             * @param type The event type to set.
             */
            virtual void SetType(EventType type) = 0;

            /**
             * @brief Gets the flag associated with the event.
             * @return The flag value.
             */
            virtual int GetFlag() const = 0;

            /**
             * @brief Sets the flag associated with the event.
             * @param flag The flag value to set.
             */
            virtual void SetFlag(int flag) = 0;

            /**
             * @brief Gets the data stack associated with the event.
             * @return A pointer to the IDataStack object.
             */
            virtual IDataStack *GetDataStack() const = 0;

            /**
             * @brief Sets the data stack associated with the event.
             * @param stack A pointer to the IDataStack object.
             */
            virtual void SetDataStack(IDataStack *stack) = 0;

        protected:
            virtual ~IEvent() = default;
        };
    }
}

#endif // BALLOON_IEVENT_H
