#ifndef BALLOON_WEAKREFFLAG_H
#define BALLOON_WEAKREFFLAG_H

#include <atomic>
#include <mutex>

#include "Balloon/IWeakRefFlag.h"
#include "Balloon/RefCount.h"
#include "Balloon/IObjectFactory.h"

namespace balloon {
    class WeakRefFlag final : public IWeakRefFlag {
    public:
        WeakRefFlag() { AddRef(); }
        ~WeakRefFlag() override = default;

        int AddRef() const override;
        int Release() const override;

        bool Get() const override;
        void Set(bool value) override;

        void Lock() const override;
        void Unlock() const override;

    private:
        mutable std::mutex m_Lock;
        mutable RefCount m_RefCount;
        bool m_Value = false;
    };

    class WeakRefFlagFactory : public IObjectFactory {
    public:
        static WeakRefFlagFactory &GetInstance();

        void *CreateInstance(void *data) const override;
        void DestroyInstance(void *ptr) override;
    };
}

#endif // BALLOON_WEAKREFFLAG_H
