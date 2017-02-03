#include "Engine.h"
#include "imgui_impl_dx11.h"
#include "ImGUI\imgui.h"

#include "Camera\CameraController.h"
#include "Mesh\Mesh.h"

#include "Input\ActionManager.h"
#include "Materials\MaterialManager.h"
#include "Textures\TextureManager.h"
#include "Mesh\MeshManager.h"
#include "Effects\ShaderManager.h"
#include "Effects\EffectManager.h"
#include "Effects\TechniquePoolManager.h"
#include "Scenes\SceneManager.h"
#include "Lights\LightManager.h"
#include "Scenes\ConstantBufferManager.h"

#include "RenderPipeline\RenderPipeline.h"
#include "Animation/AnimatedModelManager.h"

#undef BUILD_GET_SET_ENGINE_MANAGER

CEngine::CEngine()

    : m_RenderManager(nullptr)
    , m_InputManager(nullptr)
    , m_ActionManager(nullptr)
    , m_CameraController(nullptr)
    , m_MaterialManager(nullptr)
    , m_TextureManager(nullptr)
    , m_MeshManager(nullptr)
    , m_ShaderManager(nullptr)
    , m_EffectManager(nullptr)
    , m_TechniquePoolManager(nullptr)
    , m_LightManager(nullptr)
    , m_SceneManager(nullptr)
    , m_ConstantBufferManager(nullptr)
    , m_RenderPipeline(nullptr)
    , m_AnimatedModelManager(nullptr)
    , m_Clock()
    , m_PrevTime(m_Clock.now())
{
    deltaTime = 0;
    cameraSelector = 0;
    l_prevCameraSelector = 0;
}

void CEngine::Init(HWND hWnd)
{
    ImGui_ImplDX11_Init(hWnd, m_RenderManager->GetDevice(), m_RenderManager->GetDeviceContext());

    m_ConstantBufferManager = new CConstantBufferManager();

    m_ShaderManager = new CShaderManager();
    m_ShaderManager->Load("data/shaders.xml"); // #TODO properties

    m_EffectManager = new CEffectManager();
    m_EffectManager->Load("data/effects.xml");

    m_TechniquePoolManager = new CTechniquePoolManager();
    m_TechniquePoolManager->Load("data/techniques_pool.xml");
    m_TechniquePoolManager->Apply("forward");

    std::string lLevelMaterialsFilename = "reclusion.xml"; // #TODO nombres hardcoded
    std::string lDefaultMaterialsFilename = "default.xml";

    m_TextureManager = new CTextureManager();

    m_MaterialManager = new CMaterialManager();
    m_MaterialManager->Load(lLevelMaterialsFilename, lDefaultMaterialsFilename);



    m_AnimatedModelManager = new CAnimatedModelManager();
    m_AnimatedModelManager->Load("data/animated_models.xml");

    m_MeshManager = new CMeshManager();
    // m_MeshManager->GetMesh("data/meshes/Plane001.mesh"); // #TODO

    m_LightManager = new CLightManager();
    m_LightManager->Load("data/lights.xml");

    m_SceneManager = new CSceneManager();
    m_SceneManager->Load("data/scenes.xml");
}

void CEngine::ProcessInputs()
{
    ImGui_ImplDX11_NewFrame();
    m_ActionManager->Update();
}

void CEngine::Update()
{

    auto currentTime = m_Clock.now();
    std::chrono::duration<float> chronoDeltaTime = currentTime - m_PrevTime;
    m_PrevTime = currentTime;

    float dt = chronoDeltaTime.count() > 0.5f ? 0.5f : chronoDeltaTime.count();
    deltaTime = dt;

    // Reiniciem posició de l'esfera quan canviem de camera
    if (cameraSelector != l_prevCameraSelector)
    {
        m_RenderManager->m_SphereOffset = Vect3f(0, 0, 0);
    }

    l_prevCameraSelector = cameraSelector;

    switch (cameraSelector)
    {
    case 0: //Orbital
        SetCameraController(&orbitalCam);
        orbitalCameraUpdate(*m_CameraController, m_ActionManager, dt);

        break;
    case 1: //FPS
        SetCameraController(&fpsCam);
        fpsCameraUpdate( *m_CameraController,m_ActionManager, dt);

        break;
    case 2: //TPS
        SetCameraController(&tpsCam);
        sphereUpdate(*m_RenderManager, m_ActionManager, m_CameraController->getFront(), m_CameraController->getUp());
        tpsCameraUpdate(*m_CameraController, m_ActionManager, m_RenderManager->m_SphereOffset, dt);

        break;
    default:
        break;
    }

    m_CameraController->Update(dt);

    m_CameraController->SetToRenderManager(*m_RenderManager);

    m_SceneManager->Update(dt);
}

void CEngine::Render()
{
    m_RenderPipeline->Execute();
}

void CEngine::fpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt)
{

    /*ImGui::Text("Mou la camera amb WASD.");
    ImGui::Text("Orienta't amb el ratoli.");
    ImGui::Text("Vigila el cap.");*/
    CFpsCameraController *fpsCamera = static_cast<CFpsCameraController*>(&camera);
    fpsCamera->xSpeed = 0.1f * (*actionManager)("x_move")->value;
    fpsCamera->zSpeed = 0.1f * (*actionManager)("z_move")->value;

    fpsCamera->yawSpeed = 0.1f * (*actionManager)("pitch")->value;
    fpsCamera->pitchSpeed = -0.1f * (*actionManager)("yaw")->value;

    fpsCamera->Update(dt);
}

void CEngine::orbitalCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt)
{

    /*ImGui::Text("Gira la roda del mouse per controlar el zoom.");
    ImGui::Text("Apreta qualsevol boto del ratoli per habilitar la rotacio.");*/
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

    /*ImGui::Text("Mou l'esfera amb WASD");
    ImGui::Text("Orienta't amb el ratoli.");
    ImGui::Text("Amb la roda del ratoli pots controlar el zoom.");*/
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