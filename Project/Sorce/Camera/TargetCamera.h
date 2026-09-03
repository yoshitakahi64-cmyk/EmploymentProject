#pragma once

#include "Camera.h"

class TargetCamera : public CameraState
{
public:
	TargetCamera(); // コンストラクタ
	~TargetCamera() override; // デストラクタ

public:
	void Enter(Camera& camera) override;
	void Update(Camera& camera) override;
	void Exit(Camera& camera) override;

private:
	// カメラの回転を計算する関数
	void CalcRot(CameraParame& camParam);
	void Input(CameraParame& parame);
};