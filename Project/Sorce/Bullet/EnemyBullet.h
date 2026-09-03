#pragma once

#include "Bullet.h"

// 敵の弾クラス
class EnemyBullet : public Bullet
{
public:
	EnemyBullet();  // コンストラクタ
	~EnemyBullet(); // デストラクタ

public:
	void Load()override; // ロード関数

	// クローン関数
	UniquePtr<Bullet> Clone()override;

	// 当たり判定関数
	void Hit()override;  
};
