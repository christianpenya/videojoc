#include "Math/Quaternion.h"
#include "PhysxManagerImplementation.h"
#include <array>
#include "Utils/BinFileReader.h"
#include "Utils/CheckedRelease.h"
#include "Math/Transform.h"

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

#define HEADER 65109
#define FOOTER 22014

CPhysXManager::CPhysXManager():
    m_Foundation(nullptr),
    m_PhysX(nullptr),
    m_DebugConnection(nullptr),
    m_Dispatcher(nullptr),
    m_Scene(nullptr),
    m_Cooking(nullptr),
    m_ControllerManager(nullptr),
    m_LeftoverSeconds(0)
{}

CPhysXManager::~CPhysXManager()
{
    __H_CHECKED_RELEASE__(m_ControllerManager);
    __H_CHECKED_RELEASE__(m_Scene);
    __H_CHECKED_RELEASE__(m_Dispatcher);
    physx::PxProfileZoneManager* profileZoneManager = m_PhysX->getProfileZoneManager();
#	if USE_PHYSX_DEBUG
    __H_CHECKED_RELEASE__(m_DebugConnection);
#	endif
    __H_CHECKED_RELEASE__(m_Cooking);
    __H_CHECKED_RELEASE__(m_PhysX);
    __H_CHECKED_RELEASE__(profileZoneManager);
    __H_CHECKED_RELEASE__(m_Foundation);

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
    return m_ActorPositions[GetActorIndex(actorName)];
}

Quatf CPhysXManager::GetActorOrientation(const std::string& actorName)
{
    return m_ActorOrientations[GetActorIndex(actorName)];
}

physx::PxTransform CPhysXManager::GetActorTransform(const std::string& actorName)
{
    return physx::PxTransform(CastVec(GetActorPosition(actorName)), CastQuat(GetActorOrientation(actorName)));
}

physx::PxActor* CPhysXManager::GetActor(const std::string& actorName)
{
    return m_Actors[GetActorIndex(actorName)];
}


void CPhysXManager::SetActorTransform(const std::string& actorName, const Vect3f& position, Quatf orientation)
{
    physx::PxActor* tmp = m_Actors[GetActorIndex(actorName)];

    if (PxRigidActor * p = static_cast<PxRigidActor *>(tmp))
    {
        p->setGlobalPose(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    }
}

void CPhysXManager::RegisterMaterial(const std::string &name, float staticFriction, float dynamicFriction, float restitution)
{
    auto it = m_Materials.find(name);
    if (it != m_Materials.end())
    {
        it->second->release(); // if a material with that name exist, we remove it
    }

    assert(m_Materials.find(name) == m_Materials.end()); //duplicated key!
    m_Materials[name] = m_PhysX->createMaterial(staticFriction, dynamicFriction, restitution);
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

size_t CPhysXManager::CreateStaticBox(const std::string& actorName, std::string aMaterialName, const Quatf orientation,
                                      const Vect3f position, float sizeX, float sizeY, float sizeZ)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];
    size_t index = GetActorSize(actorName);
    physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    body->createShape(physx::PxBoxGeometry(sizeX / 2, sizeY / 2, sizeZ / 2), (*l_Material));
    body->userData = (void*)index;
    m_Scene->addActor(*body);
    AddActor(actorName, index, body, orientation, position);

    return index;
}

void CPhysXManager::CreateStaticSphere(const std::string& actorName, std::string aMaterialName, const Quatf orientation,
                                       const Vect3f position, float radius)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];
    size_t index = GetActorSize(actorName);

    physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    body->createShape(physx::PxSphereGeometry(radius), (*l_Material));
    body->userData = (void*)index;
    m_Scene->addActor(*body);

    AddActor(actorName, index, body, orientation, position);
}

