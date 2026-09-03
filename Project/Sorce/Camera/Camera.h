#pragma once
#include <DxLib.h>

class Camera;
class Enemy;

// カメラのパラメータ構造体
struct CameraParame
{
	// カメラの位置、回転、注視点
	VECTOR pos;
	VECTOR rot;
	VECTOR target;

	// 注視点の目標値
	VECTOR targetPos;

	// カメラの基底ベクトル
	VECTOR forward;
	VECTOR up;
	VECTOR right;

	// 注視点からの距離と高さ
	VECTOR lookDir;
	bool isUseLookDir;

	// カメラの距離と高さ
	float distance;
	float height;

	// カメラの距離と高さの目標値
	float targetdistance;
	float targetHeight;

	// カメラの回転の目標値
	float targetRotX;
	float targetRotY;
};

// カメラ状態の基底クラス
class CameraState
{
public:
	virtual ~CameraState() = default;
	virtual void Enter(Camera& camera) = 0;
	virtual void Update(Camera& camera) = 0;
	virtual void Exit(Camera& camera) = 0;
};

class Camera
{
public:
	Camera(); // コンストラクタ
	~Camera(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Update(); // アップデート関数
	void Draw(); // 描画関数

	// 状態遷移
	void ChangeState(CameraState* newState);

public:
	// ゲッター・セッター関数
	CameraParame GetParame() const { return m_Parame; }
	void SetParame(const CameraParame& parame) { m_Parame = parame; }
	void SetControllEnemy(Enemy* enemy) { m_Enemy = enemy; }

public:
	// ビュー判定関連関数
	bool IsInView(const VECTOR& pos) const;
	bool IsOutOfView(const VECTOR& pos) const;
	VECTOR GetScreenPos(const VECTOR& worldPos)const;
	bool IsInViewWithMargin(const VECTOR& pos, float margin = 100.0f)const;

public:
	// ヒットエフェクト発生関数
	void AddHitEffect(float duration, float shakePower, float zoomPower);

	// カメラとエネミーの間に障害物があるか検知する関数
	bool HitCkeckCameraToEnemyRay();

private:
	// カメラの位置と注視点を再計算する関数
	void RecalculateTransform();
	// カメラの当たり判定を解決する関数
	void SoliveCollisions();

private:
	// カメラのパラメータ
	CameraParame m_Parame;
	// 現在のカメラ状態
	CameraState* m_CurrentState;

	// ヒットエフェクト用
	// 残り時間
	float m_HitEffectTimer;
	// 発動時の合計時間
	float m_HitEffectDuration;
	// シェイクの強さ
	float m_HitShakePower;
	// ズームパンチの強さ
	float m_HitZoomPower;
	// ヒットエフェクトの方向
	VECTOR m_HitEffectDir;


	// 操作対象の敵
	Enemy* m_Enemy;
};