#ifndef NMS_CREATORS_NARRATIVES_H
#define NMS_CREATORS_NARRATIVES_H

#include "NMS/ScriptUtils.h"
#include "NMS/Guids/Narratives.h"

namespace nms {
    namespace narratives {
        inline CKBehavior *CreateBehaviorObjectLoader(CKBehavior *script, const char *file, const char *masterObjectName,
                                                      CK_CLASSID filterClass = CKCID_3DOBJECT, CKBOOL addToCurrentScene = TRUE,
                                                      CKBOOL reuseMeshes = TRUE, CKBOOL reuseMaterials = TRUE, CKBOOL dynamic = TRUE) {
            CKBehavior *beh = nms::CreateBehavior(script, VT_NARRATIVES_OBJECTLOAD, false);
            nms::GenerateInputParameter(script, beh, 0, file);
            nms::GenerateInputParameter(script, beh, 1, masterObjectName);
            nms::GenerateInputParameter(script, beh, 2, filterClass);
            nms::GenerateInputParameter(script, beh, 3, addToCurrentScene);
            nms::GenerateInputParameter(script, beh, 4, reuseMeshes);
            nms::GenerateInputParameter(script, beh, 5, reuseMaterials);
            nms::SetLocalParameterValue(beh, 0, dynamic);
            return beh;
        }
    }
}

#endif // NMS_CREATORS_NARRATIVES_H