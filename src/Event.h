#ifndef BALLOON_EVENT_H
#define BALLOON_EVENT_H

#include "Balloon/IEvent.h"
#include "Balloon/RefCount.h"

namespace balloon {
    class EventManager;

    class Event final : public IEvent {
    public:
        static Event *Create(EventType type);

        Event(const Event &rhs) = delete;
        Event(Event &&rhs) noexcept = delete;

        ~Event() override;

        Event &operator=(const Event &rhs) = delete;
        Event &operator=(Event &&rhs) noexcept = delete;

        int AddRef() const override;
        int Release() const override;

        EventType GetType() const override;
        void SetType(EventType type) override;

        int GetFlag() const override;
        void SetFlag(int flag) override;

        IDataStack *GetDataStack() const override;

        void SetDataStack(IDataStack *stack) override;

    private:
        explicit Event(EventType type);

        mutable RefCount m_RefCount;

        EventType m_Type;
        int m_Flag;

        IDataStack *m_DataStack = nullptr;
    };
}


#endif // BALLOON_EVENT_H
