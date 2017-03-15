#pragma once

#ifndef _ENGINE_PHYSXMANAGER_20170215_H
#define _ENGINE_PHYSXMANAGER_20170215_H

#include <PxPhysicsAPI.h>
#include <map>
#include <vector>
#include "Math/Quaternion.h"
#include "Utils/MemLeaks/MemLeaks.h"

#define USE_PHYSX_DEBUG 1
#define PVD_HOST "127.0.0.1"

using namespace physx;

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

namespace debugger
{
namespace comm
{
class PvdConnection;
}
}

typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;

}

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
    virtual ~CPhysXManager();
    static CPhysXManager* CreatePhysXManager();

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

    physx::PxTransform GetActorTransform(const std::string& actorName) const;
    size_t GetActorSize(const std::string& actorName);
    size_t GetActorIndex(const std::string& actorName) const;
    void AddActor(std::string actorName, size_t index, physx::PxRigidDynamic* body, const Quatf orientation, const Vect3f position);
    void AddActor(std::string actorName, size_t index, physx::PxRigidStatic* body, const Quatf orientation, const Vect3f position);

    std::map<std::string, physx::PxMaterial*> m_Materials;
    std::map<std::string, physx::PxController*> m_CharacterControllers;

    float m_LeftoverSeconds;

public:
    void RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution);
    void CreatePlane(std::string aMaterialName, float x, float y, float z, float d, size_t index);

    //STATIC
    void CreateStaticBox(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, float sizeX, float sizeY, float sizeZ);
    void CreateStaticSphere(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, float radius);
    void CreateStaticShape(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, std::vector<PxVec3> vertices);
    void CreateStaticTriangleMesh(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, std::vector<PxVec3> vertices);

    //DYNAMIC
    void CreateDynamicBox(std::string actorName,std::string aMaterialName, const Quatf orientation, const Vect3f position, float sizeX, float sizeY, float sizeZ, physx::PxReal density);
    void CreateDynamicSphere(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, float radius, physx::PxReal density);
    void CreateDynamicShape(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, std::vector<PxVec3> vertices, physx::PxReal);
    void CreateDynamicTriangleMesh(const std::string& actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position, std::vector<PxVec3> vertices, physx::PxReal);
    void Update(float _dt);

    void AddTriggerBox(const std::string& actorName, float sizeX, float sizeY, float sizeZ, const Vect3f& position, const Quatf& orientation);

    CharacterControllerData MoveCharacterController(const std::string& characterControllerName, const Vect3f& movement, float elapsedTime);
    bool Raycast(const Vect3f& origin, const Vect3f& end, int filterMask, RaycastData* result_ = nullptr);
    void AddFixedJoint(const std::string& jointName, const std::string& actor1Name, const std::string& actor2Name);
    void DeleteActor(std::string actorName, size_t index);

    Vect3f GetActorPosition(const std::string& actorName) const;
    Quatf GetActorOrientation(const std::string& actorName) const;
};

#endif //_ENGINE_RENDERMANAGER_CPB_2016110320428_H
