/**
 * @file Register.h
 * @brief Register mod
 * @warning This header should only be included once
 */
#ifndef BALLOON_REGISTER_H
#define BALLOON_REGISTER_H

#include "Balloon/ModInfo.h"

extern "C" __declspec(dllexport) void GetModInfo(
    const void *inputInfo,
    const void **const outputInfo,
    int *apiVersion,
    size_t *infoSize,
    size_t *infoAlign) {

    static balloon::ModInfo modInfo;

    if (inputInfo) {
        modInfo = *static_cast<const balloon::ModInfo *>(inputInfo);
    } else if (outputInfo) {
        if (!apiVersion || !infoSize || !infoAlign) {
            return;
        }

        bool agreement = true;
        if (*apiVersion != balloon::INFO_API_VERSION) {
            agreement = false;
        }
        if (*infoSize != sizeof(balloon::ModInfo)) {
            agreement = false;
        }
        if (*infoAlign != alignof(balloon::ModInfo)) {
            agreement = false;
        }

        *apiVersion = balloon::INFO_API_VERSION;
        *infoSize = sizeof(balloon::ModInfo);
        *infoAlign = alignof(balloon::ModInfo);

        if (!agreement) {
            return;
        }

        *outputInfo = &modInfo;
    }
}

namespace balloon {
    namespace detail {
        template<typename ModClass>
        struct Registrar {
            static void Register(const char *id, const char *version) {
                static_assert(std::is_base_of<balloon::IMod, ModClass>::value,
                              "Can not register a mod which doesn't inherit from IMod.");
                ModInfo info = {id, version, Factory, Deleter};
                GetModInfo(&info, nullptr, nullptr, nullptr, nullptr);
            }

            static IMod *Factory() { return static_cast<IMod *>(new ModClass); }
            static void Deleter(IMod *ptr) { delete static_cast<ModClass *>(ptr); }
        };
    }
}

#define BALLOON_ADD_MOD(ModClass, ModId, ModVersion) \
    namespace balloon { \
        namespace { \
            struct ExecuteOnLoading { \
                ExecuteOnLoading() { \
                    ::balloon::detail::Registrar<ModClass>::Register(ModId, ModVersion); \
                } \
            }; \
            static ExecuteOnLoading execute; \
        } \
    }

#endif // BALLOON_REGISTER_H
