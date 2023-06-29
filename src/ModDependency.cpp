#include "ModDependency.h"

#include <semver.h>

using namespace balloon;

ModDependency::ModDependency(ModDependencyType type) : m_Type(type) {}

ModDependency::~ModDependency() = default;

bool ModDependency::operator==(const ModDependency &rhs) const {
    return m_Id == rhs.m_Id &&
           m_Type == rhs.m_Type &&
           m_VersionRequirements == rhs.m_VersionRequirements;
}

bool ModDependency::operator!=(const ModDependency &rhs) const {
    return !(rhs == *this);
}

const char *ModDependency::GetId() const {
    return m_Id.c_str();
}

void ModDependency::SetId(const char *id) {
    m_Id = (id) ? id : "";
}

int ModDependency::GetType() const {
    return m_Type;
}

void ModDependency::SetType(ModDependencyType type) {
    m_Type = type;
}

bool ModDependency::Matches(const char *version) const {
    if (!version || strlen(version) == 0)
        return false;

    auto ver = std::shared_ptr<semver_t>(new semver_t, [](semver_t *ver) { semver_dtor(ver); });
    if (semver(ver.get(), version))
        return false;

    return semver_range_pmatch(ver.get(), m_VersionRange.get());
}

const char *ModDependency::GetVersionRequirements() const {
    return m_VersionRequirements.c_str();
}

bool ModDependency::AddVersionRequirement(const char *requirement) {
    if (!requirement)
        return false;

    size_t len = strlen(requirement);
    if (len == 0) return false;

    if (!m_VersionRange) {
        m_VersionRange = std::shared_ptr<semver_range_t>(new semver_range_t, [](semver_range_t* range) { semver_range_dtor(range); });
        if (semver_rangen(m_VersionRange.get(), requirement, len)) {
            return false;
        }
    } else {
        if (semver_or(m_VersionRange.get(), requirement, len)) {
            return false;
        }
    }

    char buffer[2048];
    semver_range_pwrite(m_VersionRange.get(), buffer, 1024);
    m_VersionRequirements = buffer;
    return true;
}