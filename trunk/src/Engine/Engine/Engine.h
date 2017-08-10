#pragma once

#ifndef _ENGINE_ENGINE_CPB_20161127_H
#define _ENGINE_ENGINE_CPB_20161127_H

#include "Render/RenderManager.h"
#include "Utils/Singleton.h"

#include <chrono>
#include "Imgui/imgui_impl_dx11.h"
#include "Utils/StringUtils.h"

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
class CCinematicManager;
class CTechniquePoolManager;
class CLightManager;
class CRenderPipeline;
class CConstantBufferManager;
class CAnimatedModelManager;
class CScriptManager;
class CPhysXManager;
class CParticleManager;
class ISoundManager;//class CNavMeshManager;
class CGUIManager;
class CCameraManager;
class CCharacterController;

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager = nullptr; \
public: \
std::string m_File##Manager; \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
C##Manager& Get##Manager() { return *m_##Manager; } \
bool Exist##Manager() { return m_##Manager != nullptr; } \

class CEngine : public base::utils::CSingleton<CEngine>
{

public:

    CEngine();
    ~CEngine();

    void ProcessInputs() const;
    void Update();
    void Render();
    void Init(HWND hWnd);
    void LoadFiles();

    std::string m_FileDefaultMaterial;
    std::string m_SoundFilesPath;
    std::string m_SpeakersFile;
    std::string m_BanksFile;

    BUILD_GET_SET_ENGINE_MANAGER(MaterialManager)
    BUILD_GET_SET_ENGINE_MANAGER(TextureManager)
    BUILD_GET_SET_ENGINE_MANAGER(RenderManager)
    BUILD_GET_SET_ENGINE_MANAGER(SceneManager)
    BUILD_GET_SET_ENGINE_MANAGER(InputManager)
    BUILD_GET_SET_ENGINE_MANAGER(ActionManager)
    BUILD_GET_SET_ENGINE_MANAGER(ShaderManager)
    BUILD_GET_SET_ENGINE_MANAGER(EffectManager)
    BUILD_GET_SET_ENGINE_MANAGER(MeshManager)
    BUILD_GET_SET_ENGINE_MANAGER(TechniquePoolManager)
    BUILD_GET_SET_ENGINE_MANAGER(LightManager)
    BUILD_GET_SET_ENGINE_MANAGER(RenderPipeline)
    BUILD_GET_SET_ENGINE_MANAGER(ConstantBufferManager)
    BUILD_GET_SET_ENGINE_MANAGER(AnimatedModelManager)
    BUILD_GET_SET_ENGINE_MANAGER(ScriptManager)
    BUILD_GET_SET_ENGINE_MANAGER(CinematicManager)
    BUILD_GET_SET_ENGINE_MANAGER(PhysXManager)
    BUILD_GET_SET_ENGINE_MANAGER(ParticleManager)
    //  BUILD_GET_SET_ENGINE_MANAGER(NavMeshManager)
    BUILD_GET_SET_ENGINE_MANAGER(GUIManager)
    BUILD_GET_SET_ENGINE_MANAGER(CameraManager)

    ISoundManager* m_SoundManager;

    double m_DeltaTime;
    clock_t m_DeltaTimeAcum = 0;
    unsigned int m_Frames;
    double m_FPS;

    CCharacterController* m_CharacterController;
    // void CharacterControllerUpdate(CActionManager* aActionManager, float dt);
    void DrawImgui();
};

#undef BUILD_GET_SET_ENGINE_MANAGER

#endif //_ENGINE_ENGINE_CPB_20161127_H

