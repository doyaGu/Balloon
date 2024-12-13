#ifndef NMS_EXECUTORS_NARRATIVES_H
#define NMS_EXECUTORS_NARRATIVES_H

#include "NMS/ScriptUtils.h"
#include "NMS/Guids/Narratives.h"

namespace nms {
    namespace narratives {
        inline bool LoadObject(CKBehavior *beh, XObjectArray **loadedObjects, CKObject **master, const char *file, const char *masterObjectName,
                               CK_CLASSID filterClass = CKCID_3DOBJECT, CKBOOL addToCurrentScene = TRUE,
                               CKBOOL reuseMeshes = TRUE, CKBOOL reuseMaterials = TRUE, CKBOOL dynamic = TRUE) {
            nms::SetInputParameterValue(beh, 0, file);
            nms::SetInputParameterValue(beh, 1, masterObjectName);
            nms::SetInputParameterValue(beh, 2, filterClass);
            nms::SetInputParameterValue(beh, 3, addToCurrentScene);
            nms::SetInputParameterValue(beh, 4, reuseMeshes);
            nms::SetInputParameterValue(beh, 5, reuseMaterials);
            nms::SetLocalParameterValue(beh, 0, dynamic);

            beh->ActivateInput(0);
            beh->Execute(0);

            if (loadedObjects)
                *loadedObjects = nms::GetOutputParameterValue<XObjectArray *>(beh, 0);
            if (master)
                *master = nms::GetOutputParameterObject(beh, 1);
            return beh->IsOutputActive(0) == TRUE;
        }

        inline void ActivateScript(CKBehavior *beh, CKBehavior *script, CKBOOL reset = TRUE, CKBOOL awakeObject = TRUE) {
            nms::SetInputParameterValue(beh, 0, reset);
            nms::SetInputParameterValue(beh, 1, script);
            nms::SetLocalParameterValue(beh, 0, awakeObject);

            beh->ActivateInput(0);
            beh->Execute(0);
        }

        inline void DeactivateScript(CKBehavior *beh, CKBehavior *script) {
            nms::SetInputParameterValue(beh, 0, script);

            beh->ActivateInput(0);
            beh->Execute(0);
        }
    }
}

#endif // NMS_EXECUTORS_NARRATIVES_H