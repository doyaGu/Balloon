#ifndef BALLOON_EVENTMANAGER_H
#define BALLOON_EVENTMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Balloon/IEventManager.h"
#include "Event.h"

namespace balloon {
    class EventManager final : public IEventManager {
    public:
        static EventManager &GetInstance();

        EventManager(const EventManager &rhs) = delete;
        EventManager(EventManager &&rhs) noexcept = delete;

        ~EventManager();

        EventManager &operator=(const EventManager &rhs) = delete;
        EventManager &operator=(EventManager &&rhs) noexcept = delete;

        void Reset();

        EventType AddEventType(const char *name) override;

        EventType GetEventType(const char *name) const override;
        const char *GetEventTypeName(EventType type) const override;
        size_t GetEventTypeCount() const override;

        bool RenameEventType(EventType type, const char *name) override;
        bool RenameEventType(const char *oldName, const char *newName) override;

        IEvent *NewEvent(EventType type) override;
        IEvent *NewEvent(const char *name) override;

        bool SendEvent(IEvent *event) override;
        bool SendEvent(EventType type) override;
        bool SendEvent(const char *name) override;

        bool AddListener(const char *eventName, IEventListener *listener) override;
        bool AddListener(EventType eventType, IEventListener *listener) override;

        bool RemoveListener(const char *eventName, IEventListener *listener) override;
        bool RemoveListener(EventType eventType, IEventListener *listener) override;

        bool RemoveAllListeners(const char *eventName);
        bool RemoveAllListeners(EventType eventType);

    private:
        EventManager();

        std::vector<bool> m_EventStatus;
        std::vector<std::string> m_EventTypes;
        std::unordered_map<std::string, EventType> m_EventTypeMap;
        std::unordered_map<EventType, std::vector<IEventListener *>> m_EventListeners;
    };
}

#endif // BALLOON_EVENTMANAGER_H
