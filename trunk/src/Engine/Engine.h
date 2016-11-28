#pragma once

#ifndef _ENGINE_ENGINE_CPB_20161127_H
#define _ENGINE_ENGINE_CPB_20161127_H

#include "Base\Utils\Singleton.h"
#include <chrono>

class CRenderManager;
class CActionManager;
class CCameraController;
class CMaterialManager;
class CSceneManager;

#define BUILD_GET_SET_ENGINE_MANAGER( Manager ) \
private: \
C##Manager* m_##Manager; \
public: \
void Set##Manager(C##Manager* a##Manager) { m_##Manager = a##Manager;  } \
C##Manager& Get##Manager() { return *m_##Manager; } \

class CEngine : public base::utils::CSingleton<CEngine> {

public:

	CEngine();
	virtual ~CEngine();

	void ProcessInputs();
	void Update();
	void Render();

	BUILD_GET_SET_ENGINE_MANAGER(MaterialManager);
	BUILD_GET_SET_ENGINE_MANAGER(RenderManager);
	BUILD_GET_SET_ENGINE_MANAGER(CameraController);
	BUILD_GET_SET_ENGINE_MANAGER(SceneManager);
	BUILD_GET_SET_ENGINE_MANAGER(ActionManager);

private:

	std::chrono::monotonic_clock m_Clock;
	std::chrono::monotonic_clock::time_point m_PrevTime;
};

#undef BUILD_GET_SET_ENGINE_MANAGER

#endif //_ENGINE_ENGINE_CPB_20161127_H