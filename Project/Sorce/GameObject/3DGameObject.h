#pragma once
#include "GameObject.h"
#include <string>

// プレイヤーのアニメーションの種類
enum PlayAnimationType_Player
{
	PLAYER_GRAVCONTROLLING,
	PLAYER_JUMPING,
	PLAYER_WALKING,
	PLAYER_ATTACKING,
	PLAYER_IDLEING,
	PLAYER_STRAFE_L,
	PLAYER_STRAFE_R,
	PLAYER_SHOTING,
	PLAYER_GETHITTING,
};

// ラッシュエネミーのアニメーションの種類
enum PlayAnimationType_RushEnemy
{
	RUSHENEMY_RUSHATTACKING,
	RUSHENEMY_RUSHCHARGEING,
	RUSHENEMY_WALKING,
	RUSHENEMY_IDLEING,
	RUSHENEMY_ATTACKING,
	RUSHENEMY_FLOATING,
};

// ショットエネミーのアニメーションの種類
enum PlayAnimationType_ShotEnemy
{
	SHOTENEMY_IDLEING,
	SHOTENEMY_SHOTING,
	SHOTENEMY_WALKING,
	SHOTENEMY_RUNNING,
	SHOTENEMY_FLOATING,
};

// ボタンのアニメーションの種類
enum PlayAnimationType_Button
{
	BUTTON_BUCKDEFALUT,
	BUTTON_PUSH,
};

struct GravityControlCommand
{
	// 力の方向
	VECTOR ForceDir;
	// 力の大きさ
	float ForcePower;
	// 掴み継続中
	bool IsHolding;
	// 解放フラグ
	bool Isrelease;
	// 発射されたフラグ
	bool IsShot;
};

struct CollisionResult;
class  CollisionBase;
class  Capsule;

// 3DGameObjectクラス
class GameObject3D : public GameObject
{
public:
	GameObject3D(); // コンストラクタ
	~GameObject3D(); // デストラクタ

public:
	virtual void Load() {} // ロード関数
	virtual void Start() {} // スタート関数
	virtual void Step() {} // ステップ関数
	void Update()override; // アップデート関数
	void Draw()override; // 描画関数
	void Fin()override; // 終了関数

public:
	// ゲッター、セッター関数
	VECTOR GetPos3D() const { return m_Transform.position_3D; }
	VECTOR GetScale() const { return m_Transform.scale; }
	VECTOR GetGravityDir() const { return m_GravityDir; }
	Quaternion GetRotation() const { return m_Transform.rotation; }
	bool GetActive() { return m_IsActive; }
	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool GetIsFall() { return m_IsFall; }
	void SetIsFall(bool isFall) { m_IsFall = isFall; }
	void SetMove (VECTOR move)  { m_Move = move; }
	void SetID(const std::string& id) { m_ID = id; }
	const std::string& GetID() const { return m_ID; }
	virtual std::string GetTypeName() const { return ""; }
	GravityControlCommand GetGravityCommand() const { return m_GravityCommand; }
	void SetTypeName(const std::string& typeName) { m_TypeName = typeName; }
	std::string GetTypeName() { return m_TypeName; }
	void SetTransform(VECTOR pos, Quaternion rot, VECTOR scale) {
		m_Transform.position_3D = pos;
		m_Transform.rotation = rot;
		m_Transform.scale = scale;
	}
	int GetHP() { return m_HP; }
	void SetIsHitAttack(bool isHit) { m_IsHitAttack = isHit; }
	bool GetIsHitAttack() const { return m_IsHitAttack; }
	void SetIsHitAttackFrame(int hitFrame) { m_HitAttackFrame = hitFrame; }

	// ダメージ処理関数
	void HitDamage(int);


// 当たり判定関連
	// ステージとの当たり判定関数
	virtual void CheckStageCollision();
	// 当たり判定コライダー取得
	CollisionBase* GetCollision() const { return m_Collision; }
	// ターゲット用当たり判定取得
	CollisionBase* GetTargetCollision() const { return m_TargetCollision; }

	// 他オブジェクトととの押し出し処理
	void HitBodyAnyObject(CollisionResult*);
	// 衝突情報更新関数
	void UpdateCollision(bool);
	// 接地判定関数
	bool IsGrounded();

	// 重力操作コマンド適用関数
	void ApplyGravityCommand(GravityControlCommand cmd);

protected:
// アニメーション関連
    template<typename AnimType>
	// 継承先でアニメーションを再生する関数
	void PlayAnimation(AnimType anim, bool isLoop)
	{
		// enum を int にキャストして共通処理
		int animIndex = static_cast<int>(anim);

		// ここで実際にアニメーション再生する処理
		SetFrame(animIndex, isLoop); // 仮の関数
	}
	
	// 実際にアニメーションを再生する関数
	void SetFrame(int animIndex, bool isLoop);

	// プレイヤーに操られる関数
	virtual void GravityModeUpdate();

// 当たり判定関連
	// 当たり判定との同期関数
	virtual void SyncCollision(){}
	virtual void OnCollisionEnter() {}
	virtual void OnCollisionExit() {}

	// 他オブジェクトとの当たり判定
	virtual bool CheckCollision(GameObject3D* other) const;

	virtual void Die() {}
private:
	// アニメーション再生を進める関数
	void UpdateAnimation();

protected:
// アニメーション関連変数
	// アニメーションのアタッチインデックス
	int m_AnimationAttachIndex;
	// アニメーションの再生スピード
	float m_AnimationSpeed;
	// 再生中のアニメーションの総時間
	float m_AnimationTotalTime;
	// 再生中のアニメーションの再生時間
	float m_AnimationNowTime;
	// アニメーションループフラグ
	bool m_IsLoopAnimation;
	// 再生中のアニメーション
	int m_NowAnimation;

// 移動、回転関連関数
	// 移動量
	VECTOR m_Move;
	// 重力方向
	VECTOR m_GravityDir;
	// 前回の位置
	VECTOR m_PrevPos;
	// 上ベクトル
	VECTOR m_Up;
	// 前ベクトル
	VECTOR m_Forward;
	// 右ベクトル
	VECTOR m_Right;

// フラグ
	// 生存フラグ
	bool m_IsActive;
	// ジャンプフラグ
	bool m_IsJump;
	// 重力変更開始フラグ
	bool m_IsStartChangeGravity;	
	// 壁に当たっているフラグ
	bool m_IsHitWall;
	// 攻撃受けたフラグ
	bool m_IsHitAttack;

// 当たり判定関係関数
	// 汎用当たり判定
	CollisionBase* m_Collision;
	// ターゲット用当たり判定
	CollisionBase* m_TargetCollision;
	// ステージ用当たり判定
	Capsule* m_CapsuleCollision;
	bool m_WasColliding = false;
	bool m_EnableFloorCheck = true;

	// 重力操作コマンド
    GravityControlCommand m_GravityCommand;
	// オブジェクトID
	std::string m_ID;
	// オブジェクトのタイプ名
	std::string m_TypeName;
	// HP
	int m_HP;
	// 最大HP
	int m_MaxHP;

	// 無敵時間中の点滅時間
	int m_HitAttackFrame;
};
