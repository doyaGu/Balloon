#include "ModContainer.h"

#include <cassert>
#include <utility>

using namespace balloon;

std::shared_ptr<ModContainer> ModContainer::Create(const std::string &rootPath, const std::string &sourcePath, const std::shared_ptr<ModMetadata> &metadata, const std::shared_ptr<void> &library, const ModInfo *info) {
    if (rootPath.empty() || sourcePath.empty() || !metadata || !library || !info)
        return nullptr;
    return std::shared_ptr<ModContainer>(new ModContainer(rootPath, sourcePath, metadata, library, info));
}

ModContainer::~ModContainer() = default;

const char *ModContainer::GetId() const {
    return m_Metadata->GetId();
}

const char *ModContainer::GetVersion() const {
    return m_Metadata->GetVersion();
}

uint32_t ModContainer::GetFlags() const {
    return m_Flags;
}

void ModContainer::SetFlags(uint32_t add, uint32_t remove) {
    m_Flags |= add;
    m_Flags &= ~remove;
}

IModMetadata *ModContainer::GetMetadata() const {
    return m_Metadata.get();
}

void *ModContainer::GetLibrary() const {
    return m_Library.get();
}

const char *ModContainer::GetRootPath() const {
    return m_RootPath.c_str();
}

const char *ModContainer::GetSourcePath() const {
    return m_SourcePath.c_str();
}

bool ModContainer::IsInstantiated() const {
    return m_Instance != nullptr;
}

bool ModContainer::Instantiate() {
    if (!m_Info->factory || !m_Info->deleter)
        return false;

    auto factory = m_Info->factory;
    auto deleter = m_Info->deleter;

    m_Instance = ModPtr(factory(), [deleter](IMod *ptr) { if (ptr) deleter(ptr); });

    return true;
}

IMod *ModContainer::GetInstance() const {
    if (!m_Instance)
        return nullptr;

    return m_Instance.get();
}

void ModContainer::DestroyInstance() {
    m_Instance = nullptr;
}

void *ModContainer::GetUserData(size_t type) const {
    return m_UserData.GetData(type);
}

void *ModContainer::SetUserData(void *data, size_t type) {
    return m_UserData.SetData(data, type);
}

ModContainer::ModContainer(std::string rootPath, std::string sourcePath, const std::shared_ptr<ModMetadata> &metadata, const std::shared_ptr<void> &library, const ModInfo *info)
    : m_RootPath(std::move(rootPath)), m_SourcePath(std::move(sourcePath)), m_Metadata(metadata), m_Library(library), m_Info(info), m_Flags(0) {}
