#include "Engine.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui.h"
#include "Utils/Logger.h"

#include "Graphics/Camera/CameraManager.h"

#include "Input/ActionManager.h"
#include "Graphics/Materials/MaterialManager.h"
#include "Graphics/Textures/TextureManager.h"
#include "Graphics/Mesh/MeshManager.h"
#include "Graphics/Effects/ShaderManager.h"
#include "Graphics/Effects/EffectManager.h"
#include "Graphics/Effects/TechniquePoolManager.h"
#include "Graphics/Scenes/SceneManager.h"
#include "Graphics/Lights/LightManager.h"
#include "Graphics/Buffers/ConstantBufferManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Graphics/Animation/AnimatedModelManager.h"
#include "Scripts/ScriptManager.h"
#include "Graphics/Cinematics/CinematicsManager.h"
#include "Physx/PhysxManager.h"
#include "Graphics/Particles/ParticleManager.h"
#include "Graphics/IA/NavMeshManager.h"
#include "Graphics/IA/EnemiesManager.h"
#include "Graphics/IA/LaserManager.h"
#include "Sound/ISoundManager.h"
#include "GUI/GUIManager.h"
#include "GUI/GUIPosition.h"
#include "Graphics/Animation/SceneAnimatedModel.h"
#include "Input/CharacterController.h"
#include "Events/LevelController.h"
#include "Events/EventManager.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

#undef BUILD_GET_SET_ENGINE_MANAGER

CEngine::CEngine()
    : m_MaterialManager(nullptr)
    , m_TextureManager(nullptr)
    , m_RenderManager(nullptr)
    , m_CameraManager(nullptr)
    , m_SceneManager(nullptr)
    , m_InputManager(nullptr)
    , m_ActionManager(nullptr)
    , m_ShaderManager(nullptr)
    , m_EffectManager(nullptr)
    , m_MeshManager(nullptr)
    , m_TechniquePoolManager(nullptr)
    , m_LightManager(nullptr)
    , m_RenderPipeline(nullptr)
    , m_ConstantBufferManager(nullptr)
    , m_AnimatedModelManager(nullptr)
    , m_ScriptManager(nullptr)
    , m_CinematicManager(nullptr)
    , m_PhysXManager(nullptr)
    , m_ParticleManager(nullptr)
    , m_NavMeshManager(nullptr)
    , m_EnemiesManager(nullptr)
    , m_LaserManager(nullptr)
    , m_GUIManager(nullptr)
    , m_CharacterController(nullptr)
    , m_LevelController(nullptr)
    , m_DeltaTime(0)
    , m_DeltaTimeAcum (0)
    , m_Frames(0)
    , m_FPS (0.0)
{
}

CEngine::~CEngine()
{
    ImGui_ImplDX11_Shutdown();

    // TODO Peta de mala manera
    // base::utils::CheckedDelete(m_PhysXManager);
    // base::utils::CheckedDelete(m_RenderManager);
    base::utils::CheckedDelete(m_RenderPipeline);
    base::utils::CheckedDelete(m_ParticleManager);
    base::utils::CheckedDelete(m_CinematicManager);
    base::utils::CheckedDelete(m_LightManager);
    base::utils::CheckedDelete(m_SceneManager);
    base::utils::CheckedDelete(m_MeshManager);
    base::utils::CheckedDelete(m_AnimatedModelManager);
    base::utils::CheckedDelete(m_MaterialManager);
    base::utils::CheckedDelete(m_TextureManager);
    base::utils::CheckedDelete(m_TechniquePoolManager);
    base::utils::CheckedDelete(m_EffectManager);
    base::utils::CheckedDelete(m_ShaderManager);
    base::utils::CheckedDelete(m_ConstantBufferManager);
    base::utils::CheckedDelete(m_ActionManager);
    base::utils::CheckedDelete(m_InputManager);
    base::utils::CheckedDelete(m_ScriptManager);
    base::utils::CheckedDelete(m_SoundManager);
    base::utils::CheckedDelete(m_NavMeshManager);
    base::utils::CheckedDelete(m_GUIManager);
    base::utils::CheckedDelete(m_EventManager);
    base::utils::CheckedDelete(m_EnemiesManager);
    base::utils::CheckedDelete(m_LaserManager);

}

