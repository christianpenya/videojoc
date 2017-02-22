#include "Math/Quaternion.h"
#include "PhysxManagerImplementation.h"
#include <vector>

/* Otro aspecto destacable es que estas librerías llevan el sufijo “ DEBUG ”. Esto se tiene que
corregir en la versión final ya que estas son más lentas que las librerías “release”(sin sufijo)
de PhysX al llevar más comprobaciones de errores.Hay además otro tipo de librerías, las
“ CHECKED ”, que son un intermedio de las anteriores.*/

#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysXProfileSDKDEBUG.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDKDEBUG.lib")
#pragma comment(lib, "PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x86.lib")

#else //Else load libraries for Release mode
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysXProfileSDK.lib")
#pragma comment(lib, "PhysXVisualDebuggerSDK.lib")
#pragma comment(lib, "PhysX3CharacterKinematic_x86.lib")
#pragma comment(lib, "PhysX3Cooking_x86.lib")
#endif


#if USE_PHYSX_DEBUG
#define					PVD_HOST			"127.0.0.1"
#endif

#ifdef CHECKED_RELEASE
#undef CHECKED_RELEASE
#endif
#define CHECKED_RELEASE(x) if(x!=nullptr) {x->release(); x=nullptr;}


CPhysXManager::CPhysXManager()
{}

CPhysXManager::~CPhysXManager()
{
    CHECKED_RELEASE(m_ControllerManager);
    CHECKED_RELEASE(m_Scene);
    CHECKED_RELEASE(m_Dispatcher);
    physx::PxProfileZoneManager* profileZoneManager = m_PhysX->getProfileZoneManager();
#	if USE_PHYSX_DEBUG
    CHECKED_RELEASE(m_DebugConnection);
#	endif
    CHECKED_RELEASE(m_Cooking);
    CHECKED_RELEASE(m_PhysX);
    CHECKED_RELEASE(profileZoneManager);
    CHECKED_RELEASE(m_Foundation);

    for (auto it = m_CharacterControllers.begin(); it != m_CharacterControllers.end(); ++it)
    {
        physx::PxController *cct = it->second;
        cct->release();
    }
    m_CharacterControllers.clear();
}


CPhysXManager* CPhysXManager::CreatePhysXManager()
{
    return new CPhysXManagerImplementation();
}


inline physx::PxVec3 CastVec(const Vect3f& v)
{
    return physx::PxVec3(v.x, v.y, v.z);
}
inline Vect3f CastVec(const physx::PxVec3& v)
{
    return Vect3f(v.x, v.y, v.z);
}
inline Vect3f CastVec(const physx::PxExtendedVec3& v)
{
    return Vect3f((float)v.x, (float)v.y, (float)v.z);
}

inline physx::PxQuat CastQuat(const Quatf& q)
{
    return physx::PxQuat(q.x, q.y, q.z, q.w);
}
inline Quatf CastQuat(const physx::PxQuat& q)
{
    return Quatf(q.x, q.y, q.z, q.w);
}

Vect3f CPhysXManager::GetActorPosition(const std::string& actorName)
{
    auto it = m_ActorIndexs.find(actorName);
    assert(it != m_ActorIndexs.end());
    size_t index = it->second;
    return m_ActorPositions[index];
}

Quatf CPhysXManager::GetActorOrientation(const std::string& actorName)
{
    auto it = m_ActorIndexs.find(actorName);
    assert(it != m_ActorIndexs.end());
    size_t index = it->second;
    return m_ActorOrientations[index];
}

void CPhysXManager::RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution)
{

    auto it = m_Materials.find(name);
    if (it != m_Materials.end())
    {
        it->second->release(); // if a material with taht name exist, we remove it
    }

    assert(m_Materials.find(name) == m_Materials.end()); //duplicated key!
    m_Materials[name] = m_PhysX->createMaterial(staticFriction, dynamicFriction, restitution);
}

// #TODO este método es una mierda, no entieno donde tiene que ir ni cómo se va a usar. está hecho de manera que compile y ya.
void CPhysXManager::CreateStaticObject(std::string aMaterialName, const Quatf orientation, const Vect3f position, float sizeX, float sizeY, float sizeZ, size_t index)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];

    physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(sizeX / 2, sizeY / 2, sizeZ / 2), (*l_Material));
    physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    body->attachShape(*shape);
    body->userData = (void*)index;
    m_Scene->addActor(*body);

    shape->release();
}

