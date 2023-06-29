#ifndef BALLOON_SEMANTICVERSION_H
#define BALLOON_SEMANTICVERSION_H

#include <cstdint>
#include <string>

#include <semver.h>

namespace balloon {
    class SemanticVersion {
    public:
        SemanticVersion();
        explicit SemanticVersion(const std::string &str);
        SemanticVersion(const char *str, size_t len);

        SemanticVersion(const SemanticVersion &rhs);
        SemanticVersion(SemanticVersion &&rhs) noexcept;

        ~SemanticVersion();

        SemanticVersion& operator=(const SemanticVersion &rhs);
        SemanticVersion& operator=(SemanticVersion &&rhs) noexcept;

        int Major() const { return m_SemVer.major; }
        int Minor() const { return m_SemVer.minor; }
        int Patch() const { return m_SemVer.patch; }
        const char *Prerelease() const { return m_SemVer.prerelease.raw; }
        const char *Build() const { return m_SemVer.build.raw; }

        int Compare(const SemanticVersion &rhs) const;

        bool Match(const std::string &str) const;
        bool Match(const char *str, size_t len) const;

        bool Range(const std::string &str) const;
        bool Range(const char *str, size_t len) const;

        bool FromString(const std::string &str);
        bool FromString(const char *str, size_t len);

        std::string ToString() const;
        int ToString(char *buffer, size_t size) const;

        const semver_t *GetSemVer() const { return &m_SemVer; }

    private:
        semver_t m_SemVer = {};
    };

    inline bool operator==(const SemanticVersion &lhs, const SemanticVersion &rhs) {
        return lhs.Compare(rhs) == 0;
    }

    inline bool operator!=(const SemanticVersion &lhs, const SemanticVersion &rhs) {
        return lhs.Compare(rhs) != 0;
    }

    inline bool operator<(const SemanticVersion &lhs, const SemanticVersion &rhs) {
        return lhs.Compare(rhs) < 0;
    }

    inline bool operator>(const SemanticVersion &lhs, const SemanticVersion &rhs) {
        return lhs.Compare(rhs) > 0;
    }

    inline bool operator<=(const SemanticVersion &lhs, const SemanticVersion &rhs) {
        return lhs.Compare(rhs) <= 0;
    }

    inline bool operator>=(const SemanticVersion &lhs, const SemanticVersion &rhs) {
        return lhs.Compare(rhs) >= 0;
    }
}

#endif // BALLOON_SEMANTICVERSION_H
