#ifndef CK2_CK2_H
#define CK2_CK2_H

#include "CKAll.h"

namespace ck2 {
    inline CKContext *GetCKContext() {
        CKContext *context = ::GetCKContext(0);
        assert(context != nullptr);
        return context;
    }

    inline CKRenderContext *GetRenderContext() {
        return GetCKContext()->GetPlayerRenderContext();
    }

    inline CKLevel *GetCurrentLevel() {
        return GetCKContext()->GetCurrentLevel();
    }

    inline CKScene *GetCurrentScene() {
        return GetCKContext()->GetCurrentScene();
    }

    inline CKParameterManager *GetParameterManager() {
        return GetCKContext()->GetParameterManager();
    }

    inline CKAttributeManager *GetAttributeManager() {
        return GetCKContext()->GetAttributeManager();
    }

    inline CKTimeManager *GetTimeManager() {
        return GetCKContext()->GetTimeManager();
    }

    inline CKMessageManager *GetMessageManager() {
        return GetCKContext()->GetMessageManager();
    }

    inline CKBehaviorManager *GetBehaviorManager() {
        return GetCKContext()->GetBehaviorManager();
    }

    inline CKPathManager *GetPathManager() {
        return GetCKContext()->GetPathManager();
    }

    inline CKRenderManager *GetRenderManager() {
        return GetCKContext()->GetRenderManager();
    }

    inline CKSoundManager *GetSoundManager() {
        return (CKSoundManager *)GetCKContext()->GetManagerByGuid(SOUND_MANAGER_GUID);
    }

    inline CKInputManager *GetInputManager() {
        return (CKInputManager *)GetCKContext()->GetManagerByGuid(INPUT_MANAGER_GUID);
    }

    inline CKCollisionManager *GetCollisionManager() {
        return (CKCollisionManager *)GetCKContext()->GetManagerByGuid(COLLISION_MANAGER_GUID);
    }

    inline CKFloorManager *GetFloorManager() {
        return (CKFloorManager *) GetCKContext()->GetManagerByGuid(FLOOR_MANAGER_GUID);
    }

    inline CKDataArray *GetArrayByName(const char *name) {
        return (CKDataArray *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_DATAARRAY);
    }

    inline CKGroup *GetGroupByName(const char *name) {
        return (CKGroup *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_GROUP);
    }

    inline CKMaterial *GetMaterialByName(const char *name) {
        return (CKMaterial *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_MATERIAL);
    }

    inline CKMesh *GetMeshByName(const char *name) {
        return (CKMesh *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_MESH);
    }

    inline CK2dEntity *Get2dEntityByName(const char *name) {
        return (CK2dEntity *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_2DENTITY);
    }

    inline CK3dEntity *Get3dEntityByName(const char *name) {
        return (CK3dEntity *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_3DENTITY);
    }

    inline CK3dObject *Get3dObjectByName(const char *name) {
        return (CK3dObject *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_3DOBJECT);
    }

    inline CKCamera *GetCameraByName(const char *name) {
        return (CKCamera *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_CAMERA);
    }

    inline CKTargetCamera *GetTargetCameraByName(const char *name) {
        return (CKTargetCamera *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_TARGETCAMERA);
    }

    inline CKLight *GetLightByName(const char *name) {
        return (CKLight *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_LIGHT);
    }

    inline CKTargetLight *GetTargetLightByName(const char *name) {
        return (CKTargetLight *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_TARGETLIGHT);
    }

    inline CKSound *GetSoundByName(const char *name) {
        return (CKSound *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_SOUND);
    }

    inline CKTexture *GetTextureByName(const char *name) {
        return (CKTexture *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_TEXTURE);
    }

    inline CKBehavior *GetScriptByName(const char *name) {
        return (CKBehavior *)GetCKContext()->GetObjectByNameAndClass(const_cast<CKSTRING>(name), CKCID_BEHAVIOR);
    }

    inline void SetIC(CKBeObject *obj, bool hierarchy = false) {
        GetCurrentScene()->SetObjectInitialValue(obj, CKSaveObjectState(obj));

        if (hierarchy) {
            if (CKIsChildClassOf(obj, CKCID_2DENTITY)) {
                auto *entity = (CK2dEntity *) obj;
                for (int i = 0; i < entity->GetChildrenCount(); i++)
                    SetIC(entity->GetChild(i), true);
            }
            if (CKIsChildClassOf(obj, CKCID_3DENTITY)) {
                auto *entity = (CK3dEntity *) obj;
                for (int i = 0; i < entity->GetChildrenCount(); i++)
                    SetIC(entity->GetChild(i), true);
            }
        }
    }

    inline void RestoreIC(CKBeObject *obj, bool hierarchy = false) {
        CKStateChunk *chunk = GetCurrentScene()->GetObjectInitialValue(obj);
        if (chunk)
            CKReadObjectState(obj, chunk);

        if (hierarchy) {
            if (CKIsChildClassOf(obj, CKCID_2DENTITY)) {
                auto *entity = (CK2dEntity *) obj;
                for (int i = 0; i < entity->GetChildrenCount(); i++)
                    RestoreIC(entity->GetChild(i), true);
            }
            if (CKIsChildClassOf(obj, CKCID_3DENTITY)) {
                auto *entity = (CK3dEntity *) obj;
                for (int i = 0; i < entity->GetChildrenCount(); i++)
                    RestoreIC(entity->GetChild(i), true);
            }
        }
    }

    inline void Show(CKBeObject *obj, CK_OBJECT_SHOWOPTION show, bool hierarchy = false) {
        obj->Show(show);

        if (hierarchy) {
            if (CKIsChildClassOf(obj, CKCID_2DENTITY)) {
                auto *entity = (CK2dEntity *) obj;
                for (int i = 0; i < entity->GetChildrenCount(); i++)
                    Show(entity->GetChild(i), show, true);
            }
            if (CKIsChildClassOf(obj, CKCID_3DENTITY)) {
                auto *entity = (CK3dEntity *) obj;
                for (int i = 0; i < entity->GetChildrenCount(); i++)
                    Show(entity->GetChild(i), show, true);
            }
        }
    }
}

#endif // CK2_CK2_H
