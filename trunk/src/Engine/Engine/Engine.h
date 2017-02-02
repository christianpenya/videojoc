#pragma once

#ifndef _ENGINE_ENGINE_CPB_20161127_H
#define _ENGINE_ENGINE_CPB_20161127_H
#include "Render\RenderManager.h"
#include "Utils\Singleton.h"
#include "Camera\SphericalCameraController.h"
#include "Camera\FpsCameraController.h"
#include "Camera\TpsCameraController.h"
#include <chrono>
#include "ImGUI\imgui.h"
#include "imgui_impl_dx11.h"

class CRenderManager;
class CInputManager;
class CActionManager;
class CCameraController;
class CTextureManager;
class CMaterialManager;
class CSceneManager;
class CShaderManager;
class CEffectManager;
class CMeshManager;
class CTechniquePoolManager;
class CLightManager;
class CRenderPipeline;
class CConstantBufferManager;
class CAnimatedModelManager;

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Exist##Manager() { return m_##Manager != nullptr; } \

class CEngine : public base::utils::CSingleton<CEngine>
{

public:

    CEngine();
    virtual ~CEngine()
    {
        ImGui_ImplDX11_Shutdown();
    };

    void ProcessInputs();
    void Update();
    void Render();
    void Init(HWND hWnd);

    BUILD_GET_SET_ENGINE_MANAGER(MaterialManager);
    BUILD_GET_SET_ENGINE_MANAGER(TextureManager);
    BUILD_GET_SET_ENGINE_MANAGER(RenderManager);
    BUILD_GET_SET_ENGINE_MANAGER(CameraController);
    BUILD_GET_SET_ENGINE_MANAGER(SceneManager);
	BUILD_GET_SET_ENGINE_MANAGER(InputManager);
	BUILD_GET_SET_ENGINE_MANAGER(ActionManager);
    BUILD_GET_SET_ENGINE_MANAGER(ShaderManager);
    BUILD_GET_SET_ENGINE_MANAGER(EffectManager);
    BUILD_GET_SET_ENGINE_MANAGER(MeshManager);
    BUILD_GET_SET_ENGINE_MANAGER(TechniquePoolManager);
    BUILD_GET_SET_ENGINE_MANAGER(LightManager);
    BUILD_GET_SET_ENGINE_MANAGER(RenderPipeline);
    BUILD_GET_SET_ENGINE_MANAGER(ConstantBufferManager);
    BUILD_GET_SET_ENGINE_MANAGER(AnimatedModelManager);

    CFpsCameraController fpsCam;
    CTpsCameraController tpsCam;
    CSphericalCameraController orbitalCam;
//private:

    std::chrono::monotonic_clock m_Clock;
    std::chrono::monotonic_clock::time_point m_PrevTime;
    float deltaTime;
    int cameraSelector;
    int l_prevCameraSelector;
    void fpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt);
    void tpsCameraUpdate(CCameraController& camera, CActionManager* actionManager, Vect3f sphereCenter, float dt);
    void orbitalCameraUpdate(CCameraController& camera, CActionManager* actionManager, float dt);
    void sphereUpdate(CRenderManager& renderManager, CActionManager* actionManager, Vect3f front = Vect3f(0, 0, 1), Vect3f up = Vect3f(0, 1, 0));
    void sphereRender(CRenderManager& renderManager);
};

#undef BUILD_GET_SET_ENGINE_MANAGER

#endif //_ENGINE_ENGINE_CPB_20161127_H
