#include "DataStack.h"

#include <cassert>

using namespace balloon;

std::mutex DataStack::m_RWLock;

DataStack::DataStack() {
    m_Cursors.push_back(0);
}

DataStack::DataStack(const DataStack &rhs) : m_Data(rhs.m_Data), m_Cursors(rhs.m_Cursors) {}

DataStack::~DataStack() {
    if (m_WeakRefFlag) {
        m_WeakRefFlag->Release();
        m_WeakRefFlag = nullptr;
    }
}

int DataStack::AddRef() const {
    return m_RefCount.AddRef();
}

int DataStack::Release() const {
    int r = m_RefCount.Release();
    if (r == 0) {
        if (m_WeakRefFlag)
            m_WeakRefFlag->Set(true);
        std::atomic_thread_fence(std::memory_order_acquire);
        delete const_cast<DataStack *>(this);
    }
    return r;
}

IWeakRefFlag *DataStack::GetWeakRefFlag() const {
    if (m_WeakRefFlag)
        return m_WeakRefFlag;

    std::lock_guard<std::mutex> guard(m_RWLock);

    m_WeakRefFlag = new WeakRefFlag;
    return m_WeakRefFlag;
}

void DataStack::Clear() {
    m_Data.clear();
}

bool DataStack::Empty() const {
    return m_Data.empty();
}

size_t DataStack::Size() const {
    return m_Data.size();
}

size_t DataStack::Capacity() const {
    return m_Data.capacity();
}

void DataStack::Reserve(size_t cap) {
    m_Data.reserve(cap);
}

void DataStack::Shrink() {
    m_Data.shrink_to_fit();
}

size_t DataStack::CursorTo(int offset) const {
    size_t index = m_Cursors.back() + offset;
    if (index <= 0)
        return 0;
    else if (index >= m_Data.size())
        return m_Data.size() - 1;
    else
        return index;
}

size_t DataStack::GetCursor() const {
    return m_Cursors.back();
}

bool DataStack::SetCursor(size_t index) {
    if (index >= m_Data.size())
        return false;

    m_Cursors.back() = index;
    return true;
}

void DataStack::PushCursor() {
    m_Cursors.push_back(0);
}

void DataStack::PopCursor() {
    m_Cursors.pop_back();
}

void DataStack::Push() {
    m_Data.emplace_back();
}

void DataStack::Pop() {
    m_Data.pop_back();
}

uint8_t DataStack::GetTag(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetTag();
}

uint8_t DataStack::GetType(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetType();
}

uint8_t DataStack::GetSubtype(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetSubtype();
}

size_t DataStack::GetSize(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetSize();
}

bool DataStack::GetBool(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetBool();
}

char DataStack::GetChar(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetChar();
}

uint8_t DataStack::GetUint8(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetUint8();
}

int8_t DataStack::GetInt8(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetInt8();
}

uint16_t DataStack::GetUint16(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetUint16();
}

int16_t DataStack::GetInt16(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetInt16();
}

uint32_t DataStack::GetUint32(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetUint32();
}

int32_t DataStack::GetInt32(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetInt32();
}

uint64_t DataStack::GetUint64(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetUint64();
}

int64_t DataStack::GetInt64(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetInt64();
}

float DataStack::GetFloat32(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetFloat32();
}

double DataStack::GetFloat64(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetFloat64();
}

const char *DataStack::GetString(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetString();
}

const void *DataStack::GetBuffer(size_t index, size_t *size) const {
    assert(index < m_Data.size());
    return m_Data[index].GetBuffer();
}

void *DataStack::GetPtr(size_t index) const {
    assert(index < m_Data.size());
    return m_Data[index].GetPtr();
}

void DataStack::SetValue(size_t index, bool value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, char value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, uint8_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, int8_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, uint16_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, int16_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, uint32_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, int32_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, uint64_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, int64_t value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, float value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, double value) {
    assert(index < m_Data.size());
    m_Data[index] = value;
}

void DataStack::SetValue(size_t index, const void *buf, size_t size) {
    assert(index < m_Data.size());
    m_Data[index].SetBuffer(buf, size);
}

void DataStack::SetValue(size_t index, void *ptr) {
    assert(index < m_Data.size());
    m_Data[index] = ptr;
}

void DataStack::Swap(size_t index1, size_t index2) {
    assert(index1 < m_Data.size());
    assert(index2 < m_Data.size());
    std::swap(m_Data[index1], m_Data[index2]);
}

bool DataStack::Copy(const IDataStack *other) {
    if (!other)
        return false;

    const auto *ds = reinterpret_cast<const DataStack *>(other);
    m_Data = ds->m_Data;
    m_Cursors = ds->m_Cursors;
    return true;
}

IDataStack *DataStack::Clone() {
    return new DataStack(*this);
}

bool DataStack::Merge(const IDataStack *other) {
    if (!other)
        return false;

    const auto *ds = reinterpret_cast<const DataStack *>(other);

    for (auto cursor: ds->m_Cursors) {
        m_Cursors.push_back(cursor + (m_Data.size() - 1));
    }

    for (auto &data: ds->m_Data) {
        m_Data.emplace_back(data);
    }
    return true;
}

IDataStack *DataStack::Separate() {
    size_t cursor = GetCursor();

    if (cursor == 0) {
        auto *ret = Clone();
        Clear();
        return ret;
    }

    auto *ds = new DataStack;
    std::copy(m_Data.begin() + cursor, m_Data.end(), ds->m_Data.begin());
    m_Data.erase(m_Data.begin() + cursor, m_Data.end());
    m_Cursors.clear();
    m_Cursors.push_back(0);
    return ds;
}

DataStackFactory &DataStackFactory::GetInstance() {
    static DataStackFactory instance;
    return instance;
}

void *DataStackFactory::CreateInstance(void *data) const {
    return new DataStack();
}

void DataStackFactory::DestroyInstance(void *ptr) {
    if (ptr)
        delete static_cast<DataStack *>(ptr);
}