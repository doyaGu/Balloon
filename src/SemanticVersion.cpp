#include "SemanticVersion.h"

#include <semver.h>

#define SEMVER_MAX_LEN 256
namespace balloon {
    SemanticVersion::SemanticVersion() = default;

    SemanticVersion::SemanticVersion(const std::string &str) {
        FromString(str);
    }

    SemanticVersion::SemanticVersion(const char *str, size_t len) {
        FromString(str, len);
    }

    SemanticVersion::SemanticVersion(const SemanticVersion &rhs) {
        char buf[SEMVER_MAX_LEN];
        rhs.ToString(buf, SEMVER_MAX_LEN);
        FromString(buf, strnlen(buf, SEMVER_MAX_LEN));
    }

    SemanticVersion::SemanticVersion(SemanticVersion &&rhs) noexcept {
        memcpy(&m_SemVer, &rhs.m_SemVer, sizeof(semver_t));
        memset(&rhs.m_SemVer, 0, sizeof(semver_t));
    }

    SemanticVersion::~SemanticVersion() {
        semver_dtor(&m_SemVer);
    }

    SemanticVersion &SemanticVersion::operator=(const SemanticVersion &rhs) {
        if (*this != rhs) {
            char buf[SEMVER_MAX_LEN];
            rhs.ToString(buf, SEMVER_MAX_LEN);
            FromString(buf, strnlen(buf, SEMVER_MAX_LEN));
        }
        return *this;
    }

    SemanticVersion &SemanticVersion::operator=(SemanticVersion &&rhs) noexcept {
        if (*this != rhs) {
            memcpy(&m_SemVer, &rhs.m_SemVer, sizeof(semver_t));
            memset(&rhs.m_SemVer, 0, sizeof(semver_t));
        }
        return *this;
    }

    int SemanticVersion::Compare(const SemanticVersion &rhs) const {
        return semver_pcmp(GetSemVer(), rhs.GetSemVer());
    }

    bool SemanticVersion::Match(const std::string &str) const {
        if (str.empty())
            return false;
        return semver_comp_matchn(GetSemVer(), str.c_str(), str.size()) == 1;
    }

    bool SemanticVersion::Match(const char *str, size_t len) const {
        if (!str || len == 0)
            return false;
        return semver_comp_matchn(GetSemVer(), str, len) == 1;
    }

    bool SemanticVersion::Range(const std::string &str) const {
        if (str.empty())
            return false;
        return semver_range_matchn(GetSemVer(), str.c_str(), str.size()) == 1;
    }

    bool SemanticVersion::Range(const char *str, size_t len) const {
        if (!str || len == 0)
            return false;
        return semver_range_matchn(GetSemVer(), str, len) == 1;
    }

    bool SemanticVersion::FromString(const std::string &str) {
        if (str.empty())
            return false;
        return semvern(&m_SemVer, str.c_str(), str.size()) == 0;
    }

    bool SemanticVersion::FromString(const char *str, size_t len) {
        if (!str || len == 0)
            return false;
        return semvern(&m_SemVer, str, len) == 0;
    }

    std::string SemanticVersion::ToString() const {
        char buf[SEMVER_MAX_LEN];
        ToString(buf, SEMVER_MAX_LEN);
        return buf;
    }

    int SemanticVersion::ToString(char *buffer, size_t size) const {
        return semver_pwrite(GetSemVer(), buffer, size);
    }
}