void CPhysXManager::CreateStaticShape(const std::string& actorName, std::string aMaterialName, const Quatf aOrientation,
                                      const Vect3f aPosition, std::string aFileName)
{
    unsigned short vertexNum = 0;
    void* vertexData = nullptr;
    unsigned short indexNum = 0;
    void* indexData = nullptr;

    if (LoadMeshFile(aFileName, &vertexNum, &vertexData, &indexNum, &indexData))
    {
        physx::PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = vertexNum;
        meshDesc.points.stride = sizeof(physx::PxVec3);
        meshDesc.points.data = vertexData;

        meshDesc.triangles.count = indexNum;
        meshDesc.triangles.stride = 3 * sizeof(physx::PxU16);
        meshDesc.triangles.data = indexData;
        meshDesc.flags = physx::PxMeshFlag::e16_BIT_INDICES;

        PxTolerancesScale toleranceScale;
        bool value = toleranceScale.isValid(); // make sure this value is always true
        PxCookingParams params(toleranceScale);
        params.meshWeldTolerance = 0.001f;
        params.meshPreprocessParams = PxMeshPreprocessingFlags(
                                          PxMeshPreprocessingFlag::eWELD_VERTICES |
                                          PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES |
                                          PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES);

        m_Cooking->setParams(params);
        physx::PxDefaultMemoryOutputStream buf;

        m_Cooking->cookTriangleMesh(meshDesc, buf);

        physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        physx::PxTriangleMesh* triangleMesh = m_PhysX->createTriangleMesh(input);
        physx::PxMaterial* l_Material = m_Materials[aMaterialName];

        physx::PxShape* shape = m_PhysX->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *l_Material);
        physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(aPosition), CastQuat(aOrientation)));

        body->attachShape(*shape);
        m_Scene->addActor(*body);

        size_t index = GetActorSize(actorName);
        AddActor(actorName, index, body, aOrientation, aPosition);

        delete(vertexData);
        delete(indexData);

        shape->release();
    }
}

bool CPhysXManager::LoadMeshFile(std::string aFilename, unsigned short* vertexNum, void** vertexData, unsigned short* indexNum, void** indexData)
{
    base::utils::CBinFileReader MeshBin(aFilename);

    if (MeshBin.Open())
    {
        if (MeshBin.Read<unsigned short>() == HEADER)
        {
            //Lee el nmero de vertices para crear la geometra
            *vertexNum = MeshBin.Read<unsigned short>();
            //Lee los vertices segun el flag
            size_t lNumBytesVertices = *vertexNum * (sizeof(float) * 3);
            *vertexData = MeshBin.ReadRaw(lNumBytesVertices);

            //Lee el nmero de indices para crear la geometra
            *indexNum = MeshBin.Read<unsigned short>();
            //Lee los vertices segun el tipo de dato. Coloco unsigned short de momento.
            size_t lNumBytesIndices = *indexNum * sizeof(unsigned short);
            *indexData = MeshBin.ReadRaw(lNumBytesIndices);

            int val = MeshBin.Read<unsigned short>();
            if (val == FOOTER)
            {
                //MeshBin.Close();
                //loaded = true;
            }
            else
            {
                //TODO ERROR DE LECTURA DE MALLA EN ARCHIVO AFileName. Detener el programa
                return false;
            }
            MeshBin.Close();
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
    /*
    std::fstream l_File(aFilename, std::ios::binary | std::ios::in);
    if (!l_File.is_open())
    {
        return false;
    }
    else
    {
    	        unsigned short l_BufferUnsignedShort;
        l_File.read((char*)&l_BufferUnsignedShort, sizeof(unsigned short));
        if (l_BufferUnsignedShort != HEADER)
        {
            return false;
        }


        l_File.read((char*)Size_, sizeof(physx::PxU32));

        *Data_ = (physx::PxU8*)malloc(sizeof(physx::PxU8)*(*Size_));
        l_File.read((char*)*Data_, *Size_);

        l_File.read((char*)&l_BufferUnsignedShort, sizeof(unsigned short));
        if (l_BufferUnsignedShort != FOOTER)
        {
            return false;
        }
    }
    return true;
    */
}

void CPhysXManager::CreateStaticTriangleMesh(const std::string& actorName, std::string aMaterialName, const Quatf orientation,
        const Vect3f position, std::vector<PxVec3> vertices)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];
    size_t index = GetActorSize(actorName);

    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = vertices.size();
    meshDesc.points.stride = sizeof(Vect3f);
    meshDesc.points.data = &vertices[0];

    PxDefaultMemoryOutputStream writeBuffer;
    bool status = m_Cooking->cookTriangleMesh(meshDesc, writeBuffer);
    if (!status)
        return;

    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    PxTriangleMesh* lTriangleMesh = m_PhysX->createTriangleMesh(readBuffer);

    physx::PxShape* shape = m_PhysX->createShape(physx::PxTriangleMeshGeometry(lTriangleMesh), *l_Material);

    physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(position), CastQuat(orientation)));

    body->attachShape(*shape);
    body->userData = (void*)index;
    m_Scene->addActor(*body);

    AddActor(actorName, index, body, orientation, position);

    shape->release();
}

