#pragma once


#include "CameraController.h";

class SphericalCameraController : public CCameraController {
public:
	Vect3f center;
	float yawSpeed, pitchSpeed, rollSpeed;
	float zoomSpeed;

	SphericalCameraController(Vect3f center = Vect3f(0, 0, 0), float maxPitch = 1.5f, float minPitch = -1.5f, float maxZoom = 20, float minZoom = 1)
	: center(center)
	, maxPitch(maxPitch)
	, minPitch(minPitch)
	, maxZoom(maxZoom)
	, minZoom(minZoom)
	, yawSpeed(0)
	, pitchSpeed(0)
	, rollSpeed(0)
	, zoomSpeed(0)
	, yaw(1.0f)
	, pitch(1.0f)
	, roll(0.0)
	, zoom(10.0)
	{};
	virtual ~SphericalCameraController(){};

	virtual void Update(float ElapsedTime) override;

private:
	float yaw, pitch, roll, zoom;

	float maxPitch, minPitch;
	float maxZoom, minZoom;
};