#pragma once
#ifndef _ENGINE_FPSCAMERACONTROLLER_CPB_20161127_H
#define _ENGINE_FPSCAMERACONTROLLER_CPB_20161127_H

#include "CameraController.h"

class CFpsCameraController : public CCameraController {
public:
	Vect3f center;
	float xSpeed, ySpeed, zSpeed;
	float yawSpeed, pitchSpeed, rollSpeed;
	float zoomSpeed;

	CFpsCameraController(Vect3f center = Vect3f(0, 1, 0), float maxPitch = 1.5f, float minPitch = -1.5f)
		: center(center)
		, maxPitch(maxPitch)
		, minPitch(minPitch)
		, xSpeed(0.0f)
		, ySpeed(0.0f)
		, zSpeed(0.0f)
		, yawSpeed(0.0f)
		, pitchSpeed(0.0f)
		, rollSpeed(0.0f)
		, zoomSpeed(0.0f)
		, yaw(0.0f)
		, pitch(0.0f)
		, roll(0.0f)
		, maxDistance(5.0f)
	{};
	virtual ~CFpsCameraController(){};

	virtual void Update(float ElapsedTime) override;

private:
	
	float yaw, pitch, roll;
	float maxPitch, minPitch;
	float maxDistance;
};

#endif //_ENGINE_FPSCAMERACONTROLLER_CPB_20161127_H
