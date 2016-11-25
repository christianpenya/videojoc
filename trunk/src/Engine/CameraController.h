#pragma once

#include "Engine\Render\RenderManager.h"

class CCameraController
{
public:
	CCameraController() :m_Position(0, 0, 0), m_Front(0, 0, 1), m_Up(0, 1, 0) {}
	virtual ~CCameraController() {}
	virtual void Update(float ElapsedTime) = 0;

	void SetToRenderManager(engine::render::CRenderManager &_RenderManager) const
	{
		_RenderManager.SetViewMatrix(m_Position, m_Position + m_Front, m_Up);
	}

protected:
	Vect3f 					m_Position;
	Vect3f 					m_Front;
	Vect3f 					m_Up;
};