void CEngine::LoadFiles()
{
    ImGui_ImplDX11_NewFrame();

    m_ActionManager = new CActionManager(*m_InputManager);
    m_ActionManager->LoadActions(m_FileActionManager);
    LOG_INFO_APPLICATION("Engine -> Action Manager Loaded! \\(^-^)/");

    m_ConstantBufferManager = new CConstantBufferManager();
    LOG_INFO_APPLICATION("Engine -> Constant Buffer Loaded! \\(^-^)/");

    m_ShaderManager = new CShaderManager();
    m_ShaderManager->Load(m_FileShaderManager);
    LOG_INFO_APPLICATION("Engine -> Shaders Loaded! \\(^-^)/");

    m_EffectManager = new CEffectManager();
    m_EffectManager->Load(m_FileEffectManager);
    LOG_INFO_APPLICATION("Engine -> Effects Loaded! \\(^-^)/");

    m_TechniquePoolManager = new CTechniquePoolManager();
    m_TechniquePoolManager->Load(m_FileTechniquePoolManager);
    LOG_INFO_APPLICATION("Engine -> Technique Pool Loaded! \\(^-^)/");

    std::string lLevelMaterialsFilename = m_FileMaterialManager;
    std::string lDefaultMaterialsFilename = m_FileDefaultMaterial;

    m_TextureManager = new CTextureManager();
    LOG_INFO_APPLICATION("Engine -> Textures Manager Initiated! \\(^-^)/");

    m_MaterialManager = new CMaterialManager();
    m_MaterialManager->Load(lLevelMaterialsFilename, lDefaultMaterialsFilename);
    LOG_INFO_APPLICATION("Engine -> Materials Loaded! \\(^-^)/");

    m_AnimatedModelManager = new CAnimatedModelManager();
    m_AnimatedModelManager->Load(m_FileAnimatedModelManager);
    LOG_INFO_APPLICATION("Engine -> Animated Models Loaded! \\(^-^)/");

    m_MeshManager = new CMeshManager();
    LOG_INFO_APPLICATION("Engine -> Mesh Manager Initiated! \\(^-^)/");

    m_LightManager = new CLightManager();
    m_LightManager->Load(m_FileLightManager);
    LOG_INFO_APPLICATION("Engine -> Lights Loaded! \\(^-^)/");

    m_PhysXManager = CPhysXManager::CreatePhysXManager();
    const std::string material = "Default";
    LOG_INFO_APPLICATION("Engine -> PhysX Loaded! \\(^-^)/");

    m_ParticleManager = new CParticleManager();
    m_ParticleManager->Load(m_FileParticleManager);
    LOG_INFO_APPLICATION("Engine -> Particles Loaded! \\(^-^)/");

    m_NavMeshManager = new CNavMeshManager;
    m_NavMeshManager->Load("data/navMesh.xml");
    LOG_INFO_APPLICATION("Engine -> NavMesh Loaded! \\(^-^)/");

    m_EnemiesManager = new CEnemiesManager;
    m_EnemiesManager->Load("data/enemies.xml");
    LOG_INFO_APPLICATION("Engine -> Enemies Loaded! \\(^-^)/");

    m_LaserManager = new CLaserManager;
    m_LaserManager->Load("data/laser.xml");
    LOG_INFO_APPLICATION("Engine -> Laser Loaded! \\(^-^)/");


    m_SceneManager = new CSceneManager();
    m_SceneManager->Load(m_FileSceneManager);
    LOG_INFO_APPLICATION("Engine -> Scenes Loaded! \\(^-^)/");

    m_CinematicManager = new CCinematicManager;
    m_CinematicManager->Load(m_FileCinematicManager);
    LOG_INFO_APPLICATION("Engine -> Cinematics Loaded! \\(^-^)/");

    m_SoundManager = ISoundManager::InstantiateSoundManager();
    m_SoundManager->SetPath(m_SoundFilesPath);
    m_SoundManager->Init();
    m_SoundManager->Load(m_BanksFile, m_SpeakersFile);

    m_GUIManager = new CGUIManager();
    m_GUIManager->Load("data/gui.xml");
    LOG_INFO_APPLICATION("Engine -> GUI Loaded! \\(^-^)/");

    m_CharacterController = new CCharacterController();
    m_CharacterController->Init(m_SceneManager);

    m_LevelController = new CLevelController(1);
    m_LevelController->Init();

    m_CameraManager = new CCameraManager();
    m_CameraManager->Init(m_CharacterController);


    m_EventManager = new CEventManager();
    m_EventManager->Load(m_FileEventManager);

    m_RenderPipeline = new CRenderPipeline();
    m_RenderPipeline->Load(m_FileRenderPipeline);
    LOG_INFO_APPLICATION("Engine -> Render Pipeline Loaded! \\(^-^)/");
}

