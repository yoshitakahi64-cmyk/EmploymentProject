#pragma once

#include "Bullet.h"

// プレイヤーの弾クラス
class PlayerBullet : public Bullet
{
public:
	PlayerBullet();   // コンストラクタ
	~PlayerBullet();  // デストラクタ

public:
	void Load()override; // ロード関数

	// クローン関数
	UniquePtr<Bullet> Clone()override;;

	// 当たり判定関数
	void Hit()override;  	
};
