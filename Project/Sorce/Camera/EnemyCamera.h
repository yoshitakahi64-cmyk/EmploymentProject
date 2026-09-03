#pragma once

#include "Camera.h"

class EnemyCamera : public CameraState
{
public:
	EnemyCamera();
	~EnemyCamera() override;

public:
	void Enter(Camera& camera) override;
	void Update(Camera& camera) override;
	void Exit(Camera& camera) override;

private:
	void Input(CameraParame& parame);
};