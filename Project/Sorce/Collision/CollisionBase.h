#pragma once
#include <DxLib.h>

// 当たり判定の形状列挙体
enum CollsionType
{
	AABB,
	SPHERE,
	CAPSULE,
	COLLISON_TYPEMAX,
	COLLISON_TYPENONE = -1,
};

// 衝突結果格納構造体
struct CollisionResult
{
	// ヒットフラグ
	bool isHit = false;
	// 衝突法線
	VECTOR hitNormal = VGet(0.0f, 0.0f, 0.0f);
	// めり込み量
	float panetration = 0.0f;

};

class CollisionAABB;
class CollisionSphere;

// 当たり判定の基底クラス
class CollisionBase
{
public:
	CollisionBase(); // コンストラクタ
	~CollisionBase() = default; // デストラクタ

public:
	// ゲッター・セッター関数
	virtual CollsionType GetCollisonType() const = 0;
	bool GetIsActive() { return m_IsActive; }
	void SetIsActive(bool isActive) 
	{
		int test = 0;
		m_IsActive = isActive; }
	virtual void SetTargetPos(VECTOR* pos) = 0;
	virtual void SetOffsetPos(VECTOR offset) = 0;
	virtual void SetSize(VECTOR size) {};
	virtual void SetRadius(float radius) {};

public:
	// 他のコリジョンとの当たり判定をチェック
	virtual void CheckCollision(const CollisionBase* other, CollisionResult& collisionResult) const = 0;

	// 同形状同士の当たり判定
	virtual CollisionResult CheckCollisionAABB(CollisionAABB* other) const = 0;
	virtual CollisionResult CheckCollisionSphere(CollisionSphere* other) const = 0;

	// 生存フラグ
	bool m_IsActive;
protected:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;

};