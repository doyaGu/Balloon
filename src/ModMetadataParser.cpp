#include "ModMetadataParser.h"

#include <yyjson.h>

#include "Logger.h"

using namespace balloon;

ModMetadataParser &ModMetadataParser::GetInstance() {
    static ModMetadataParser instance;
    return instance;
}

ModMetadataParser::~ModMetadataParser() = default;

std::shared_ptr<ModMetadata> ModMetadataParser::Parse(const char *data, size_t len) {
    auto metadata = ReadModManifest(data, len);
    if (!metadata) {
        return nullptr;
    }

    if (!VerifyModMetadata(metadata)) {
        return nullptr;
    }

    return metadata;
}

ModMetadataParser::ModMetadataParser() : m_ReservedWords({"Balloon"}) {}

std::shared_ptr<ModMetadata> ModMetadataParser::ReadModManifest(const char *data, size_t len) {
    if (!data || len == 0)
        return nullptr;

    yyjson_read_flag flg = YYJSON_READ_ALLOW_TRAILING_COMMAS | YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_INF_AND_NAN;
    yyjson_read_err err;
    auto doc = std::shared_ptr<yyjson_doc>(yyjson_read_opts(const_cast<char *>(data), len, flg, nullptr, &err),
                                           [](yyjson_doc *doc) { yyjson_doc_free(doc); });
    if (!doc) {
        LOG_ERROR("Failed to parse \"balloon.mod.json\": %s", err.msg);
        return nullptr;
    }

    yyjson_val *root = yyjson_doc_get_root(doc.get());
    if (!root) {
        LOG_ERROR("Unreachable error: yyjson_doc_get_root() return null.");
        return nullptr;
    }

    yyjson_val *schVer = yyjson_obj_get(root, "schemaVersion");
    if (schVer && yyjson_get_int(schVer) != 1) {
        LOG_ERROR("Unsupported mod schema version %i found, stop manifest parsing.", yyjson_get_int(schVer));
        return nullptr;
    }

    auto metadata = std::make_shared<ModMetadata>();

    yyjson_val *key, *val;
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(root, &iter);
    while ((key = yyjson_obj_iter_next(&iter))) {
        const char *name = yyjson_get_str(key);
        val = yyjson_obj_iter_get_val(key);

        if (strcmp(name, "id") == 0) {
            if (!yyjson_is_str(val)) {
                LOG_ERROR("Mod Id must be a string.");
                return nullptr;
            }
            const char *id = yyjson_get_str(val);
            if (m_ReservedWords.find(id) != m_ReservedWords.end()) {
                LOG_ERROR("Mod Id can not be a reserved identifier.");
                return nullptr;
            }
            if (!metadata->SetId(id)) {
                LOG_ERROR("Mod Id is not valid.");
                return nullptr;
            }
        } else if (strcmp(name, "version") == 0) {
            if (!yyjson_is_str(val)) {
                LOG_ERROR("Mod Version must be a string.");
                return nullptr;
            }
            if (!metadata->SetVersion(yyjson_get_str(val))) {
                LOG_ERROR("Mod Version is not valid.");
                return nullptr;
            }
        } else if (strcmp(name, "type") == 0) {
            if (yyjson_is_str(val)) {
                metadata->SetType(yyjson_get_str(val));
            } else {
                LOG_WARN("Mod Type must be a string.");
            }
        } else if (strcmp(name, "name") == 0) {
            if (yyjson_is_str(val)) {
                metadata->SetName(yyjson_get_str(val));
            } else {
                LOG_WARN("Mod Name must be a string.");
            }
        } else if (strcmp(name, "description") == 0) {
            if (yyjson_is_str(val)) {
                metadata->SetDescription(yyjson_get_str(val));
            } else {
                LOG_WARN("Mod Description must be a string.");
            }
        } else if (strcmp(name, "authors") == 0) {
            if (yyjson_is_str(val)) {
                metadata->AddAuthor(yyjson_get_str(val));
            } else if (yyjson_is_arr(val)) {
                yyjson_val *item;
                yyjson_arr_iter ait = yyjson_arr_iter_with(val);
                while ((item = yyjson_arr_iter_next(&ait))) {
                    if (yyjson_is_str(item)) {
                        metadata->AddAuthor(yyjson_get_str(item));
                    } else {
                        LOG_WARN("Mod Author must be a string.");
                    }
                }
                LOG_WARN("Mod Authors must be a string or an array of string.");
            }
        } else if (strcmp(name, "contributors") == 0) {
            if (yyjson_is_str(val)) {
                metadata->AddContributor(yyjson_get_str(val));
            } else if (yyjson_is_arr(val)) {
                yyjson_val *item;
                yyjson_arr_iter ait = yyjson_arr_iter_with(val);
                while ((item = yyjson_arr_iter_next(&ait))) {
                    if (yyjson_is_str(item)) {
                        metadata->AddContributor(yyjson_get_str(item));
                    } else {
                        LOG_WARN("Mod Contributor must be a string.");
                    }
                }
                LOG_WARN("Mod Contributors must be a string or an array of string.");
            }
        } else if (strcmp(name, "homepage") == 0) {
            if (yyjson_is_str(val)) {
                metadata->SetHomepage(yyjson_get_str(val));
            } else {
                LOG_WARN("Mod Homepage must be a string.");
            }
        } else if (strcmp(name, "repository") == 0) {
            if (yyjson_is_str(val)) {
                metadata->SetRepository(yyjson_get_str(val));
            } else {
                LOG_WARN("Mod Repository must be a string.");
            }
        } else if (strcmp(name, "license") == 0) {
            if (yyjson_is_str(val)) {
                metadata->SetLicense(yyjson_get_str(val));
            } else {
                LOG_WARN("Mod License must be a string.");
            }
        } else if (strcmp(name, "keywords") == 0) {
            if (yyjson_is_str(val)) {
                metadata->AddKeyword(yyjson_get_str(val));
            } else if (yyjson_is_arr(val)) {
                yyjson_val *item;
                yyjson_arr_iter ait = yyjson_arr_iter_with(val);
                while ((item = yyjson_arr_iter_next(&ait))) {
                    if (yyjson_is_str(item)) {
                        metadata->AddKeyword(yyjson_get_str(item));
                    } else {
                        LOG_WARN("Mod Keyword must be a string.");
                    }
                }
                LOG_WARN("Mod Keywords must be a string or an array of string.");
            }
        } else if (strcmp(name, "categories") == 0) {
            if (yyjson_is_str(val)) {
                metadata->AddCategory(yyjson_get_str(val));
            } else if (yyjson_is_arr(val)) {
                yyjson_val *item;
                yyjson_arr_iter ait = yyjson_arr_iter_with(val);
                while ((item = yyjson_arr_iter_next(&ait))) {
                    if (yyjson_is_str(item)) {
                        metadata->AddCategory(yyjson_get_str(item));
                    } else {
                        LOG_WARN("Mod Category must be a string.");
                    }
                }
                LOG_WARN("Mod Categories must be a string or an array of string.");
            }
        } else if (strcmp(name, "depends") == 0) {
            if (yyjson_is_obj(val)) {
                std::vector<std::shared_ptr<ModDependency>> dependencies;
                ReadDependencies(val, MDT_DEPEND, dependencies);
                metadata->AddDependencies(dependencies);
            } else {
                LOG_ERROR("Mod Dependencies must be an object.");
                return nullptr;
            }
        } else if (strcmp(name, "breaks") == 0) {
            if (yyjson_is_obj(val)) {
                std::vector<std::shared_ptr<ModDependency>> dependencies;
                ReadDependencies(val, MDT_BREAK, dependencies);
                metadata->AddDependencies(dependencies);
            } else {
                LOG_ERROR("Mod Dependencies must be an object.");
                return nullptr;
            }
        } else if (strcmp(name, "conflicts") == 0) {
            if (yyjson_is_obj(val)) {
                std::vector<std::shared_ptr<ModDependency>> dependencies;
                ReadDependencies(val, MDT_CONFLICT, dependencies);
                metadata->AddDependencies(dependencies);
            } else {
                LOG_ERROR("Mod Dependencies must be an object.");
                return nullptr;
            }
        } else if (strcmp(name, "recommends") == 0) {
            if (yyjson_is_obj(val)) {
                std::vector<std::shared_ptr<ModDependency>> dependencies;
                ReadDependencies(val, MDT_RECOMMEND, dependencies);
                metadata->AddDependencies(dependencies);
            } else {
                LOG_ERROR("Mod Dependencies must be an object.");
                return nullptr;
            }
        } else if (strcmp(name, "suggests") == 0) {
            if (yyjson_is_obj(val)) {
                std::vector<std::shared_ptr<ModDependency>> dependencies;
                ReadDependencies(val, MDT_SUGGEST, dependencies);
                metadata->AddDependencies(dependencies);
            } else {
                LOG_ERROR("Mod Dependencies must be an object.");
                return nullptr;
            }
        }
    }

    return metadata;
}

