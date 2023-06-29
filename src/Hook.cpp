#include "Hook.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <MinHook.h>

using namespace balloon;

Hook &Hook::GetInstance() {
    static Hook instance;
    return instance;
}

Hook::~Hook() = default;

bool Hook::Init() {
    return MH_Initialize() == MH_OK;
}

bool Hook::Shutdown() {
    return MH_Uninitialize() == MH_OK;
}

HookError Hook::CreateHook(void *target, void *detour, void **originalPtr) {
    return StatusToErrorCode(MH_CreateHook(target, detour, originalPtr));
}

HookError Hook::CreateHookApi(const char *modulePath, const char *procName, void *detour, void **originalPtr, void **targetPtr) {
    HMODULE hModule;

    if (modulePath) {
        int size = ::MultiByteToWideChar(CP_UTF8, 0, modulePath, -1, nullptr, 0);
        if (size == 0)
            return HOOK_ERROR_MODULE_NOT_FOUND;

        auto ws = new wchar_t[size];
        ::MultiByteToWideChar(CP_UTF8, 0, modulePath, -1, ws, size);

        hModule = ::GetModuleHandleW(ws);
        delete[] ws;
        if (!hModule)
            return HOOK_ERROR_MODULE_NOT_FOUND;
    } else {
        hModule = GetModuleHandle(nullptr);
    }

    void *target = reinterpret_cast<void *>(::GetProcAddress(hModule, procName));
    if (!target)
        return HOOK_ERROR_FUNCTION_NOT_FOUND;

    if (targetPtr)
        *targetPtr = target;

    return StatusToErrorCode(MH_CreateHook(target, detour, originalPtr));
}

HookError Hook::RemoveHook(void *target) {
    return StatusToErrorCode(MH_RemoveHook(target));
}

HookError Hook::EnableHook(void *target) {
    return StatusToErrorCode(MH_EnableHook(target));
}

HookError Hook::DisableHook(void *target) {
    return StatusToErrorCode(MH_DisableHook(target));
}

HookError Hook::QueueEnableHook(void *target) {
    return StatusToErrorCode(MH_QueueEnableHook(target));
}

HookError Hook::QueueDisableHook(void *target) {
    return StatusToErrorCode(MH_QueueDisableHook(target));
}

HookError Hook::ApplyQueued() {
    return StatusToErrorCode(MH_ApplyQueued());
}

HookError Hook::StatusToErrorCode(int status) {
    switch (status) {
        case MH_UNKNOWN:
            return HOOK_UNKNOWN;
        case MH_OK:
            return HOOK_OK;
        case MH_ERROR_ALREADY_CREATED:
            return HOOK_ERROR_ALREADY_CREATED;
        case MH_ERROR_NOT_CREATED:
            return HOOK_ERROR_NOT_CREATED;
        case MH_ERROR_ENABLED:
            return HOOK_ERROR_ENABLED;
        case MH_ERROR_DISABLED:
            return HOOK_ERROR_DISABLED;
        case MH_ERROR_NOT_EXECUTABLE:
            return HOOK_ERROR_NOT_EXECUTABLE;
        case MH_ERROR_UNSUPPORTED_FUNCTION:
            return HOOK_ERROR_UNSUPPORTED_FUNCTION;
        case MH_ERROR_MEMORY_ALLOC:
            return HOOK_ERROR_MEMORY_ALLOC;
        case MH_ERROR_MEMORY_PROTECT:
            return HOOK_ERROR_MEMORY_PROTECT;
        case MH_ERROR_MODULE_NOT_FOUND:
            return HOOK_ERROR_MODULE_NOT_FOUND;
        case MH_ERROR_FUNCTION_NOT_FOUND:
            return HOOK_ERROR_FUNCTION_NOT_FOUND;
        default:
            break;
    }
    return HOOK_UNKNOWN;
}

Hook::Hook() = default;
