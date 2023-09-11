#include "EventManager.h"

using namespace balloon;

EventManager &EventManager::GetInstance() {
    static EventManager instance;
    return instance;
}

EventManager::~EventManager() = default;

void EventManager::Reset() {
    m_EventListeners.clear();
    m_EventTypeMap.clear();
    m_EventTypes.clear();
    m_EventStatus.clear();
}

EventType EventManager::AddEventType(const char *name) {
    if (!name) return -1;
    std::string eventName = name;
    auto it = m_EventTypeMap.find(eventName);
    if (it != m_EventTypeMap.end())
        return -1;

    EventType type = m_EventTypes.size();
    m_EventTypeMap[eventName] = type;
    m_EventTypes.emplace_back(std::move(eventName));
    m_EventStatus.push_back(false);
    return type;
}

EventType EventManager::GetEventType(const char *name) const {
    if (!name) return -1;
    auto it = m_EventTypeMap.find(name);
    if (it == m_EventTypeMap.end())
        return -1;
    return it->second;
}

const char *EventManager::GetEventTypeName(EventType type) const {
    if (type >= m_EventTypes.size())
        return nullptr;
    return m_EventTypes[type].c_str();
}

size_t EventManager::GetEventTypeCount() const {
    return m_EventTypes.size();
}

bool EventManager::RenameEventType(EventType type, const char *name) {
    if (type >= m_EventTypes.size() || !name)
        return false;

    std::string newName = name;
    auto it = m_EventTypeMap.find(newName);
    if (it == m_EventTypeMap.end())
        return false;

    auto &oldName = m_EventTypes[type];
    m_EventTypeMap.erase(oldName);
    m_EventTypeMap[newName] = type;
    m_EventTypes[type] = std::move(newName);
    return true;
}

bool EventManager::RenameEventType(const char *oldName, const char *newName) {
    if (!oldName || !newName)
        return false;

    std::string name = newName;
    auto it = m_EventTypeMap.find(name);
    if (it == m_EventTypeMap.end())
        return false;

    auto type = GetEventType(oldName);
    if (type == -1)
        return false;

    m_EventTypeMap.erase(oldName);
    m_EventTypeMap[name] = type;
    m_EventTypes[type] = std::move(name);
    return true;
}

IEvent *EventManager::NewEvent(EventType type) {
    if (type >= m_EventTypes.size())
        return nullptr;
    return Event::Create(type);
}

IEvent *EventManager::NewEvent(const char *name) {
    return NewEvent(GetEventType(name));
}

bool EventManager::SendEvent(IEvent *event) {
    if (!event)
        return false;

    auto type = event->GetType();
    if (type >= m_EventTypes.size())
        return false;

    if (m_EventStatus[type])
        return false;

    m_EventStatus[type] = true;
    auto &listeners = m_EventListeners[type];
    auto it = listeners.begin();
    while (it != listeners.end()) {
        if ((*it)->OnEvent(event) == 0) {
            it = listeners.erase(it);
        } else {
            ++it;
        }
    }
    m_EventStatus[type] = false;

    return true;
}

bool EventManager::SendEvent(EventType type) {
    if (type >= m_EventTypes.size())
        return false;

    if (m_EventStatus[type])
        return false;

    m_EventStatus[type] = true;
    auto *event = Event::Create(type);
    auto &listeners = m_EventListeners[type];
    auto it = listeners.begin();
    while (it != listeners.end()) {
        if ((*it)->OnEvent(event) == 0) {
            it = listeners.erase(it);
        } else {
            ++it;
        }
    }
    event->Release();
    m_EventStatus[type] = false;

    return true;
}

bool EventManager::SendEvent(const char *name) {
    return SendEvent(GetEventType(name));
}

bool EventManager::AddListener(EventType eventType, IEventListener *listener) {
    if (eventType >= m_EventTypes.size() || !listener)
        return false;

    if (m_EventStatus[eventType])
        return false;

    m_EventListeners[eventType].push_back(listener);
    return true;
}

bool EventManager::AddListener(const char *eventName, IEventListener *listener) {
    if (!eventName || !listener)
        return false;

    auto type = GetEventType(eventName);
    if (type == -1)
        return false;

    if (m_EventStatus[type])
        return false;

    m_EventListeners[type].push_back(listener);
    return true;
}

bool EventManager::RemoveListener(EventType eventType, IEventListener *listener) {
    if (eventType >= m_EventTypes.size() || !listener)
        return false;

    if (m_EventStatus[eventType])
        return false;

    auto &listeners = m_EventListeners[eventType];
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    return true;
}

bool EventManager::RemoveListener(const char *eventName, IEventListener *listener) {
    if (!eventName || !listener)
        return false;

    auto type = GetEventType(eventName);
    if (type == -1)
        return false;

    if (m_EventStatus[type])
        return false;

    auto &listeners = m_EventListeners[type];
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
    return true;
}

bool EventManager::RemoveAllListeners(EventType eventType) {
    if (eventType >= m_EventTypes.size())
        return false;

    if (m_EventStatus[eventType])
        return false;

    m_EventListeners[eventType].clear();
    return true;
}

bool EventManager::RemoveAllListeners(const char *eventName) {
    if (!eventName)
        return false;

    auto type = GetEventType(eventName);
    if (type == -1)
        return false;

    if (m_EventStatus[type])
        return false;

    m_EventListeners[type].clear();
    return true;
}

EventManager::EventManager() = default;

