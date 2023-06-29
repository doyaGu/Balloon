#include "ModCandidate.h"

#include <memory>
#include <utility>

using namespace balloon;

ModCandidate::ModCandidate() = default;

ModCandidate::ModCandidate(std::string path, std::shared_ptr<ModMetadata> metadata)
    : m_Path(std::move(path)), m_Metadata(std::move(metadata)) {}

bool ModCandidate::operator==(const ModCandidate &rhs) const {
    if (IsValid()) {
        return strcmp(m_Metadata->GetId(), rhs.m_Metadata->GetId()) == 0 &&
               m_Metadata->GetSemVer() == rhs.m_Metadata->GetSemVer();
    } else {
        return true;
    }
}

bool ModCandidate::operator!=(const ModCandidate &rhs) const {
    return !(rhs == *this);
}

bool ModCandidate::operator<(const ModCandidate &rhs) const {
    if (IsValid()) {
//        int depCmp = static_cast<int>(m_Metadata->GetDependencyCount() - rhs.m_Metadata->GetDependencyCount());
//        if (depCmp != 0)
//            return depCmp < 0;

        // Sort mod id
        int idCmp = strcmp(m_Metadata->GetId(), rhs.m_Metadata->GetId());
        if (idCmp != 0)
            return idCmp < 0;

        // Sort mod version (lower version later)
        int verCmp = rhs.m_Metadata->GetSemVer().Compare(m_Metadata->GetSemVer());
        if (verCmp != 0)
            return verCmp < 0;
    }
    return false;
}

bool ModCandidate::operator>(const ModCandidate &rhs) const {
    return rhs < *this;
}

bool ModCandidate::operator<=(const ModCandidate &rhs) const {
    return !(rhs < *this);
}

bool ModCandidate::operator>=(const ModCandidate &rhs) const {
    return !(*this < rhs);
}

bool ModCandidate::IsValid() const {
    return !m_Path.empty() && m_Metadata != nullptr;
}

const char *ModCandidate::GetId() const {
    if (!IsValid())
        return nullptr;
    return m_Metadata->GetId();
}

const char *ModCandidate::GetVersion() const {
    if (!IsValid())
        return nullptr;
    return m_Metadata->GetVersion();
}

bool ModCandidate::IsBuiltin() const {
    if (!IsValid())
        return false;
    return strcmp(m_Metadata->GetType(), "builtin") == 0;
}