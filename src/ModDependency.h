#ifndef BALLOON_MODDEPENDENCY_H
#define BALLOON_MODDEPENDENCY_H

#include <string>
#include <vector>
#include <memory>

#include "Balloon/IModDependency.h"

typedef struct semver_range semver_range_t;

namespace balloon {
    class ModDependency final : public IModDependency {
    public:
        explicit ModDependency(ModDependencyType type);

        ModDependency(const ModDependency &rhs) = delete;
        ModDependency(ModDependency &&rhs) noexcept = delete;

        ~ModDependency();

        ModDependency &operator=(const ModDependency &rhs) = delete;
        ModDependency &operator=(ModDependency &&rhs) noexcept = delete;

        bool operator==(const ModDependency &rhs) const;
        bool operator!=(const ModDependency &rhs) const;

        const char *GetId() const override;
        void SetId(const char *id);

        int GetType() const override;
        void SetType(ModDependencyType type);

        bool Matches(const char *version) const override;

        const char *GetVersionRequirements() const override;
        bool AddVersionRequirement(const char *requirement);

    private:
        std::string m_Id;
        ModDependencyType m_Type;
        std::string m_VersionRequirements;
        std::shared_ptr<semver_range_t> m_VersionRange;
    };
}



#endif // BALLOON_MODDEPENDENCY_H
