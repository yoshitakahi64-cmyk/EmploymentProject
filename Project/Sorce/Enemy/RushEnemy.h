#pragma once
#include <DxLib.h>
#include "../Enemy/Enemy.h"

class CollisionAABB;


// 突進攻撃を行う敵クラス
class RushEnemy : public Enemy
{
public:
	RushEnemy(); // コンストラクタ
	~RushEnemy() = default; // デストラクタ

public:
	void Load()override; // ロード関数
	void OnStart()override; // スタート関数
	void Update()override { Enemy::Update(); } // アップデート関数

	// クローン関数
	UniquePtr<Enemy> Clone()override;

	// アニメーション管理関数
	void ControllAnimation()override;

	// 突進攻撃と攻撃の当たり判定に当たったときの処理関数
	void HitRush()override;
	void HitAttack()override;

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "RushEnemy"; }
	

private:
	// 状態更新
	void UpdateState(float distance)override;

	// 突進攻撃
	void Attack()override;
	// 突進移動
	void RushMove()override;
	// 突進チャージ
	void RushCharge()override;

private:
	// 突進チャージ時間
	float m_RushChargeTime;
	// 突進終了時間
	float m_FinishRushTime;
	// 突進開始フラグ
	bool m_StartRush;
	// 突進方向
	VECTOR m_RushDir;
	// 突進中フラグ
	bool m_IsRushing;

	// 攻撃可能フラグ
	bool m_CanAttack;
	// 突進可能フラグ
	bool m_CanRush;
	// 追いかけ可能フラグ
	bool m_CanChase;
};
