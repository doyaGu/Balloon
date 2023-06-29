#ifndef NMS_CREATORS_PHYSICS_H
#define NMS_CREATORS_PHYSICS_H

#include "NMS/ScriptUtils.h"
#include "NMS/Guids/physics_RT.h"

namespace nms {
    namespace physics {
        inline CKBehavior *CreateBehaviorPhysicalize(CKBehavior *script, CK3dEntity *target, CKBOOL fixed, float friction, float elasticity, float mass,
                                                     const char *collGroup, CKBOOL startFrozen, CKBOOL enableColl, CKBOOL calcMassCenter,
                                                     float linearDamp, float rotDamp, const char *collSurface, VxVector massCenter) {
            CKBehavior *beh = nms::CreateBehavior(script, PHYSICS_RT_PHYSICALIZE, true);
            nms::GenerateTargetParameter(script, beh, target);
            nms::GenerateInputParameter(script, beh, 0, fixed);
            nms::GenerateInputParameter(script, beh, 1, friction);
            nms::GenerateInputParameter(script, beh, 2, elasticity);
            nms::GenerateInputParameter(script, beh, 3, mass);
            nms::GenerateInputParameter(script, beh, 4, collGroup);
            nms::GenerateInputParameter(script, beh, 5, startFrozen);
            nms::GenerateInputParameter(script, beh, 6, enableColl);
            nms::GenerateInputParameter(script, beh, 7, calcMassCenter);
            nms::GenerateInputParameter(script, beh, 8, linearDamp);
            nms::GenerateInputParameter(script, beh, 9, rotDamp);
            nms::GenerateInputParameter(script, beh, 10, collSurface);
            nms::SetLocalParameterValue(beh, 3, massCenter);
            return beh;
        }

        inline CKBehavior *CreateBehaviorPhysicalizeConvex(CKBehavior *script, CK3dEntity *target = nullptr, CKBOOL fixed = false,
                                                           float friction = 0.7f, float elasticity = 0.4f, float mass = 1.0f,
                                                           const char *collGroup = "", CKBOOL startFrozen = false,
                                                           CKBOOL enableColl = true, CKBOOL calcMassCenter = false,
                                                           float linearDamp = 0.1f, float rotDamp = 0.1f,
                                                           const char *collSurface = "", VxVector massCenter = VxVector(0, 0, 0),
                                                           CKMesh *mesh = nullptr) {
            CKBehavior *beh = CreateBehaviorPhysicalize(script, target, fixed, friction, elasticity, mass, collGroup, startFrozen,
                                                        enableColl, calcMassCenter, linearDamp, rotDamp, collSurface, massCenter);
            nms::GenerateInputParameter(script, beh, 11, mesh);
            return beh;
        }

        inline CKBehavior *CreateBehaviorPhysicalizeBall(CKBehavior *script, CK3dEntity *target = nullptr, CKBOOL fixed = false,
                                                         float friction = 0.7f, float elasticity = 0.4f, float mass = 1.0f,
                                                         const char *collGroup = "", CKBOOL startFrozen = false,
                                                         CKBOOL enableColl = true, CKBOOL calcMassCenter = false,
                                                         float linearDamp = 0.1f, float rotDamp = 0.1f,
                                                         const char *collSurface = "", VxVector massCenter = VxVector(0, 0, 0),
                                                         VxVector ballCenter = VxVector(0, 0, 0), float ballRadius = 2.0f) {
            CKBehavior *beh = CreateBehaviorPhysicalize(script, target, fixed, friction, elasticity, mass, collGroup, startFrozen,
                                                        enableColl, calcMassCenter, linearDamp, rotDamp, collSurface, massCenter);
            nms::SetLocalParameterValue(beh, 0, 0);
            nms::SetLocalParameterValue(beh, 1, 1);
            beh->CallCallbackFunction(CKM_BEHAVIORSETTINGSEDITED);
            nms::GenerateInputParameter(script, beh, 11, ballCenter);
            nms::GenerateInputParameter(script, beh, 12, ballRadius);
            return beh;
        }

        inline CKBehavior *CreateBehaviorPhysicalizeConcave(CKBehavior *script, CK3dEntity *target = nullptr, CKBOOL fixed = false,
                                                            float friction = 0.7f, float elasticity = 0.4f, float mass = 1.0f,
                                                            const char *collGroup = "", CKBOOL startFrozen = false,
                                                            CKBOOL enableColl = true, CKBOOL calcMassCenter = false,
                                                            float linearDamp = 0.1f, float rotDamp = 0.1f,
                                                            const char *collSurface = "", VxVector massCenter = VxVector(0, 0, 0),
                                                            CKMesh *mesh = nullptr) {
            CKBehavior *beh = CreateBehaviorPhysicalize(script, target, fixed, friction, elasticity, mass, collGroup, startFrozen,
                                                        enableColl, calcMassCenter, linearDamp, rotDamp, collSurface, massCenter);
            nms::SetLocalParameterValue(beh, 0, 0);
            nms::SetLocalParameterValue(beh, 2, 1);
            beh->CallCallbackFunction(CKM_BEHAVIORSETTINGSEDITED);
            nms::GenerateInputParameter(script, beh, 11, mesh);
            return beh;
        }

        inline CKBehavior *CreateBehaviorSetPhysicsForce(CKBehavior *script, CK3dEntity *target = nullptr,
                                                         VxVector position = VxVector(0, 0, 0), CK3dEntity *posRef = nullptr,
                                                         VxVector direction = VxVector(0, 0, 0), CK3dEntity *directionRef = nullptr,
                                                         float force = 0.0f) {
            CKBehavior *beh = nms::CreateBehavior(script, PHYSICS_RT_PHYSICSFORCE, true);
            nms::GenerateTargetParameter(script, beh, target);
            nms::GenerateInputParameter(script, beh, 0, position);
            nms::GenerateInputParameter(script, beh, 1, posRef);
            nms::GenerateInputParameter(script, beh, 2, direction);
            nms::GenerateInputParameter(script, beh, 3, directionRef);
            nms::GenerateInputParameter(script, beh, 4, force);
            return beh;
        }

        inline CKBehavior *CreateBehaviorPhysicsImpulse(CKBehavior *script, CK3dEntity *target = nullptr,
                                                        VxVector position = VxVector(0, 0, 0), CK3dEntity *posRef = nullptr,
                                                        VxVector direction = VxVector(0, 0, 0), CK3dEntity *dirRef = nullptr,
                                                        float impulse = 0.0f) {
            CKBehavior *beh = nms::CreateBehavior(script, PHYSICS_RT_PHYSICSIMPULSE, true);
            nms::GenerateTargetParameter(script, beh, target);
            nms::GenerateInputParameter(script, beh, 0, position);
            nms::GenerateInputParameter(script, beh, 1, posRef);
            nms::GenerateInputParameter(script, beh, 2, direction);
            nms::GenerateInputParameter(script, beh, 3, dirRef);
            nms::GenerateInputParameter(script, beh, 4, impulse);
            return beh;
        }

        inline CKBehavior *CreateBehaviorPhysicsWakeUp(CKBehavior *script, CK3dEntity *target = nullptr) {
            CKBehavior *beh = nms::CreateBehavior(script, PHYSICS_RT_PHYSICSWAKEUP, true);
            nms::GenerateTargetParameter(script, beh, target);
            return beh;
        }
    }
}

#endif // NMS_CREATORS_PHYSICS_H