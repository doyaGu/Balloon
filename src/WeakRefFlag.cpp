#include "WeakRefFlag.h"

using namespace balloon;

int WeakRefFlag::AddRef() const {
    return m_RefCount.AddRef();
}

int WeakRefFlag::Release() const {
    int r = m_RefCount.Release();
    if (r == 0) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete const_cast<WeakRefFlag *>(this);
    }
    return r;
}

bool WeakRefFlag::Get() const {
    return m_Value;
}

void WeakRefFlag::Set(bool value) {
    Lock();
    m_Value = value;
    Unlock();
}

void WeakRefFlag::Lock() const {
    m_Lock.lock();
}

void WeakRefFlag::Unlock() const {
    m_Lock.unlock();
}

WeakRefFlagFactory &WeakRefFlagFactory::GetInstance() {
    static WeakRefFlagFactory instance;
    return instance;
}

void *WeakRefFlagFactory::CreateInstance(void *data) const {
    return new WeakRefFlag();
}

void WeakRefFlagFactory::DestroyInstance(void *ptr) {
    if (ptr)
        delete static_cast<WeakRefFlag*>(ptr);
}
