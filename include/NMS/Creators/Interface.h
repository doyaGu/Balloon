#ifndef NMS_CREATORS_INTERFACE_H
#define NMS_CREATORS_INTERFACE_H

#include "NMS/ScriptUtils.h"
#include "NMS/Guids/Interface.h"

namespace nms {
    namespace interface {
        inline CKBehavior *Create2DText(CKBehavior *script, CK2dEntity *target = nullptr, int font = 0, const char *text = "",
                                            int align = 0, VxRect margin = {2, 2, 2, 2},
                                            Vx2DVector offset = {0, 0}, Vx2DVector pindent = {0, 0},
                                            CKMaterial *bgmat = nullptr,float caretsize = 0.1f,
                                            CKMaterial *caretmat = nullptr, int flags = 1) {
            CKBehavior *beh = nms::CreateBehavior(script, VT_INTERFACE_2DTEXT, true);
            nms::SetTargetParameterObject(beh, target);
            nms::GenerateInputParameter(script, beh, 0, font);
            nms::GenerateInputParameter(script, beh, 1, text);
            nms::GenerateInputParameter(script, beh, 2, align);
            nms::GenerateInputParameter(script, beh, 3, margin);
            nms::GenerateInputParameter(script, beh, 4, offset);
            nms::GenerateInputParameter(script, beh, 5, pindent);
            nms::GenerateInputParameterObject(script, beh, 6, bgmat);
            nms::GenerateInputParameter(script, beh, 7, caretsize);
            nms::GenerateInputParameterObject(script, beh, 8, caretmat);
            nms::SetLocalParameterValue(beh, 0, flags);

            return beh;
        }
    }
}

#endif // NMS_CREATORS_INTERFACE_H