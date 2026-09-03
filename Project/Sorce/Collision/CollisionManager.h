#pragma once
#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"


class CollisionAABB;
class CollisionSphere;
class Capsule;

#define COLLISION_MAX 64

// 当たり判定管理クラス
class CollisionManager : public Singleton<CollisionManager>
{
public:
	CollisionManager(); // コンストラクタ
	~CollisionManager(); // デストラクタ

public:
	void Update();  // アップデート関数
	void Draw();	// 描画関数
	void Fin();     // 終了関数

public:
	// AABBを生成する
	CollisionAABB* CreateAABB();
	// AABBを未使用にする
	void ReuseAABB(CollisionAABB* targetAABB);
	// Sphereを生成する
	CollisionSphere* CreateSphere();
	// Sphereを未使用にする
	void ReuseSphere(CollisionSphere* targetSphere);
	// Capsuleを生成する
	Capsule* CreateCapsule();
	// Capsuleを未使用にする
	void ReuseCapsule(Capsule* targetCapsule);
	
public:
	// 当たり判定のチェック
	void CheckCollision();
	 
	// 無敵状態の更新
	void UpdateInvincible();

private:
	// 当たり判定管理用配列
	Vector<UniquePtr<CollisionAABB>> m_AABB;
	Vector<UniquePtr<CollisionSphere>> m_Sphere;
	Vector<UniquePtr<Capsule>> m_Capsule;

	// 無敵時間
	int m_PlayerInvincibleTime;
};
