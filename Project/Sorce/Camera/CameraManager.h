#pragma once

#include"../Singleton.h"
#include "../Memory.h"
#include "NormalCamera.h"
#include "TargetCamera.h"
#include "DebugCamera.h"
#include "ShotCamera.h"

class Camera;

class CameraManager : public Singleton<CameraManager>
{
public:
	CameraManager(); // コンストラクタ
	~CameraManager(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Update(); // アップデート関数
	void Draw(); // 描画関数

public:
	// カメラの切り替え関数
	void SetNormal();
	void SetShot();
	void SetTarget();
	void SetDebug();

	// カメラの使用フラグをリセットする関数
	void ResetUseCameraFlags() { m_UseNormalCamera = false; m_UseShotCamera = false; m_UseTargetCamera = false; }

public:
	// カメラの生成と取得関数
	void CreateCamera();

	// カメラのゲッター関数
	const UniquePtr<Camera>& GetCamera() const { return  m_Camera; }
	void GetUseFlag(bool& useNormalCamera, bool& useShotCamera, bool& useTargetCamera) const
	{
		useNormalCamera = m_UseNormalCamera;
		useShotCamera = m_UseShotCamera;
		useTargetCamera = m_UseTargetCamera;
	}
	bool GetUseTargetCamera() { return m_UseTargetCamera;}
	bool GetUseNormalCamera() { return m_UseNormalCamera; }

private:
	// カメラのインスタンス
	UniquePtr<Camera> m_Camera;
	// 各カメラの状態
	NormalCamera m_NormalCamera;
	ShotCamera m_ShotCamera;
	TargetCamera m_TargetCamera;
	DebugCamera m_DebugCamera;

	// 各カメラの使用フラグ
	bool m_UseNormalCamera;
	bool m_UseShotCamera;
	bool m_UseTargetCamera;
};
