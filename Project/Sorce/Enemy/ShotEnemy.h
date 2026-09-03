#pragma once
#include <DxLib.h>
#include "../Enemy/Enemy.h"
#include "../Shot/Shot.h"
#include "../Container.h"

// 射撃攻撃を行う敵クラス
class ShotEnemy : public Enemy
{
public:
	ShotEnemy(); // コンストラクタ
	~ShotEnemy() = default; // デストラクタ

public:
	void OnStart()override; // スタート関数
	void Load()override; // ロード関数

	// クローン関数
	UniquePtr<Enemy> Clone()override;
	// アニメーション更新
	void ControllAnimation()override;

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "ShotEnemy"; }
private:
	// 状態更新
	void UpdateState(float distance)override;
	// 射撃関数
	void EnemyShot()override;
	// 逃走関数
	void EnemyEscape()override;

private:
	// Shotクラス
	Shot m_Shot;

	// 射撃の発射間隔
	float m_ReShotTime;
	// 逃走継続時間
	float m_EscapeTime;
	// 逃走開始位置
	VECTOR m_EscapeStartPos;
	// 逃走方向
	VECTOR m_EscapeDir;
	// 逃走方向決定フラグ
	bool m_IsdecideEscapeDir;

	bool m_IsShooting;
	int m_ShotAnimTimer;
};