#ifndef NMS_CREATORS_HOOKS_H
#define NMS_CREATORS_HOOKS_H

#include "NMS/ScriptUtils.h"
#include "NMS/Guids/Hooks.h"

namespace nms {
    namespace hooks {
        typedef int (*CKBehaviorCallback)(const CKBehaviorContext *behcontext, void *arg);

        inline CKBehavior *CreateHookBlock(CKBehavior *script, CKBehaviorCallback callback, void *arg = nullptr, int inCount = 1, int outCount = 1) {
            CKBehavior *beh = CreateBehavior(script, HOOKS_HOOKBLOCK_GUID, false);
            beh->SetLocalParameterValue(0, &callback);
            beh->SetLocalParameterValue(1, &arg);

            XString pinName = "In ";
            for (int i = 0; i < inCount; i++) {
                beh->CreateInput((pinName << i).Str());
            }

            XString poutName = "Out ";
            for (int i = 0; i < outCount; i++) {
                beh->CreateOutput((poutName << i).Str());
            }

            return beh;
        }
    }
}

#endif // NMS_CREATORS_HOOKS_H