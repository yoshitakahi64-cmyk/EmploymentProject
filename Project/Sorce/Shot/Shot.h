#pragma once
#include <DxLib.h>
#include "../Bullet/Bullet.h"

class Shot
{
public: 
	Shot(); // コンストラクタ
	~Shot() = default; // デストラクタ
public:
	// 発射関数
	void Fire(VECTOR pos,VECTOR front,BULLETOWNER_TYPE owner,float speed);

private:
	// 弾の位置
	VECTOR m_BulletPos;
};	
