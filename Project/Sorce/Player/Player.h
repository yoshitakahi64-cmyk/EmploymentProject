#pragma once
#include <DxLib.h>
#include "../GameObject/3DGameObject.h"
#include "../Input/Input.h"
#include "../Shot/Shot.h"
#include "../Container.h"


class Capsule;
class CollisionAABB;
class Camera;
class Enemy;
class StageObject;

// プレイヤーの操作モード
enum PlayerMode
{
	MODE_NORMAL,
	MODE_GRAVITY,
	MODE_CONTROLOBJECT,
	MODE_NONE = -1,
};

// プレイヤーのショットモード
enum ShotMode
{
	SHOT_NORMAL,
	SHOT_TARGET,
	SHOT_MODE_MAX,
};

class Player : public GameObject3D
{
public:
	Player(); // コンストラクタ
	~Player(); // デストラクタ

public:
	void Load()override; // ロード関数
	void Start()override; // スタート関数
	void Step()override; // ステップ関数

public:
	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Player"; }
	PlayerMode GetMode() const { return m_Mode; }
	ShotMode GetShotMode() const { return m_ShotMode; }
	void SetIsJamp(bool isJump) { m_IsJump = isJump; }
	void SetControlEnemy(Enemy* enemy) { m_ControlEnemy = enemy; }
	void SetMode(PlayerMode mode) { m_Mode = mode; }
	void SetShotMode(ShotMode mode) { m_ShotMode = mode; }
	void SetGetDamage(bool isDamaged) { m_IsDamaged = isDamaged; }
	void SetIsControllEnemy(bool isControll) { m_IsControllEnemy = isControll; }

	// 接地判定更新関数
	void UpdateGroundState();

private:
	// 入力関数
	void Input();
	// ターゲット操作関数
	void HandleTargetInput();
	// プレイヤー移動関数
	void Move();
	// プレイヤージャンプ関数
	void Jump();
	// プレイヤーアタック入力関数
	void Attack();
	// プレイヤーダメージを受けてるかどうかのフラグを更新する関数
	void UpdateDamageFlag();
	// アニメーション管理関数
	void ControllAnimation();
	// 当たり判定同期関数
	void SyncCollision()override;
	// プレイヤーの回転更新
	void UpdateRotate(float angle);
	// ショットモード変更関数
	void ChangeShotMode(ShotMode);
	// コントロール可能な敵を取得
	void GetEnableControllEnemy();
	// 敵コントロール更新関数
	void CheckControllEnemy();
	// 死亡関数
	void Die()override;

// 重力操作関係
	// 重力玉発射関数
	void ShootGravityOrb();
	// オブジェクト重力操作入力関数
	void HandleObjectGravityInput();
	// 敵追従関数
	void FollowEnemy(VECTOR& dir,float& force);

private:
    // ほかクラス参照変数
	Camera* m_Camera;
	Shot m_Shot;

	// コントロール可能な敵のリスト
	List<Enemy*> m_EnableControllEnemy;

// 入力関係
	// 上下入力
	float m_InputV;
	// 左右入力
	float m_InputH;
	// 左スティック入力
	LeftStick m_inputLeftState;
	// 右スティック入力
	RightStick m_inputRightState;

// プレイヤー操作関係
    // 入力終了時の移動方向
	VECTOR m_LastMoveDir;
    // プレイヤーの操作モード
	PlayerMode m_Mode;
	// プレイヤーのスピード
	float m_MoveSpeed;
	// プレイヤーのジャンプ力
	float m_JumpForce;
	// ジャンプ入力フラグ
	bool m_InputJump;
	// 接地判定無視の猶予
	float m_CoyoteTimer;
	// プレイヤーの移動方向
	VECTOR m_MoveDir;
	// プレイヤーの射撃モード
	ShotMode m_ShotMode;

// 重力関係
	// 重力の強さ
	float m_GravityPower;
	// 重力ローテーション
	Quaternion m_GravityRotation;
	// 操作できる敵
	Enemy* m_ControlEnemy;
	// 重力球リキャスト時間
	float m_AbleReShotTime;
	VECTOR m_ControllForceDir;
	float m_ControllForce;
	// 敵操作中フラグ
	bool m_IsControllEnemy;

// カメラ関係
	bool m_UseNormalCamera;
	bool m_UseTargetCamera;
	bool m_UseShotCamera;

// アニメーションフラグ
	// 歩くフラグ
	bool m_IsWalk;
	// 攻撃フラグ
	bool m_IsAttack;
	// 射撃フラグ
	bool m_IsShotting;
	// ダメージを受けたフラグ
	bool m_IsDamaged;
};
