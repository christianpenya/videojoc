#pragma once

#ifndef _ENGINE_PHYSXMANAGERIMPLEMENTATION_20170215_H
#define _ENGINE_PHYSXMANAGERIMPLEMENTATION_20170215_H

#include "PhysxManager.h"
#include "PxSimulationEventCallback.h"
#include "characterkinematic/PxController.h"
#include "Math/Quaternion.h"

class CPhysXManagerImplementation :
    public CPhysXManager,
    public physx::PxSimulationEventCallback,
    public physx::PxUserControllerHitReport
{
public:
    CPhysXManagerImplementation();
    ~CPhysXManagerImplementation();

    void AddCharacterController(const std::string& actorName, float height, float radius, const Vect3f& position, const Quatf& orientation, const std::string& material, float density);

    // PxSimulationEventCallback
    void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {}
    void onWake(physx::PxActor** actors, physx::PxU32 count) {}
    void onSleep(physx::PxActor** actors, physx::PxU32 count) {}
    void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {}
    void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);

    // PxUserControllerHitReport
    void onShapeHit(const physx::PxControllerShapeHit& hit) {};
    void onControllerHit(const physx::PxControllersHit& hit) {};
    void onObstacleHit(const physx::PxControllerObstacleHit& hit) {};
};

#endif //_ENGINE_PHYSXMANAGERIMPLEMENTATION_20170215_H