void CPhysXManager::CreatePlane(std::string aMaterialName, float x, float y, float z, float d, size_t index)
{
    physx::PxMaterial* l_Material = m_Materials[aMaterialName];
    physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*m_PhysX, physx::PxPlane(x, y, z, d), (*l_Material));
    groundPlane->userData = (void*)index;
    m_Scene->addActor(*groundPlane);

    physx::PxShape* shape;
    size_t numShapes = groundPlane->getShapes(&shape, 1);
    assert(numShapes == 1);
}

void CPhysXManager::CreateDynamicObject(std::string aMaterialName, const Quatf orientation, const Vect3f position, float sizeX, float sizeY, float sizeZ, size_t index, physx::PxReal density)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];

    physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(sizeX / 2, sizeY / 2, sizeZ / 2), (*l_Material));
    body->attachShape(*shape);
    body->userData = (void*)index;
    physx::PxRigidBodyExt::updateMassAndInertia(*body, density);

    m_Scene->addActor(*body);

    shape->release();
}

void CPhysXManager::CreateShape(std::string aMaterialName, const Quatf orientation, const Vect3f position)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];
    //std::vector<Vect3f> vertices; //old

    static const PxVec3 vertices[] = { PxVec3(0, 1, 0), PxVec3(1, 0, 0), PxVec3(-1, 0, 0), PxVec3(0, 0, 1), PxVec3(0, 0, -1) };

    physx::PxConvexMeshDesc convexDesc;
    convexDesc.points.count = 5;
    //convexDesc.points.count = vertices.size(); //old
    convexDesc.points.stride = sizeof(PxVec3);
    //convexDesc.points.stride = sizeof(Vect3f); //old
    convexDesc.points.data = vertices;
    //convexDesc.points.data = &vertices[0]; //old
    convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxDefaultMemoryOutputStream buf;
    physx::PxConvexMeshCookingResult::Enum result;
    bool success = m_Cooking->cookConvexMesh(convexDesc, buf, &result);
    assert(success);
    physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = m_PhysX->createConvexMesh(input);

    physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    physx::PxShape* shape = body->createShape(physx::PxConvexMeshGeometry(convexMesh), *l_Material);
}

void CPhysXManager::Update(float _dt)
{
    m_LeftoverSeconds += _dt;
    if (m_LeftoverSeconds >= PHYSX_UPDATE_STEP)
    {
        m_Scene->simulate(PHYSX_UPDATE_STEP);
        m_Scene->fetchResults(true);

        physx::PxU32 numActiveTransforms;
        const physx::PxActiveTransform* activeTransforms = m_Scene->getActiveTransforms(numActiveTransforms);

        for (physx::PxU32 i = 0; i < numActiveTransforms; ++i)
        {
            uintptr_t index = (uintptr_t)(activeTransforms[i].userData);
            m_ActorPositions[index] = CastVec(activeTransforms[i].actor2World.p);
            m_ActorOrientations[index] = CastQuat(activeTransforms[i].actor2World.q);
        }

        m_LeftoverSeconds = fmod(m_LeftoverSeconds, PHYSX_UPDATE_STEP);
    }
}


void CPhysXManager::AddTriggerBox(const std::string& actorName, float sizeX, float sizeY, float sizeZ, const Vect3f& position, const Quatf& orientation)
{
    assert(m_ActorIndexs.find(actorName) == m_ActorIndexs.end()); // duplicated key!

    assert(m_Actors.size() == m_ActorNames.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorPositions.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorOrientations.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorIndexs.size()); // AOS sync fail


    auto *l_Material = m_Materials["Default"];
    size_t index = m_Actors.size();

    physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(sizeX / 2, sizeY / 2, sizeZ / 2), *l_Material);
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    body->attachShape(*shape);
    body->userData = (void*)index;
    m_Scene->addActor(*body);

    shape->release();

    m_ActorIndexs[actorName] = index;
    m_ActorNames.push_back(actorName);
    m_ActorPositions.push_back(position);
    m_ActorOrientations.push_back(orientation);
    m_Actors.push_back(body);
}


