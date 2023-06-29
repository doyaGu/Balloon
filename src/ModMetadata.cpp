#include "ModMetadata.h"

using namespace balloon;

ModMetadata::ModMetadata() = default;

ModMetadata::~ModMetadata() = default;

const char *ModMetadata::GetId() const {
    return m_Id.c_str();
}

bool ModMetadata::SetId(const char *id) {
    if (!id || strnlen(id, 1) == 0)
        return false;

    m_Id = id;
    return true;
}

const char *ModMetadata::GetVersion() const {
    return m_Version.c_str();
}

const SemanticVersion &ModMetadata::GetSemVer() const {
    return m_SemVer;
}

bool ModMetadata::SetVersion(const char *version) {
    if (!version)
        return false;

    size_t len = strlen(version);
    if (len == 0)
        return false;

    if (!m_SemVer.FromString(version, len))
        return false;

    m_Version = m_SemVer.ToString();
    return true;
}

const char *ModMetadata::GetType() const {
    return m_Type.c_str();
}

void ModMetadata::SetType(const char *type) {
    m_Type = (type) ? type : "";
}

size_t ModMetadata::GetDependencyCount() const {
    return m_Dependencies.size();
}

IModDependency *ModMetadata::GetDependencyByIndex(size_t index) const {
    return m_Dependencies[index].get();
}

std::vector<std::shared_ptr<ModDependency>> ModMetadata::GetDependencies() const {
    return m_Dependencies;
}

void ModMetadata::AddDependencies(const std::vector<std::shared_ptr<ModDependency>> &dependencies) {
    for (const auto &dep: dependencies) {
        m_Dependencies.push_back(dep);
    }
}

const char *ModMetadata::GetName() const {
    return m_Name.c_str();
}

void ModMetadata::SetName(const char *name) {
    m_Name = (name) ? name : "";
}

const char *ModMetadata::GetDescription() const {
    return m_Description.c_str();
}

void ModMetadata::SetDescription(const char *desc) {
    m_Description = (desc) ? desc : "";
}

size_t ModMetadata::GetAuthorCount() const {
    return m_Authors.size();
}

const char *ModMetadata::GetAuthorByIndex(size_t index) const {
    if (index >= m_Authors.size())
        return nullptr;
    return m_Authors[index].c_str();
}

void ModMetadata::AddAuthor(const char *author) {
    m_Authors.emplace_back((author) ? author : "");
}

size_t ModMetadata::GetContributorCount() const {
    return m_Contributors.size();
}

const char *ModMetadata::GetContributorByIndex(size_t index) const {
    if (index >= m_Contributors.size())
        return nullptr;
    return m_Contributors[index].c_str();
}

void ModMetadata::AddContributor(const char *contributor) {
    m_Contributors.emplace_back((contributor) ? contributor : "");
}

const char *ModMetadata::GetHomepage() const {
    return m_Homepage.c_str();
}

void ModMetadata::SetHomepage(const char *homepage) {
    m_Homepage = (homepage) ? homepage : "";
}

const char *ModMetadata::GetRepository() const {
    return m_Repository.c_str();
}

void ModMetadata::SetRepository(const char *repo) {
    m_Repository = (repo) ? repo : "";
}

const char *ModMetadata::GetLicense() const {
    return m_License.c_str();
}

void ModMetadata::SetLicense(const char *lic) {
    m_License = (lic) ? lic : "";
}

size_t ModMetadata::GetKeywordCount() const {
    return m_Keywords.size();
}

const char *ModMetadata::GetKeywordByIndex(size_t index) const {
    if (index >= m_Keywords.size())
        return nullptr;
    return m_Keywords[index].c_str();
}

void ModMetadata::AddKeyword(const char *keyword) {
    m_Keywords.emplace_back((keyword) ? keyword : "");
}

size_t ModMetadata::GetCategoryCount() const {
    return m_Categories.size();
}

const char *ModMetadata::GetCategoryByIndex(size_t index) const {
    if (index >= m_Categories.size())
        return nullptr;
    return m_Categories[index].c_str();
}

void ModMetadata::AddCategory(const char *category) {
    m_Categories.emplace_back((category) ? category : "");
}

void *ModMetadata::GetUserData(size_t type) const {
    return m_UserData.GetData(type);
}

void *ModMetadata::SetUserData(void *data, size_t type) {
    return m_UserData.SetData(data, type);
}