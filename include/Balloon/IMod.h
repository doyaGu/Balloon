/**
 * @file IMod.h
 * @brief The interface of mod.
 */
#ifndef BALLOON_IMOD_H
#define BALLOON_IMOD_H

#include <cstdint>

namespace balloon {
    inline namespace v1 {
        class IModContext;

        /**
         * @enum ModFlags
         * @brief All flags for mod
         */
        typedef enum ModFlags {
            MOD_HAS_ONUPDATE = 0x00000001, /**< Mod has OnUpdate() */
            MOD_HAS_ONLATEUPDATE = 0x00000002, /**< Mod has OnLateUpdate() */

            // Internal Flags
            MOD_FIXED = 0x00010000, /**< Mod can not be removed */
            MOD_INITIALIZED = 0x00020000, /**< Mod has been initialized */
            MOD_CONNECTED = 0x00040000, /**< Mod is connected to game */

            MOD_LOGGER_RETRIEVED = 0x00100000, /**< Mod logger has been retrieved */
            MOD_CONFIG_RETRIEVED = 0x00200000, /**< Mod config has been retrieved */
        } ModFlags;

        /**
         * @interface IMod
         * @brief The interface of mod
         *
         * Any valid mod class must implement this interface.
         *
         * A mod class will only be instantiated once by the mod loader.
         */
        class IMod {
        public:
            /**
             * @brief Initialize mod and acquire mod context.
             * @param[in] context The mod context that provides all kinds of functions.
             * @param[out] flags The flags used for setting mod.
             * @return true on success, or false on error.
             * @note
             * Mod can not access any API provided by mod loader until this method is called.
             * This method will be called after CKContext is initialized.
             * At this point, most of managers are available except CKRenderManager,
             * and CKRenderContext which created by CKRenderManager is not ready.
             * No game objects and scripts are loaded at this moment.
             */
            virtual bool Init(IModContext *context, uint32_t *flags) { return true; }

            /**
             * @brief Shutdown mod and mark it as removable
             * @note
             * This method will be called before CKContext is destroyed.
             * At this point, all CKObjects are already released.
             */
            virtual void Shutdown() {}

            /**
             * @brief Connect to the game.
             * @return true on success, or false on error.
             * @note
             * This method will be called after CKContext are reset.
             * At this point, essential game objects and scripts are loaded,
             * but scripts are not activated.
             */
            virtual bool Connect() { return true; }

            /**
             * @brief Disconnect to the game.
             * @note
             * This method will be called before CKContext is about to clear all objects.
             */
            virtual void Disconnect() {}

            /**
             * @brief Update mod logic every game loop
             * @note
             * This method will be called after scripts are executed in each game loop.
             * This method is optional. If required, mod should set flag MOD_HAS_ONUPDATE in Init().
             */
            virtual void OnUpdate() {}

            /**
             * @brief Update mod logic lately every game loop
             * @note
             * This method will be called after all mods execute OnUpdate() in each game loop.
             * This method is optional. If required, mod should set flag MOD_HAS_ONLATEUPDATE in Init().
             */
            virtual void OnLateUpdate() {}
        };
    }
}

#endif // BALLOON_IMOD_H