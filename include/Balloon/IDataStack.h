/**
 * @file IDataStack.h
 * @brief The interface of data stack.
 */
#ifndef BALLOON_IDATASTACK_H
#define BALLOON_IDATASTACK_H

#include <cstdint>

#include "Balloon/IWeakRefFlag.h"

namespace balloon {
    inline namespace v1 {

#define DATA_TYPE_NONE        ((uint8_t)0)        /* _____000 */
#define DATA_TYPE_BOOL        ((uint8_t)1)        /* _____001 */
#define DATA_TYPE_CHAR        ((uint8_t)2)        /* _____010 */
#define DATA_TYPE_NUM         ((uint8_t)3)        /* _____011 */
#define DATA_TYPE_STR         ((uint8_t)4)        /* _____100 */
#define DATA_TYPE_BUF         ((uint8_t)5)        /* _____101 */
#define DATA_TYPE_PTR         ((uint8_t)6)        /* _____110 */

#define DATA_SUBTYPE_NONE     ((uint8_t)(0 << 3)) /* _0000___ */
#define DATA_SUBTYPE_FALSE    ((uint8_t)(0 << 3)) /* _0000___ */
#define DATA_SUBTYPE_TRUE     ((uint8_t)(1 << 3)) /* _0001___ */
#define DATA_SUBTYPE_UINT8    ((uint8_t)(0 << 3)) /* _0000___ */
#define DATA_SUBTYPE_INT8     ((uint8_t)(1 << 3)) /* _0001___ */
#define DATA_SUBTYPE_UINT16   ((uint8_t)(2 << 3)) /* _0010___ */
#define DATA_SUBTYPE_INT16    ((uint8_t)(3 << 3)) /* _0011___ */
#define DATA_SUBTYPE_UINT32   ((uint8_t)(4 << 3)) /* _0100___ */
#define DATA_SUBTYPE_INT32    ((uint8_t)(5 << 3)) /* _0101___ */
#define DATA_SUBTYPE_UINT64   ((uint8_t)(6 << 3)) /* _0110___ */
#define DATA_SUBTYPE_INT64    ((uint8_t)(7 << 3)) /* _0111___ */
#define DATA_SUBTYPE_FLOAT32  ((uint8_t)(8 << 3)) /* _1000___ */
#define DATA_SUBTYPE_FLOAT64  ((uint8_t)(9 << 3)) /* _1001___ */

        class IDataStack {
        public:
            virtual int AddRef() const = 0;
            virtual int Release() const = 0;
            virtual IWeakRefFlag *GetWeakRefFlag() const = 0;

            virtual void Clear() = 0;

            virtual bool Empty() const = 0;
            virtual size_t Size() const = 0;

            virtual size_t Capacity() const = 0;
            virtual void Reserve(size_t cap) = 0;
            virtual void Shrink() = 0;

            virtual size_t CursorTo(int offset) const = 0;
            virtual size_t GetCursor() const = 0;
            virtual bool SetCursor(size_t index) = 0;

            virtual void PushCursor() = 0;
            virtual void PopCursor() = 0;

            virtual void Push() = 0;
            virtual void Pop() = 0;

            size_t Top() const {
                auto size = Size();
                return size == 0 ? 0 : Size() - 1;
            }

            uint8_t GetTag() const { return GetTag(Top()); }
            uint8_t GetType() const { return GetType(Top()); }
            uint8_t GetSubtype() const { return GetSubtype(Top()); }

            size_t GetSize() const { return GetSize(Top()); }

            bool GetBool() const { return GetBool(Top()); }
            char GetChar() const { return GetChar(Top()); }
            uint8_t GetUint8() const { return GetUint8(Top()); }
            int8_t GetInt8() const { return GetInt8(Top()); }
            uint16_t GetUint16() const { return GetUint16(Top()); }
            int16_t GetInt16() const { return GetInt16(Top()); }
            uint32_t GetUint32() const { return GetUint32(Top()); }
            int32_t GetInt32() const { return GetInt32(Top()); }
            uint64_t GetUint64() const { return GetUint64(Top()); }
            int64_t GetInt64() const { return GetInt64(Top()); }
            float GetFloat32() const { return GetFloat32(Top()); }
            double GetFloat64() const { return GetFloat64(Top()); }
            const char *GetString() const { return GetString(Top()); }
            const void *GetBuffer(size_t *size) const { return GetBuffer(Top(), size); }
            void *GetPtr() const { return GetPtr(Top()); }

            void SetValue(bool value) { SetValue(Top(), value); }
            void SetValue(char value) { SetValue(Top(), value); }
            void SetValue(uint8_t value) { SetValue(Top(), value); }
            void SetValue(int8_t value) { SetValue(Top(), value); }
            void SetValue(uint16_t value) { SetValue(Top(), value); }
            void SetValue(int16_t value) { SetValue(Top(), value); }
            void SetValue(uint32_t value) { SetValue(Top(), value); }
            void SetValue(int32_t value) { SetValue(Top(), value); }
            void SetValue(uint64_t value) { SetValue(Top(), value); }
            void SetValue(int64_t value) { SetValue(Top(), value); }
            void SetValue(float value) { SetValue(Top(), value); }
            void SetValue(double value) { SetValue(Top(), value); }
            void SetValue(const void *buf, size_t size) { SetValue(Top(), buf, size); }
            void SetValue(void *ptr) { SetValue(Top(), ptr); }

            virtual uint8_t GetTag(size_t index) const = 0;
            virtual uint8_t GetType(size_t index) const = 0;
            virtual uint8_t GetSubtype(size_t index) const = 0;

            virtual size_t GetSize(size_t index) const = 0;

            virtual bool GetBool(size_t index) const = 0;
            virtual char GetChar(size_t index) const = 0;
            virtual uint8_t GetUint8(size_t index) const = 0;
            virtual int8_t GetInt8(size_t index) const = 0;
            virtual uint16_t GetUint16(size_t index) const = 0;
            virtual int16_t GetInt16(size_t index) const = 0;
            virtual uint32_t GetUint32(size_t index) const = 0;
            virtual int32_t GetInt32(size_t index) const = 0;
            virtual uint64_t GetUint64(size_t index) const = 0;
            virtual int64_t GetInt64(size_t index) const = 0;
            virtual float GetFloat32(size_t index) const = 0;
            virtual double GetFloat64(size_t index) const = 0;
            virtual const char *GetString(size_t index) const = 0;
            virtual const void *GetBuffer(size_t index, size_t *size) const = 0;
            virtual void *GetPtr(size_t index) const = 0;

            virtual void SetValue(size_t index, bool value) = 0;
            virtual void SetValue(size_t index, char value) = 0;
            virtual void SetValue(size_t index, uint8_t value) = 0;
            virtual void SetValue(size_t index, int8_t value) = 0;
            virtual void SetValue(size_t index, uint16_t value) = 0;
            virtual void SetValue(size_t index, int16_t value) = 0;
            virtual void SetValue(size_t index, uint32_t value) = 0;
            virtual void SetValue(size_t index, int32_t value) = 0;
            virtual void SetValue(size_t index, uint64_t value) = 0;
            virtual void SetValue(size_t index, int64_t value) = 0;
            virtual void SetValue(size_t index, float value) = 0;
            virtual void SetValue(size_t index, double value) = 0;
            virtual void SetValue(size_t index, const void *buf, size_t size) = 0;
            virtual void SetValue(size_t index, void *ptr) = 0;

            virtual void Swap(size_t index1, size_t index2) = 0;

            virtual bool Copy(const IDataStack *other) = 0;
            virtual IDataStack *Clone() = 0;

            virtual bool Merge(const IDataStack *other) = 0;
            virtual IDataStack *Separate() = 0;

        protected:
            virtual ~IDataStack() = default;
        };
    }
}

#endif // BALLOON_IDATASTACK_H