void CEngine::Init(HWND hWnd)
{
    LOG_INFO_APPLICATION("Engine -> Init -.-");

    ImGui_ImplDX11_Init(hWnd, m_RenderManager->GetDevice(), m_RenderManager->GetDeviceContext());
    m_InputManager = new CInputManager(hWnd);
    m_ScriptManager = new CScriptManager();

    if (m_ScriptManager->Load("data/scripts/engine.lua"))
    {
        lua_State* mLS = m_ScriptManager->GetScript("data/scripts/engine.lua")->GetState();

        LOG_INFO_APPLICATION("Engine -> Lua Loaded! (-(-_(-_-)_-)-)");

        m_FileAnimatedModelManager = call_function<std::string>(mLS, "getFileAnimatedModel");
        m_FileDefaultMaterial = call_function<std::string>(mLS, "getFileDefaultMaterial");
        m_FileEffectManager = call_function<std::string>(mLS, "getFileEffects");
        m_FileMaterialManager = call_function<std::string>(mLS, "getFileLevelMaterial");
        m_FileNavMeshManager = call_function<std::string>(mLS, "getFileNavMeshManager");
        m_FileEnemiesManager = call_function<std::string>(mLS, "getFileEnemiesManager");
        m_FileLightManager = call_function<std::string>(mLS, "getFileLightManager");
        m_FileSceneManager = call_function<std::string>(mLS, "getFileSceneManager");
        m_FileShaderManager = call_function<std::string>(mLS, "getFileShaderManager");
        m_FileTechniquePoolManager = call_function<std::string>(mLS, "getFileTechniquePoolManager");
        m_FileTextureManager = call_function<std::string>(mLS, "getFileTexture");
        m_FileActionManager = call_function<std::string>(mLS, "getActionManager");
        m_FileRenderPipeline = call_function<std::string>(mLS, "getRenderPipeline");
        m_FileParticleManager = call_function<std::string>(mLS, "getFileParticleManager");
        m_FileCinematicManager = call_function<std::string>(mLS, "getFileCinematicManager");
        m_FileEventManager = call_function<std::string>(mLS, "getFileEventManager");
        m_FileLaserManager = call_function<std::string>(mLS, "getFileLaserManager");
        m_SoundFilesPath = call_function<std::string>(mLS, "getSoundFilesPath");
        m_SpeakersFile = call_function<std::string>(mLS, "getSoundSpeakersFile");
        m_BanksFile = call_function<std::string>(mLS, "getSoundBankFile");

        LOG_INFO_APPLICATION("Engine -> Lua Finished! (/.__.)/ \\(.__.\\)");

        LoadFiles();
    }


    //SetCameraController(m_FreeCam);

    //TEST SOUND
    //SoundEvent se;
    //se.eventName = "background_music";
    //m_SoundManager->PlayEvent(se);
}

