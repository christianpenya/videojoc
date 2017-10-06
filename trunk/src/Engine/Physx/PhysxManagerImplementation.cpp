#include "PhysxManagerImplementation.h"
#include "foundation/PxFoundation.h"
#include "cooking/PxCooking.h"
#include "PxPhysics.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "PxSceneDesc.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include <characterkinematic/PxControllerManager.h>
#include "Utils/Logger.h"
#include "Engine/Engine.h"
#include "Events/Event.h"
#include "Events/EventManager.h"

CPhysXManagerImplementation::CPhysXManagerImplementation()
{
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;
    static physx::PxDefaultErrorCallback gDefaultErrorCallback;
    physx::PxFoundation *mFoundation = nullptr;

    m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    assert(m_Foundation);

    physx::PxProfileZoneManager* profileZoneManager = nullptr;
#	if USE_PHYSX_DEBUG
    profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(m_Foundation);
#	endif
    m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, profileZoneManager);
    assert(m_PhysX);

    m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(physx::PxTolerancesScale()));
    assert(m_Cooking);

#	if USE_PHYSX_DEBUG
    if (m_PhysX->getPvdConnectionManager())
    {
        m_PhysX->getVisualDebugger()->setVisualizeConstraints(true);
        m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
        m_PhysX->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
        m_DebugConnection = physx::PxVisualDebuggerExt::createConnection(m_PhysX->getPvdConnectionManager(), PVD_HOST, 5425, 10);
    }
    else
    {
        m_DebugConnection = nullptr;
    }
#	endif

    physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_Dispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    sceneDesc.flags = physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
    m_Scene = m_PhysX->createScene(sceneDesc);
    assert(m_Scene);

    m_Scene->setSimulationEventCallback(this);

    m_ControllerManager = PxCreateControllerManager(*m_Scene, true);
    m_ControllerManager->setOverlapRecoveryModule(true);

#	if USE_PHYSX_DEBUG
    if (m_DebugConnection != nullptr)
    {
        m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
        m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
    }
#	endif

    // default material

    RegisterMaterial("Default", 0.5f, 0.5f, 0.6f);
    AddCharacterController("player", 1.1f, 0.17f, Vect3f(0, 1, 8), Quatf(0, 0, 0, 1), "Default", 0.5f);

    m_LeftoverSeconds = 0.0f;
}

CPhysXManagerImplementation::~CPhysXManagerImplementation() {}

void CPhysXManagerImplementation::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
    for (physx::PxU32 i = 0; i < count; i++)
    {
        // ignore pairs when shapes have been deleted
        if ((pairs[i].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)))
            continue;

        size_t indexTrigger = (size_t)pairs[i].triggerActor->userData;
        size_t indexActor = (size_t)pairs[i].otherActor->userData;

        std::string triggerName = m_ActorNames[indexTrigger];
        std::string actorName = m_ActorNames[indexActor];

        CEvent* lEvent = CEngine::GetInstance().GetEventManager().GetEvent(triggerName);

        if (lEvent && !lEvent->IsFinished() && !lEvent->IsHappeningRightFuckingNow())
        {
            lEvent->Start();
        }

        /*

        LOG_INFO_APPLICATION("Trigger \"%s\" fired with \"%s\"", triggerName.c_str(), actorName.c_str());

        if (actorName == "player")
        {
            bool hitted = Raycast(m_ActorPositions[indexTrigger], Vect3f(4.0, 0.0, 0.0), 0001, nullptr);//TODO Inicializar RaycastData y pasar-lo como parametro en lugar de nullptr
            if (hitted)
            {
                //CEngine::GetInstance().GetEventManager()("Box hitted");
            }
        }
        */
    }
}

void CPhysXManagerImplementation::AddCharacterController(const std::string& characterControllerName, float height, float radius, const Vect3f& position = Vect3f(0, 0, 0), const Quatf& orientation = Quatf(0, 0, 0, 1), const std::string& material = "Default", float density = 15)
{
    assert(m_Materials.find(material) != m_Materials.end()); // missing material!
    assert(m_CharacterControllers.find(characterControllerName) == m_CharacterControllers.end()); // duplicated key!
    assert(m_ActorIndexs.find(characterControllerName) == m_ActorIndexs.end()); // duplicated key!

    assert(m_Actors.size() == m_ActorNames.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorPositions.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorOrientations.size()); // AOS sync fail
    assert(m_Actors.size() == m_ActorIndexs.size()); // AOS sync fail

    auto *l_Material = m_Materials[material];

    size_t index = m_Actors.size();

    physx::PxCapsuleControllerDesc desc;
    desc.height = height;
    desc.radius = radius;
    desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;
    desc.slopeLimit = cosf(3.1415f / 6); // 30
    desc.stepOffset = 0.25f;
    desc.density = density;
    desc.reportCallback = this;
    desc.position = physx::PxExtendedVec3(position.x, position.y + radius + height * 0.5f, position.z);
    desc.material = l_Material;
    desc.userData = (void*)index;

    physx::PxController* cct = m_ControllerManager->createController(desc);

    m_CharacterControllers[characterControllerName] = cct;

    m_ActorIndexs[characterControllerName] = index;
    m_ActorNames.push_back(characterControllerName);
    m_ActorPositions.push_back(position);
    m_ActorOrientations.push_back(Quatf(0, 0, 0, 1));
    m_Actors.push_back(cct->getActor());

    cct->getActor()->userData = (void*)index;
}
