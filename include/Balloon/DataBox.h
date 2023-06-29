/**
 * @file DataBox.h
 * @brief The class implements data storage box.
 */
#ifndef BALLOON_DATABOX_H
#define BALLOON_DATABOX_H

#include <mutex>
#include <vector>

namespace balloon {
    class DataBox {
    public:
        void *GetData(size_t type) const {
            std::lock_guard<std::mutex> guard(m_RWLock);

            const auto n = m_UserData.size();
            for (size_t i = 0; i < n; i += 2) {
                if (m_UserData[i] == type)
                    return reinterpret_cast<void *>(m_UserData[i + 1]);
            }
            return nullptr;
        }

        void *SetData(void *data, size_t type) {
            std::lock_guard<std::mutex> guard(m_RWLock);

            const auto n = m_UserData.size();
            for (size_t i = 0; i < n; i += 2) {
                if (m_UserData[i] == type) {
                    void *oldData = reinterpret_cast<void *>(m_UserData[i + 1]);
                    m_UserData[i + 1] = reinterpret_cast<size_t>(data);
                    return oldData;
                }
            }

            m_UserData.push_back(type);
            m_UserData.push_back(reinterpret_cast<size_t>(data));
            return nullptr;
        }

    private:
        mutable std::mutex m_RWLock;
        std::vector<size_t> m_UserData;
    };
}

#endif // BALLOON_DATABOX_H