CPhysXManager::CharacterControllerData CPhysXManager::MoveCharacterController(const std::string& characterControllerName, const Vect3f& movement, float elapsedTime)
{
    physx::PxController* cct = m_CharacterControllers[characterControllerName];
    const physx::PxControllerFilters filters(nullptr, nullptr, nullptr);

    physx::PxRigidDynamic* actor = cct->getActor();
    size_t index = (size_t)actor->userData;

    cct->move(CastVec(movement), movement.Length() * 0.01f, elapsedTime, filters);


    physx::PxExtendedVec3 p = cct->getFootPosition();
    physx::PxVec3 v = actor->getLinearVelocity();

    m_ActorPositions[index] = CastVec(p);

    CharacterControllerData result;
    result.position = CastVec(p);
    result.linearVelocity = CastVec(v);
    return result;
}

bool CPhysXManager::Raycast(const Vect3f& origin, const Vect3f& end, int filterMask, RaycastData* result_)
{
    Vect3f dir = end - origin;
    float len = dir.Length();
    if (origin != Vect3f(0, 0, 0))
    {
        dir.Normalize(1);
    }
    else
    {
        dir = Vect3f(1, 0, 0);
    }

    physx::PxFilterData filterData;
    filterData.setToDefault();
    filterData.word0 = 1;

    physx::PxRaycastBuffer hit;
    bool status = m_Scene->raycast(CastVec(origin), CastVec(dir), len, hit, physx::PxHitFlags(physx::PxHitFlag::eDEFAULT), physx::PxQueryFilterData(filterData, physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC));
    if (status && result_ != nullptr)
    {
        result_->position = Vect3f(hit.block.position.x, hit.block.position.y, hit.block.position.z);
        result_->normal = Vect3f(hit.block.normal.x, hit.block.normal.y, hit.block.normal.z);
        result_->distance = hit.block.distance;
        result_->actor = m_ActorNames[(size_t)hit.block.actor->userData];
    }
    return status;
}

void CPhysXManager::AddFixedJoint(const std::string& jointName, const std::string& actor1Name, const std::string& actor2Name)
{
    //assert(m_Joints.find(jointName) == m_Joints.end()); // duplicated key!

    /*physx::PxRigidActor* l_Actor1 = nullptr;
    physx::PxRigidActor* l_Actor2 = nullptr;*/ //Old

    physx::PxActor* l_Actor1 = nullptr;
    physx::PxActor* l_Actor2 = nullptr;


    auto l_Actor1It = m_ActorIndexs.find(actor1Name);
    auto l_Actor2It = m_ActorIndexs.find(actor2Name);

    assert(l_Actor1It != m_ActorIndexs.end());

    l_Actor1 = m_Actors[l_Actor1It->second];
    if (l_Actor2It != m_ActorIndexs.end())
    {
        l_Actor2 = m_Actors[l_Actor2It->second];
    }

    assert(l_Actor1 != nullptr);

    physx::PxTransform transform1, transform2;

    if (l_Actor2 != nullptr)
    {
        /*        transform1 = l_Actor1->getGlobalPose();
                transform2 = l_Actor2->getGlobalPose();

                physx::PxTransform transform1to2 = l_Actor2->getGlobalPose().getInverse() * l_Actor1->getGlobalPose();

                transform1 = transform1to2;
                transform2 = physx::PxTransform(physx::PxVec3(0, 0, 0), physx::PxQuat(0, 0, 0, 1));*/
    }

    /*    physx::PxFixedJoint *joint = physx::PxFixedJointCreate(*m_PhysX, l_Actor1, transform1, l_Actor2, transform2);

        m_Joints[jointName] = joint;*/
}

void CPhysXManager::DeleteActor(std::string actorName, size_t index)
{
    auto it_controller = m_CharacterControllers.find(actorName);
    if (it_controller != m_CharacterControllers.end())
    {
        it_controller->second->release();
        m_CharacterControllers.erase(it_controller);
    }
    else
    {
        m_Actors[index]->release();
    }

    m_Actors[index] = m_Actors[m_Actors.size() - 1];
    m_Actors.resize(m_Actors.size() - 1);
    m_ActorIndexs[m_ActorNames[index]] = index;
    m_Actors[index]->userData = (void*)index;
}



