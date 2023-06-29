#include "Event.h"

using namespace balloon;

Event *Event::Create(EventType type) {
    return new Event(type);
}

Event::~Event() = default;

int Event::AddRef() const {
    return m_RefCount.AddRef();
}

int Event::Release() const {
    int r = m_RefCount.Release();
    if (r == 0) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete const_cast<Event *>(this);
    }
    return r;
}

EventType Event::GetType() const {
    return m_Type;
}

void Event::SetType(EventType type) {
    m_Type = type;
}

int Event::GetFlag() const {
    return m_Flag;
}

void Event::SetFlag(int flag) {
    m_Flag = flag;
}

IDataStack *Event::GetDataStack() const {
    return m_DataStack;
}

void Event::SetDataStack(IDataStack *stack) {
    m_DataStack = stack;
}

Event::Event(EventType type) : m_Type(type), m_Flag(0) {}