void CEngine::ProcessInputs() const
{
    ImGui_ImplDX11_NewFrame();
    m_ActionManager->Update();
}

double clockToMilliseconds(clock_t ticks)
{
    // units/(units/time) => time (seconds) * 1000 = milliseconds
    // TODO evitar casts en update o render
    return (ticks / static_cast<double>(CLOCKS_PER_SEC))*1000.0;
}

void CEngine::Update()
{
    //CharacterControllerUpdate(m_ActionManager, (float)m_DeltaTime);
    m_LevelController->Update(m_DeltaTime);
    m_PhysXManager->Update(m_DeltaTime);
    m_CharacterController->Update(m_DeltaTime);
    //m_PhysXManager->MoveCharacterController("player", m_CharacterController->m_Movement, PHYSX_UPDATE_STEP);

    //m_CameraController->SetToRenderManager(*m_RenderManager);
    m_CameraManager->Update((float)m_DeltaTime);

    m_RenderManager->Update();
    m_SceneManager->Update(m_DeltaTime);
    m_CinematicManager->Update(m_DeltaTime);

    m_SoundManager->Update(&m_CameraManager->GetCurrentCamera());

    m_EventManager->Update(m_DeltaTime);

    // ReSharper disable once CppMsExtBindingRValueToLvalueReference


    //Vect2u size  = m_RenderManager->GetWindowSize();
    // m_GUIManager->DoGUISprite("gui1", "backgroundMENU", CGUIPosition(0, 0, size.x, size.y));
    /* if (m_GUIManager->DoButton("gui1", "teula_button", CGUIPosition(50, 50, 400, 100)))
     {

     }*/

    //m_GUIManager->FillCommandQueueWithText("font1", "TEST", l_Pos, CGUIManager::TOP_CENTER,CColor(1.0f, 1.0f, 1.0f));
    //m_GUIManager->DoSlider("slider1", "teula_slider",  CGUIPosition(50, 50, 412, 40), .0f, 100.0f, .0f);
}

void CEngine::Render()
{
    clock_t l_BeginFrame = clock();

    m_RenderPipeline->Execute();

    clock_t l_EndFrame = clock();
    m_DeltaTime = l_EndFrame - l_BeginFrame;
    m_DeltaTime = m_DeltaTime / 400.0f;
    m_DeltaTimeAcum += l_EndFrame - l_BeginFrame;
    ++m_Frames;

    if (clockToMilliseconds(m_DeltaTimeAcum) > 1000.0)  //every second
    {
        m_FPS = (double)m_Frames*0.5 + m_FPS*0.5; //more stable
        m_Frames = 0;
        m_DeltaTimeAcum -= CLOCKS_PER_SEC;
        //averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);
    }
}

float clamp(float x, float upper, float lower)
{
    return min(upper, max(x, lower));
}


/*
void CEngine::CharacterControllerUpdate(CActionManager* actionManager, float dt)
{
float x = (*actionManager)("x_move")->value * 0.5f;
float z = (*actionManager)("z_move")->value * 0.5f;
m_CharacterController.m_Movement = {x, 0.0f, z};
}*/

void CEngine::DrawImgui(int choice)
{
    if (choice == 0)
        m_SceneManager->DrawImgui();
    else if (choice == 1)
        m_SoundManager->DrawImgui();
    else if (choice == 2)
        m_MaterialManager->DrawImgui();
    else if (choice == 3)
        m_EventManager->DrawImgui();
    else if (choice == 4)
        m_LightManager->DrawImgui();
    else if (choice == 5)
        m_ParticleManager->DrawImgui();
    else if (choice == 6)
        m_RenderPipeline->DrawImgui();
    else if (choice == 7)
        m_EnemiesManager->DrawImgui();
}
