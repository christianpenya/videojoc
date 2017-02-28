#include "Engine.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI\imgui.h"
#include "Utils/Logger.h"

#include "Graphics/Camera/CameraController.h"
#include "Input\ActionManager.h"
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
#include "Graphics/Cinematics\CinematicsManager.h"

#undef BUILD_GET_SET_ENGINE_MANAGER

CEngine::CEngine()
    : m_MaterialManager(nullptr)
    , m_TextureManager(nullptr)
    , m_RenderManager(nullptr)
    , m_CameraController(nullptr)
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
    , m_DeltaTime(0)
    , m_DeltaTimeAcum (0)
    , m_Frames(0)
    , m_FPS (0.0)
    , m_CameraSelector(0)
    , m_PrevCameraSelector(0)
{}

CEngine::~CEngine()
{
    ImGui_ImplDX11_Shutdown();
    base::utils::CheckedDelete(m_RenderPipeline);
    base::utils::CheckedDelete(m_CinematicManager);
    base::utils::CheckedDelete(m_SceneManager);
    base::utils::CheckedDelete(m_LightManager);
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
}


void CEngine::LoadFiles()
{
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

    m_SceneManager = new CSceneManager();
    m_SceneManager->Load(m_FileSceneManager);
    LOG_INFO_APPLICATION("Engine -> Scenes Loaded! \\(^-^)/");

    m_CinematicManager = new CCinematicManager;
    m_CinematicManager->Load("data/cinematics.xml");
    LOG_INFO_APPLICATION("Engine -> Cinematics Loaded! \\(^-^)/");

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
        m_FileLightManager = call_function<std::string>(mLS, "getFileLightManager");
        m_FileSceneManager = call_function<std::string>(mLS, "getFileSceneManager");
        m_FileShaderManager = call_function<std::string>(mLS, "getFileShaderManager");
        m_FileTechniquePoolManager = call_function<std::string>(mLS, "getFileTechniquePoolManager");
        m_FileTextureManager = call_function<std::string>(mLS, "getFileTexture");
        m_FileActionManager = call_function<std::string>(mLS, "getActionManager");
        m_FileRenderPipeline = call_function<std::string>(mLS, "getRenderPipeline");
        LOG_INFO_APPLICATION("Engine -> Lua Finished! (/.__.)/ \\(.__.\\)");

        LoadFiles();
    }
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
    //m_DeltaTime = m_DeltaTime > 0.5f ? 0.5f : m_DeltaTime;

    // Reiniciem posició de l'esfera quan canviem de camera
    if (m_CameraSelector != m_PrevCameraSelector)
    {
        m_RenderManager->m_SphereOffset = Vect3f(0, 0, 0);
    }

    m_PrevCameraSelector = m_CameraSelector;

    switch (m_CameraSelector)
    {
    case 0: //Orbital
        SetCameraController(&m_OrbitalCam);
        orbitalCameraUpdate(*m_CameraController, m_ActionManager, (float)m_DeltaTime);

        break;
    case 1: //FPS
        SetCameraController(&m_FpsCam);
        fpsCameraUpdate(*m_CameraController, m_ActionManager, (float)m_DeltaTime);

        break;
    case 2: //TPS
        SetCameraController(&m_TpsCam);
        sphereUpdate(*m_RenderManager, m_ActionManager, m_CameraController->getFront(), m_CameraController->getUp());
        tpsCameraUpdate(*m_CameraController, m_ActionManager, m_RenderManager->m_SphereOffset, (float)m_DeltaTime);

        break;
    default:
        break;
    }

    m_CameraController->Update((float)m_DeltaTime);
    m_CameraController->SetToRenderManager(*m_RenderManager);
    m_RenderManager->Update();
    m_SceneManager->Update(m_DeltaTime);
    m_CinematicManager->Update(m_DeltaTime);
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

void CEngine::fpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt)
{
    CFpsCameraController *fpsCamera = static_cast<CFpsCameraController*>(&camera);
    fpsCamera->xSpeed = 0.1f * (*actionManager)("x_move")->value;
    fpsCamera->zSpeed = 0.1f * (*actionManager)("z_move")->value;

    fpsCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    fpsCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    fpsCamera->Update(dt);
}

void CEngine::orbitalCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt)
{
    CSphericalCameraController *sphericalCamera = static_cast<CSphericalCameraController*>(&camera);
    sphericalCamera->zoomSpeed = (*actionManager)("zoom")->value;

    if ((*actionManager)("enable_rotation")->active)
    {
        sphericalCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
        sphericalCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;
    }
    else
    {
        sphericalCamera->yawSpeed = 0.0;
        sphericalCamera->pitchSpeed = 0.0;
    }

    sphericalCamera->Update(dt);
}

void CEngine::tpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, Vect3f sphereCenter, float dt)
{
    CTpsCameraController *tpsCamera = static_cast<CTpsCameraController*>(&camera);
    tpsCamera->center = sphereCenter;

    tpsCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    tpsCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    tpsCamera->zoomSpeed = (*actionManager)("zoom")->value;

    tpsCamera->Update(dt);
}

float clamp(float x, float upper, float lower)
{
    return min(upper, max(x, lower));
}

void CEngine::sphereUpdate(CRenderManager& renderManager, CActionManager* actionManager, Vect3f front, Vect3f up)
{
    Vect3f right = front ^ up;
    float smoother = 0.1f;

    renderManager.m_SphereOffset += (*actionManager)("z_move")->value * front * smoother;
    renderManager.m_SphereOffset += (*actionManager)("x_move")->value * right * smoother;

    renderManager.m_SphereOffset.x = clamp(renderManager.m_SphereOffset.x, 5.0, -5.0);
    renderManager.m_SphereOffset.y = 1;
    renderManager.m_SphereOffset.z = clamp(renderManager.m_SphereOffset.z, 5.0, -5.0);
}

void CEngine::sphereRender(CRenderManager& renderManager)
{
    renderManager.DrawSphere(1, CColor(1, 1, 1, 1));
}
