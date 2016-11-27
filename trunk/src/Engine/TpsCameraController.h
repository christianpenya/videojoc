#pragma once
#ifndef _ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H
#define _ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H

#include "CameraController.h";

class CTpsCameraController : public CCameraController {
public:
	Vect3f center;
	float yawSpeed, pitchSpeed, rollSpeed;
	float zoomSpeed;

	CTpsCameraController(Vect3f center = Vect3f(0, 0, 0), float maxPitch = 1.5f, float minPitch = -1.5f, float maxZoom = 35.0f, float minZoom = 8.0f)
		: center(center)
		, maxPitch(maxPitch)
		, minPitch(minPitch)
		, maxZoom(maxZoom)
		, minZoom(minZoom)
		, yawSpeed(0.0f)
		, pitchSpeed(0.0f)
		, rollSpeed(0.0f)
		, zoomSpeed(0.0f)
		, yaw(0.0f)
		, pitch(-1.0f)
		, roll(0.0f)
		, zoom(10.0f)
	{};
	virtual ~CTpsCameraController(){};

	virtual void Update(float ElapsedTime) override;
	Vect3f getFront() { return m_Front; }
	Vect3f getUp() { return m_Up; }

private:
	float yaw, pitch, roll, zoom;

	float maxPitch, minPitch;
	float maxZoom, minZoom;

};

#endif //_ENGINE_TPSCAMERACONTROLLER_CPB_20161127_H
