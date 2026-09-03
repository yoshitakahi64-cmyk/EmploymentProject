#pragma once

#include <DxLib.h>
#include "../GameObject/3DGameObject.h"
#include "../Memory.h"

class Capsule;
class CollisionAABB;
class Player;
class UIEnemyHP;

// 敵の状態列挙体
enum  EnemyState
{
    IDEL,
	CHASE,
	ATTACK,
	RUSHCHARGE,
	RUSHATTACK,
	SHOT,
	ESCAPE,
	GRAVTYMODE,
	STATE_MAX,
};

// 敵の基底クラス
class Enemy : public GameObject3D
{
public:
	Enemy(); // コンストラクタ
	~Enemy() = default; // デストラクタ

public:
	virtual void Load() {}    // ロード関数
	void Start()override;     // スタート関数
	virtual void OnStart() {} // 継承先のスタート関数
	void Step()override;      // ステップ関数

	// クローン関数
	virtual  UniquePtr<Enemy> Clone() { return nullptr; }

	// ゲッター・セッター関数
	void SetState(EnemyState enemyState) { m_NextState = enemyState; }
	void SetEnableTarget(bool enable) { m_EnableTarget = enable; }
	bool GetEnableTarget() const { return m_EnableTarget; }
	void SetEnableControll(bool enable) { m_EnableControll = enable; }
	bool GetEnableControll()const { return m_EnableControll; }
	void SetIsTargeting(bool isTargeting) { m_IsTargeting = isTargeting; }
	bool GetIsTargeting() const { return m_IsTargeting; }
	void SetIsControlling(bool isControlling) { m_IsControlling = isControlling; }
	bool GetIsControlling() const { return m_IsControlling; }

// 当たり判定関数
	// 攻撃の当たり判定を取得
	CollisionAABB* GetAttackCollison() { return m_AttackCollison; }
	CollisionAABB* GetRushCollision() { return m_RushCollison; }

	// プレイヤーとの攻撃当たり判定
	virtual void HitAttack(){}
	// プレイヤーとの突進当たり判定
	virtual void HitRush(){}

	// 消滅関数
	void Vanish();
protected:
// 状態更新関数
	virtual void UpdateState(float distance){}

	// 攻撃関数
	virtual void Attack(){}

// RushEnemy用の仮想関数
	// 突進チャージ関数
	virtual void RushCharge(){}
	// 突進攻撃関数
	virtual void RushMove(){}

// ShotEnemy用の仮想関数
	// 射撃関数
	virtual void EnemyShot(){}
	// 逃走関数
	virtual void EnemyEscape(){}

private:
	// 待機関数
	void Idel();
	// 移動関数
	void Move();

	// 関数ポインタ
	using StateFunc = void (Enemy::*)();

	// アニメーション管理関数
	virtual void ControllAnimation(){}
	// 当たり判定同期関数
	void SyncCollision()override;
	// 重力操作の更新関数
	void GravityModeUpdate()override;

	// 死亡処理関数
	void Die()override;

protected:
// 移動関係
	// プレイヤーとの距離
	float m_distance;
// 攻撃関係
	float m_ReAttackTime;  
// 他のソースの参照
	// プレイヤー
	Player* m_Player;
// フラグ
	// 敵の状態
	EnemyState m_State;
	// 敵の次の状態
	EnemyState m_NextState;
	// 攻撃ヒットフラグ
	bool m_HitAttack;
// 当たり判定関係
	// Attack用当たり判定
	CollisionAABB* m_AttackCollison;
	// 突進用当たり判定
	CollisionAABB* m_RushCollison;

	// 消滅待機タイマー
	float m_VanishTime;

	// 操作中フラグ
	bool m_IsControlling;

	UIEnemyHP* m_UIEnemyHP;
private:
	// 敵の状態関数ポインタ
	StateFunc m_StateTable[STATE_MAX];

	// 操作可能フラグ
	bool m_EnableControll;
	// ターゲット可能フラグ
	bool m_EnableTarget;
	// ターゲット中フラグ
	bool m_IsTargeting;
	// 消滅待機タイマー操作可能フラグ
	bool m_IsVanishWaitTimer;

	// エフェクトインターバル
	float m_EffectInterval;
};