void CPhysXManager::CreateDynamicBox(std::string actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position,
                                     float sizeX, float sizeY, float sizeZ, physx::PxReal density)
{
    CreateDynamicBox(actorName, aMaterialName, orientation, position, sizeX, sizeY, sizeZ, density, false);
}

void CPhysXManager::CreateDynamicBox(std::string actorName, std::string aMaterialName, const Quatf orientation, const Vect3f position,
                                     float sizeX, float sizeY, float sizeZ, physx::PxReal density, bool isKinematic)
{
    size_t index = GetActorSize(actorName);
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];

    physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(sizeX / 2, sizeY / 2, sizeZ / 2), (*l_Material));

    physx::PxFilterData filterData;
    filterData.setToDefault();
    filterData.word0 = 0001;
    shape->setQueryFilterData(filterData);

    body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);

    body->attachShape(*shape);
    body->userData = (void*)index;

    physx::PxRigidBodyExt::updateMassAndInertia(*body, density);
    m_Scene->addActor(*body);

    AddActor(actorName, index, body, orientation, position);
}

void CPhysXManager::CreateDynamicSphere(const std::string& actorName, std::string aMaterialName, const Quatf orientation,
                                        const Vect3f position, float radius, physx::PxReal density)
{
    size_t index = GetActorSize(actorName);
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];

    physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    body->createShape(physx::PxSphereGeometry(radius), (*l_Material));
    body->userData = (void*)index;

    physx::PxRigidBodyExt::updateMassAndInertia(*body, density);
    m_Scene->addActor(*body);

    AddActor(actorName, index, body, orientation, position);
}

void CPhysXManager::CreateDynamicShape(const std::string& actorName, std::string aMaterialName, const Quatf orientation,
                                       const Vect3f position, std::vector<PxVec3> vertices, physx::PxReal density)
{
    size_t index = GetActorSize(actorName);
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];

    physx::PxConvexMeshDesc convexDesc;
    convexDesc.points.count = vertices.size();
    convexDesc.points.stride = sizeof(Vect3f);
    convexDesc.points.data = &vertices[0];
    convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    physx::PxDefaultMemoryOutputStream buf;
    physx::PxConvexMeshCookingResult::Enum result;
    bool success = m_Cooking->cookConvexMesh(convexDesc, buf, &result);
    assert(success);
    physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = m_PhysX->createConvexMesh(input);

    physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    physx::PxShape* shape = body->createShape(physx::PxConvexMeshGeometry(convexMesh), *l_Material);

    body->userData = (void*)index;
    physx::PxRigidBodyExt::updateMassAndInertia(*body, density);
    m_Scene->addActor(*body);
    AddActor(actorName, index, body, orientation, position);
}

