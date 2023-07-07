/**
 * @file IHook.h
 * @brief The interface for hooking.
 */
#ifndef BALLOON_IHOOK_H
#define BALLOON_IHOOK_H

namespace balloon {
    inline namespace v1 {
        typedef enum HookError {
            HOOK_UNKNOWN = -1,
            HOOK_OK = 0,
            HOOK_ERROR_ALREADY_CREATED,
            HOOK_ERROR_NOT_CREATED,
            HOOK_ERROR_ENABLED,
            HOOK_ERROR_DISABLED,
            HOOK_ERROR_NOT_EXECUTABLE,
            HOOK_ERROR_UNSUPPORTED_FUNCTION,
            HOOK_ERROR_MEMORY_ALLOC,
            HOOK_ERROR_MEMORY_PROTECT,
            HOOK_ERROR_MODULE_NOT_FOUND,
            HOOK_ERROR_FUNCTION_NOT_FOUND
        } HookError;

        class IHook {
        public:
            virtual HookError CreateHook(void *target, void *detour, void **originalPtr) = 0;
            virtual HookError CreateHookApi(const char *modulePath, const char *procName, void *detour, void **originalPtr, void **targetPtr) = 0;
            virtual HookError RemoveHook(void *target) = 0;

            virtual HookError EnableHook(void *target) = 0;
            virtual HookError DisableHook(void *target) = 0;

            virtual HookError QueueEnableHook(void *target) = 0;
            virtual HookError QueueDisableHook(void *target) = 0;
            virtual HookError ApplyQueued() = 0;
        };
    }
}

#endif // BALLOON_IHOOK_H
