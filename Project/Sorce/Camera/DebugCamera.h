#pragma once

#include "Camera.h"

class DebugCamera : public CameraState
{
public:
	DebugCamera();
	~DebugCamera() override;

public:
	void Enter(Camera& camera) override;
	void Update(Camera& camera) override;
	void Exit(Camera& camera) override;
};