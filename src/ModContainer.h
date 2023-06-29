#ifndef BALLOON_MODCONTAINER_H
#define BALLOON_MODCONTAINER_H

#include <string>
#include <memory>
#include <functional>

#include "Balloon/IModContainer.h"
#include "Balloon/IMod.h"
#include "Balloon/ModInfo.h"
#include "Balloon/DataBox.h"
#include "ModMetadata.h"

namespace balloon {
    class ModContainer final : public IModContainer {
    public:
        static std::shared_ptr<ModContainer> Create(const std::string &path, const std::string &realPath, const std::shared_ptr<ModMetadata> &metadata, const std::shared_ptr<void> &library, const ModInfo *info);

        ModContainer(const ModContainer &rhs) = delete;
        ModContainer(ModContainer &&rhs) noexcept = delete;

        ~ModContainer();

        ModContainer &operator=(const ModContainer &rhs) = delete;
        ModContainer &operator=(ModContainer &&rhs) noexcept = delete;

        const char *GetId() const override;
        const char *GetVersion() const override;

        uint32_t GetFlags() const override;
        void SetFlags(uint32_t add, uint32_t remove);

        IModMetadata *GetMetadata() const override;
        void *GetLibrary() const override;

        const char *GetRootPath() const override;
        const char *GetSourcePath() const override;

        bool IsInstantiated() const;
        bool Instantiate();
        IMod *GetInstance() const;
        void DestroyInstance();

        void *GetUserData(size_t type) const override;
        void *SetUserData(void *data, size_t type) override;

    private:
        ModContainer(std::string rootPath, std::string sourcePath, const std::shared_ptr<ModMetadata> &metadata, const std::shared_ptr<void> &library, const ModInfo *info);

        std::string m_RootPath;
        std::string m_SourcePath;
        std::shared_ptr<ModMetadata> m_Metadata;
        std::shared_ptr<void> m_Library;
        const ModInfo *m_Info;
        uint32_t m_Flags;

        typedef std::unique_ptr<IMod, std::function<void(IMod *)>> ModPtr;
        ModPtr m_Instance;

        DataBox m_UserData;
    };

}


#endif // BALLOON_MODCONTAINER_H
