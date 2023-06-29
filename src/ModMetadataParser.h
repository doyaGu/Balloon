#ifndef BALLOON_MODMETADATAPARSER_H
#define BALLOON_MODMETADATAPARSER_H

#include <memory>
#include <string>
#include <unordered_set>

#include "ModMetadata.h"
#include "ModDependency.h"

typedef struct yyjson_val yyjson_val;

namespace balloon {
    class ModMetadataParser final {
    public:
        static ModMetadataParser &GetInstance();

        ModMetadataParser(const ModMetadataParser &rhs) = delete;
        ModMetadataParser(ModMetadataParser &&rhs) noexcept = delete;

        ~ModMetadataParser();

        ModMetadataParser &operator=(const ModMetadataParser &rhs) = delete;
        ModMetadataParser &operator=(ModMetadataParser &&rhs) noexcept = delete;

        std::shared_ptr<ModMetadata> Parse(const char *data, size_t len);

    private:
        ModMetadataParser();

        std::shared_ptr<ModMetadata> ReadModManifest(const char *data, size_t len);
        bool VerifyModMetadata(const std::shared_ptr<ModMetadata> &metadata);

        bool ReadDependencies(yyjson_val *obj, ModDependencyType type, std::vector<std::shared_ptr<ModDependency>> &dependencies);

        std::unordered_set<std::string> m_ReservedWords;
    };

}

#endif // BALLOON_MODMETADATAPARSER_H
