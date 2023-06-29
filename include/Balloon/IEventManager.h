/**
 * @file IEventManager.h
 * @brief The interface of event manager.
 */
#ifndef BALLOON_IEVENTMANAGER_H
#define BALLOON_IEVENTMANAGER_H

#include "Balloon/IEvent.h"

namespace balloon {
    inline namespace v1 {
        class IEventListener {
        public:
            // OnEvent is called by EventManager if event just occurred
            virtual int OnEvent(const IEvent *event) = 0;
        };

        class IEventManager {
            public:
            // Event Type
            virtual EventType AddEventType(const char *name) = 0;

            virtual EventType GetEventType(const char *name) const = 0;
            virtual const char *GetEventTypeName(EventType type) const = 0;
            virtual size_t GetEventTypeCount() const = 0;

            virtual bool RenameEventType(EventType type, const char *name) = 0;
            virtual bool RenameEventType(const char *oldName, const char *newName) = 0;

            // Event
            virtual IEvent *NewEvent(EventType type) = 0;
            virtual IEvent *NewEvent(const char *name) = 0;

            virtual bool SendEvent(IEvent *event) = 0;
            virtual bool SendEvent(EventType type) = 0;
            virtual bool SendEvent(const char *name) = 0;

            // Listener
            virtual bool AddListener(const char *eventName, IEventListener *listener) = 0;
            virtual bool AddListener(EventType eventType, IEventListener *listener) = 0;

            virtual bool RemoveListener(const char *eventName, IEventListener *listener) = 0;
            virtual bool RemoveListener(EventType eventType, IEventListener *listener) = 0;
        };
    }
}

#endif // BALLOON_IEVENTMANAGER_H
