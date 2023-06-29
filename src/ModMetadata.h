#ifndef BALLOON_MODMETADATA_H
#define BALLOON_MODMETADATA_H

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Balloon/IModMetadata.h"
#include "Balloon/DataBox.h"

#include "ModDependency.h"
#include "SemanticVersion.h"

namespace balloon {
    class ModMetadata final : public IModMetadata {
    public:
        ModMetadata();

        ModMetadata(const ModMetadata &rhs) = delete;
        ModMetadata(ModMetadata &&rhs) noexcept = delete;

        ~ModMetadata();

        ModMetadata &operator=(const ModMetadata &rhs) = delete;
        ModMetadata &operator=(ModMetadata &&rhs) noexcept = delete;

        const char *GetId() const override;
        bool SetId(const char *id);

        const char *GetVersion() const override;
        const SemanticVersion &GetSemVer() const;
        bool SetVersion(const char *version);

        const char *GetType() const override;
        void SetType(const char *type);

        size_t GetDependencyCount() const override;
        IModDependency *GetDependencyByIndex(size_t index) const override;

        std::vector<std::shared_ptr<ModDependency>> GetDependencies() const;
        void AddDependencies(const std::vector<std::shared_ptr<ModDependency>> &dependencies);

        const char *GetName() const override;
        void SetName(const char *name);

        const char *GetDescription() const override;
        void SetDescription(const char *desc);

        size_t GetAuthorCount() const override;
        const char *GetAuthorByIndex(size_t index) const override;
        void AddAuthor(const char *author);

        size_t GetContributorCount() const override;
        const char *GetContributorByIndex(size_t index) const override;
        void AddContributor(const char *contributor);

        const char *GetHomepage() const override;
        void SetHomepage(const char *homepage);

        const char *GetRepository() const override;
        void SetRepository(const char *repo);

        const char *GetLicense() const override;
        void SetLicense(const char *lic);

        size_t GetKeywordCount() const override;
        const char *GetKeywordByIndex(size_t index) const override;
        void AddKeyword(const char *keyword);

        size_t GetCategoryCount() const override;
        const char *GetCategoryByIndex(size_t index) const override;
        void AddCategory(const char *category);

        void *GetUserData(size_t type) const override;
        void *SetUserData(void *data, size_t type) override;
    private:
        std::string m_Id;
        std::string m_Version;
        SemanticVersion m_SemVer;
        std::string m_Type;

        std::vector<std::shared_ptr<ModDependency>> m_Dependencies;

        std::string m_Name;
        std::string m_Description;
        std::vector<std::string> m_Authors;
        std::vector<std::string> m_Contributors;
        std::string m_Homepage;
        std::string m_Repository;
        std::string m_License;
        std::vector<std::string> m_Keywords;
        std::vector<std::string> m_Categories;

        DataBox m_UserData;
    };
}

#endif // BALLOON_MODINFO_H