bool ModMetadataParser::ReadDependencies(yyjson_val *obj, ModDependencyType type, std::vector<std::shared_ptr<ModDependency>> &dependencies) {
    if (!yyjson_is_obj(obj))
        return false;

    if (type > MDT_BREAK)
        return false;

    yyjson_val *key, *val;
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(obj, &iter);
    while ((key = yyjson_obj_iter_next(&iter))) {
        const char *id = yyjson_get_str(key);
        val = yyjson_obj_iter_get_val(key);
        auto dep = std::make_shared<ModDependency>(type);
        dep->SetId(id);

        if (yyjson_is_str(val)) {
            dep->AddVersionRequirement(yyjson_get_str(val));
        } else if (yyjson_is_arr(val)) {
            yyjson_val *item;
            yyjson_arr_iter ait = yyjson_arr_iter_with(val);
            while ((item = yyjson_arr_iter_next(&ait))) {
                if (yyjson_is_str(item)) {
                    dep->AddVersionRequirement(yyjson_get_str(item));
                } else {
                    LOG_WARN("Mod Dependency Version Requirement must be a string.");
                }
            }
            LOG_WARN("Mod Dependency Version Requirements must be a string or an array of string.");
        }

        if (strnlen(dep->GetVersionRequirements(), 1) != 0) {
            dependencies.push_back(dep);
        }
    }
    return true;
}

bool ModMetadataParser::VerifyModMetadata(const std::shared_ptr<ModMetadata> &metadata) {
    if (strcmp(metadata->GetId(), "") == 0) {
        LOG_ERROR("Mod Id is not provided.");
        return false;
    }
    return true;
}

