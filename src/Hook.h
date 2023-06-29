#ifndef BALLOON_HOOK_H
#define BALLOON_HOOK_H

#include "Balloon/IHook.h"

namespace balloon {
    class Hook : public IHook {
    public:
        static Hook &GetInstance();

        Hook(const Hook &rhs) = delete;
        Hook(Hook &&rhs) noexcept = delete;

        ~Hook();

        Hook &operator=(const Hook &rhs) = delete;
        Hook &operator=(Hook &&rhs) noexcept = delete;

        bool Init();
        bool Shutdown();

        HookError CreateHook(void *target, void *detour, void **originalPtr) override;
        HookError CreateHookApi(const char *modulePath, const char *procName, void *detour, void **originalPtr, void **targetPtr) override;
        HookError RemoveHook(void *target) override;

        HookError EnableHook(void *target) override;
        HookError DisableHook(void *target) override;

        HookError QueueEnableHook(void *target) override;
        HookError QueueDisableHook(void *target) override;
        HookError ApplyQueued() override;

    private:
        static HookError StatusToErrorCode(int status);

        Hook();
    };
}

#endif // BALLOON_HOOK_H
