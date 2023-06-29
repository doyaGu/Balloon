/**
 * @file ModInfo.h
 * @brief The struct for mod registration.
 */
#ifndef BALLOON_MODINFO_H
#define BALLOON_MODINFO_H

namespace balloon {
    constexpr int INFO_API_VERSION = 1;

    inline namespace v1 {
        class IModContext;
        class IMod;

        typedef IMod *(*ModFactoryFunction)();
        typedef void (*ModDeleterFunction)(IMod *);

        /**
         * @struct ModInfo
         * @brief The struct for mod registration.
         * @warning This struct is for internal use only.
         */
        typedef struct ModInfo {
            const char *id; /**< Mod Id */
            const char *version; /**< Mod Version */
            ModFactoryFunction factory; /**< Mod Factory */
            ModDeleterFunction deleter; /**< Mod Deleter */
        } ModInfo;
    }
}

#endif // BALLOON_MODINFO_H