void CPhysXManager::CreateDynamicTriangleMesh(const std::string& actorName, std::string aMaterialName,
        const Quatf orientation, const Vect3f position, std::vector<PxVec3> vertices, physx::PxReal density)
{
    const physx::PxMaterial* l_Material = m_Materials[aMaterialName];
    size_t index = GetActorSize(actorName);

    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = vertices.size();
    meshDesc.points.stride = sizeof(Vect3f);
    meshDesc.points.data = &vertices[0];

    PxDefaultMemoryOutputStream writeBuffer;
    bool status = m_Cooking->cookTriangleMesh(meshDesc, writeBuffer);
    if (!status)
        return;

    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    PxTriangleMesh* lTriangleMesh = m_PhysX->createTriangleMesh(readBuffer);

    physx::PxRigidDynamic* body = m_PhysX->createRigidDynamic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    physx::PxShape* shape = body->createShape(physx::PxTriangleMeshGeometry(lTriangleMesh), *l_Material);

    body->userData = (void*)index;
    m_Scene->addActor(*body);

    AddActor(actorName, index, body, orientation, position);
}

size_t CPhysXManager::GetActorSize(const std::string& actorName)
{
    assert(m_ActorIndexs.find(actorName) == m_ActorIndexs.end()); // duplicated key!
    assert(m_Actors.size() == m_ActorNames.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorPositions.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorOrientations.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorIndexs.size()); // AOS sync fail

    return m_Actors.size();
}

size_t CPhysXManager::GetActorIndex(const std::string& actorName) const
{
    auto it = m_ActorIndexs.find(actorName);
    assert(it != m_ActorIndexs.end());
    return it->second;
}

void CPhysXManager::AddActor(std::string actorName, size_t index, physx::PxRigidDynamic* body, const Quatf orientation, const Vect3f position)
{
    m_ActorIndexs[actorName] = index;
    m_ActorNames.push_back(actorName);
    m_ActorPositions.push_back(position);
    m_ActorOrientations.push_back(orientation);
    m_Actors.push_back(body);
}

void CPhysXManager::AddActor(std::string actorName, size_t index, physx::PxRigidStatic* body, const Quatf orientation, const Vect3f position)
{
    m_ActorIndexs[actorName] = index;
    m_ActorNames.push_back(actorName);
    m_ActorPositions.push_back(position);
    m_ActorOrientations.push_back(orientation);
    m_Actors.push_back(body);
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
    auto *l_Material = m_Materials["Default"];
    size_t index = GetActorSize(actorName);
    physx::PxRigidStatic* body = m_PhysX->createRigidStatic(physx::PxTransform(CastVec(position), CastQuat(orientation)));
    physx::PxShape* shape = m_PhysX->createShape(physx::PxBoxGeometry(sizeX/2, sizeY/2,  sizeZ/2), *l_Material);
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

    body->attachShape(*shape);
    body->userData = (void*)index;
    m_Scene->addActor(*body);

    shape->release();
    AddActor(actorName, index, body, orientation, position);
}

CPhysXManager::CharacterControllerData CPhysXManager::MoveCharacterController(const std::string& characterControllerName, const Vect3f& movement, float elapsedTime)
{
    physx::PxController* cct = m_CharacterControllers[characterControllerName];
    const physx::PxControllerFilters filters(nullptr, nullptr, nullptr);

    physx::PxRigidDynamic* actor = cct->getActor();
    size_t index = (size_t)actor->userData;
    Vect3f movement2 = movement + Vect3f(0, -1, 0);
    cct->move(CastVec(movement2), movement.Length() * 0.01f, elapsedTime, filters);

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
    /*
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
    filterData.word0 = filterMask;

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
    */

    physx::PxFilterData filterData;
    filterData.setToDefault();
    filterData.word0 = 0000;  //GROUP1 | GROUP2;
    physx::PxRaycastBuffer hit;
    bool status = m_Scene->raycast(
                      CastVec(origin),
                      CastVec((end - origin).GetNormalized()),
                      origin.Distance(end),
                      hit,
                      physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
                      physx::PxQueryFilterData(
                          filterData,
                          physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eSTATIC)
                  );
    if (status)
    {
        if (hit.hasBlock)
        {
            result_->position = CastVec(hit.block.position);
            result_->normal = CastVec(hit.block.normal);
            result_->distance = origin.Distance(end); //hit.block.distance
            result_->actor = m_ActorNames[(size_t)hit.block.actor->userData];
        }
        else
            status = false;
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

