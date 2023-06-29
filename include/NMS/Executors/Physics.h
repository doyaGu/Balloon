#ifndef NMS_EXECUTORS_PHYSICS_H
#define NMS_EXECUTORS_PHYSICS_H

#include "CK3dEntity.h"
#include "CKMesh.h"

#include "NMS/ScriptUtils.h"

namespace nms {
    namespace physics {
        inline void PhysicalizeParam(CKBehavior *beh, CK3dEntity *target, CKBOOL fixed, float friction, float elasticity,
                              float mass, const char *collGroup, CKBOOL startFrozen, CKBOOL enableColl,
                              CKBOOL calcMassCenter, float linearDamp, float rotDamp, const char *collSurface,
                              VxVector massCenter) {
            nms::SetTargetParameterObject(beh, target);
            nms::SetInputParameterValue(beh, 0, fixed);
            nms::SetInputParameterValue(beh, 1, friction);
            nms::SetInputParameterValue(beh, 2, elasticity);
            nms::SetInputParameterValue(beh, 3, mass);
            nms::SetInputParameterValue(beh, 4, collGroup);
            nms::SetInputParameterValue(beh, 5, startFrozen);
            nms::SetInputParameterValue(beh, 6, enableColl);
            nms::SetInputParameterValue(beh, 7, calcMassCenter);
            nms::SetInputParameterValue(beh, 8, linearDamp);
            nms::SetInputParameterValue(beh, 9, rotDamp);
            nms::SetInputParameterValue(beh, 10, collSurface);
            nms::SetLocalParameterValue(beh, 3, massCenter);
        }

        inline void PhysicalizeConvex(CKBehavior *beh, CK3dEntity *target = nullptr, CKBOOL fixed = false, float friction = 0.7f,
                               float elasticity = 0.4f, float mass = 1.0f, const char *collGroup = "",
                               CKBOOL startFrozen = false, CKBOOL enableColl = true,
                               CKBOOL calcMassCenter = false, float linearDamp = 0.1f, float rotDamp = 0.1f,
                               const char *collSurface = "", VxVector massCenter = VxVector(0, 0, 0),
                               CKMesh *mesh = nullptr) {
            PhysicalizeParam(beh, target, fixed, friction, elasticity, mass, collGroup, startFrozen,
                             enableColl, calcMassCenter, linearDamp, rotDamp, collSurface, massCenter);
            nms::SetInputParameterObject(beh, 11, mesh);
            beh->ActivateInput(0);
            beh->Execute(0);
        }

        inline void PhysicalizeBall(CKBehavior *beh, CK3dEntity *target = nullptr, CKBOOL fixed = false, float friction = 0.7f,
                             float elasticity = 0.4f, float mass = 1.0f, const char *collGroup = "",
                             CKBOOL startFrozen = false, CKBOOL enableColl = true, CKBOOL calcMassCenter = false,
                             float linearDamp = 0.1f, float rotDamp = 0.1f, const char *collSurface = "",
                             VxVector massCenter = VxVector(0, 0, 0), VxVector ballCenter = VxVector(0, 0, 0),
                             float ballRadius = 2.0f) {
            PhysicalizeParam(beh, target, fixed, friction, elasticity, mass, collGroup, startFrozen,
                             enableColl, calcMassCenter, linearDamp, rotDamp, collSurface, massCenter);
            nms::SetInputParameterValue(beh, 11, ballCenter);
            nms::SetInputParameterValue(beh, 12, ballRadius);
            beh->ActivateInput(0);
            beh->Execute(0);
        }

        inline void PhysicalizeConcave(CKBehavior *beh, CK3dEntity *target = nullptr, CKBOOL fixed = false, float friction = 0.7f,
                                float elasticity = 0.4f, float mass = 1.0f, const char *collGroup = "",
                                CKBOOL startFrozen = false, CKBOOL enableColl = true,
                                CKBOOL calcMassCenter = false, float linearDamp = 0.1f, float rotDamp = 0.1f,
                                const char *collSurface = "", VxVector massCenter = VxVector(0, 0, 0),
                                CKMesh *mesh = nullptr) {
            PhysicalizeParam(beh, target, fixed, friction, elasticity, mass, collGroup, startFrozen,
                             enableColl, calcMassCenter, linearDamp, rotDamp, collSurface, massCenter);
            nms::SetInputParameterObject(beh, 11, mesh);
            beh->ActivateInput(0);
            beh->Execute(0);
        }

        inline void Unphysicalize(CKBehavior *beh, CK3dEntity *target) {
            nms::SetTargetParameterObject(beh, target);
            beh->ActivateInput(1);
            beh->Execute(0);
        }

        inline void SetPhysicsForce(CKBehavior *beh, CK3dEntity *target = nullptr,
                             VxVector position = VxVector(0, 0, 0), CK3dEntity *posRef = nullptr,
                             VxVector direction = VxVector(0, 0, 0), CK3dEntity *directionRef = nullptr,
                             float force = 0.0f) {
            nms::SetTargetParameterObject(beh, target);
            nms::SetInputParameterValue(beh, 0, position);
            nms::SetInputParameterObject(beh, 1, posRef);
            nms::SetInputParameterValue(beh, 2, direction);
            nms::SetInputParameterObject(beh, 3, directionRef);
            nms::SetInputParameterValue(beh, 4, force);
            beh->ActivateInput(0);
            beh->Execute(0);
        }

        inline void UnsetPhysicsForce(CKBehavior *beh, CK3dEntity *target = nullptr) {
            nms::SetTargetParameterObject(beh, target);
            beh->ActivateInput(1);
            beh->Execute(0);
        }

        inline void PhysicsImpulse(CKBehavior *beh, CK3dEntity *target = nullptr,
                            VxVector position = VxVector(0, 0, 0), CK3dEntity *posRef = nullptr,
                            VxVector direction = VxVector(0, 0, 0), CK3dEntity *dirRef = nullptr,
                            float impulse = 0.0f) {
            nms::SetTargetParameterObject(beh, target);
            nms::SetInputParameterValue(beh, 0, position);
            nms::SetInputParameterObject(beh, 1, posRef);
            nms::SetInputParameterValue(beh, 2, direction);
            nms::SetInputParameterObject(beh, 3, dirRef);
            nms::SetInputParameterValue(beh, 4, impulse);
            beh->ActivateInput(0);
            beh->Execute(0);
        }

        inline void PhysicsWakeUp(CKBehavior *beh, CK3dEntity *target = nullptr) {
            nms::SetTargetParameterObject(beh, target);
            beh->ActivateInput(0);
            beh->Execute(0);
        }
    }
}

#endif // NMS_EXECUTORS_PHYSICS_H