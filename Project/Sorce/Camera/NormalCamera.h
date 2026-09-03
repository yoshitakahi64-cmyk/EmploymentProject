#pragma once

#include "Camera.h"

// 通常カメラ状態クラス
class NormalCamera : public CameraState
{
public:
	NormalCamera(); // コンストラクタ
	~NormalCamera() override; // デストラクタ

public:
	void Enter(Camera& camera) override;
	void Update(Camera& camera) override;
	void Exit(Camera& camera) override;

private:
	void Input(CameraParame& parame);
};