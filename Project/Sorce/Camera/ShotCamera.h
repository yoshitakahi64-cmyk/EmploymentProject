#pragma once

#include "Camera.h"

class ShotCamera : public CameraState
{
public:
	ShotCamera(); // コンストラクタ
	~ShotCamera() override; // デストラクタ

public:
	void Enter(Camera& camera) override;
	void Update(Camera& camera) override;
	void Exit(Camera& camera) override;

private:
	void Input(CameraParame& parame);
};