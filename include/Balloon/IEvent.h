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

        class IEvent {
        public:
            virtual int AddRef() const = 0;
            virtual int Release() const = 0;

            virtual EventType GetType() const = 0;
            virtual void SetType(EventType type) = 0;

            virtual int GetFlag() const = 0;
            virtual void SetFlag(int flag) = 0;

            virtual IDataStack *GetDataStack() const = 0;
            virtual void SetDataStack(IDataStack *stack) = 0;

        protected:
            virtual ~IEvent() = default;
        };
    }
}

#endif // BALLOON_IEVENT_H
