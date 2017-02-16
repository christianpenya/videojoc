#pragma once

#ifndef _ENGINE_PHYSXMANAGER_20170215_H
#define _ENGINE_PHYSXMANAGER_20170215_H

#include <PxPhysicsAPI.h>
#include <map>
#include <vector>
#include "Math/Quaternion.h"

namespace physx
{
class PxFoundation;
class PxPhysics;
class PxDefaultCpuDispatcher;
class PxScene;
class PxCooking;
class PxMaterial;
class PxActor;
class PxRigidActor;
class PxControllerManager;
class PxController;
class PxJoint;
class PxMaterial;

namespace debugger
{
namespace comm
{
class PvdConnection;
}
}

typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
}

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

const float PHYSX_UPDATE_STEP = 0.017f;

class CPhysXManager
{
    struct CharacterControllerData
    {
        Vect3f position;
        Vect3f linearVelocity;
    };

    struct RaycastData
    {
        Vect3f position;
        Vect3f normal;
        float distance;
        std::string actor;
    };

protected:
    CPhysXManager();
public:
    static CPhysXManager* CreatePhysXManager();
    virtual ~CPhysXManager();

protected:

    physx::PxFoundation							*m_Foundation;
    physx::PxPhysics							*m_PhysX;

#	if USE_PHYSX_DEBUG
    physx::PxVisualDebuggerConnection		*m_DebugConnection;
#	endif

    physx::PxDefaultCpuDispatcher				*m_Dispatcher;
    physx::PxScene								*m_Scene;
    physx::PxCooking							*m_Cooking;
    physx::PxControllerManager					*m_ControllerManager;

    std::map<std::string, size_t>				m_ActorIndexs;
    std::vector<std::string>					m_ActorNames;
    std::vector<Vect3f>							m_ActorPositions;
    std::vector<Quatf>							m_ActorOrientations;
    std::vector<physx::PxActor*>				m_Actors;

    Vect3f GetActorPosition(const std::string& actorName);
    Quatf GetActorOrientation(const std::string& actorName);

    std::map<std::string, physx::PxMaterial*> m_Materials;
    std::map<std::string, physx::PxController*> m_CharacterControllers;

    float m_LeftoverSeconds;

    //void Update(float _dt);
    void RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution);
    void CreateStaticObject(std::string aMaterialName, const Quatf orientation, const Vect3f position, float sizeX, float sizeY, float sizeZ, size_t index);
    void CreateDynamicObject(std::string aMaterialName, const Quatf orientation, const Vect3f position, float sizeX, float sizeY, float sizeZ, size_t index, physx::PxReal density);
    void CreatePlane(std::string aMaterialName, float x, float y, float z, float d, size_t index);
    void CreateShape(std::string aMaterialName, const Quatf orientation, const Vect3f position);
    void Update(float _dt);
    void AddTriggerBox(const std::string& actorName, float sizeX, float sizeY, float sizeZ, const Vect3f& position, const Quatf& orientation);

    CharacterControllerData MoveCharacterController(const std::string& characterControllerName, const Vect3f& movement, float elapsedTime);
    bool Raycast(const Vect3f& origin, const Vect3f& end, int filterMask, RaycastData* result_ = nullptr);
    void AddFixedJoint(const std::string& jointName, const std::string& actor1Name, const std::string& actor2Name);
    void DeleteActor(std::string actorName, size_t index);
};

#endif //_ENGINE_RENDERMANAGER_CPB_2016110320428_H
