#ifndef BALLOON_DATASTACK_H
#define BALLOON_DATASTACK_H

#include <mutex>
#include <string>
#include <vector>

#include "Balloon/IDataStack.h"
#include "Balloon/RefCount.h"
#include "Balloon/IObjectFactory.h"
#include "WeakRefFlag.h"
#include "Variant.h"

namespace balloon {
    class DataStack final : public IDataStack {
    public:
        DataStack();

        DataStack(const DataStack &rhs);
        DataStack(DataStack &&rhs) noexcept = delete;

        ~DataStack() override;

        DataStack &operator=(const DataStack &rhs) = delete;
        DataStack &operator=(DataStack &&rhs) noexcept = delete;

        int AddRef() const override;
        int Release() const override;
        IWeakRefFlag *GetWeakRefFlag() const override;

        void Clear() override;
        bool Empty() const override;

        size_t Size() const override;
        size_t Capacity() const override;
        void Reserve(size_t cap) override;
        void Shrink() override;

        size_t CursorTo(int offset) const override;
        size_t GetCursor() const override;
        bool SetCursor(size_t index) override;

        void PushCursor() override;
        void PopCursor() override;

        void Push() override;
        void Pop() override;

        uint8_t GetTag(size_t index) const override;
        uint8_t GetType(size_t index) const override;
        uint8_t GetSubtype(size_t index) const override;

        size_t GetSize(size_t index) const override;

        bool GetBool(size_t index) const override;
        char GetChar(size_t index) const override;
        uint8_t GetUint8(size_t index) const override;
        int8_t GetInt8(size_t index) const override;
        uint16_t GetUint16(size_t index) const override;
        int16_t GetInt16(size_t index) const override;
        uint32_t GetUint32(size_t index) const override;
        int32_t GetInt32(size_t index) const override;
        uint64_t GetUint64(size_t index) const override;
        int64_t GetInt64(size_t index) const override;
        float GetFloat32(size_t index) const override;
        double GetFloat64(size_t index) const override;
        const char *GetString(size_t index) const override;
        const void *GetBuffer(size_t index, size_t *size) const override;
        void *GetPtr(size_t index) const override;

        void SetValue(size_t index, bool value) override;
        void SetValue(size_t index, char value) override;
        void SetValue(size_t index, uint8_t value) override;
        void SetValue(size_t index, int8_t value) override;
        void SetValue(size_t index, uint16_t value) override;
        void SetValue(size_t index, int16_t value) override;
        void SetValue(size_t index, uint32_t value) override;
        void SetValue(size_t index, int32_t value) override;
        void SetValue(size_t index, uint64_t value) override;
        void SetValue(size_t index, int64_t value) override;
        void SetValue(size_t index, float value) override;
        void SetValue(size_t index, double value) override;
        void SetValue(size_t index, const void *buf, size_t size) override;
        void SetValue(size_t index, void *ptr) override;

        void Swap(size_t index1, size_t index2) override;

        bool Copy(const IDataStack *other) override;
        IDataStack *Clone() override;

        bool Merge(const IDataStack *other) override;
        IDataStack *Separate() override;

    private:
        mutable RefCount m_RefCount;
        mutable WeakRefFlag *m_WeakRefFlag = nullptr;

        std::vector<Variant> m_Data;
        std::vector<size_t> m_Cursors;

        static std::mutex m_RWLock;
    };

    class DataStackFactory : public IObjectFactory {
    public:
        static DataStackFactory &GetInstance();
        void *CreateInstance(void *data) const override;
        void DestroyInstance(void *ptr) override;
    };
}

#endif // BALLOON_DATASTACK_H
