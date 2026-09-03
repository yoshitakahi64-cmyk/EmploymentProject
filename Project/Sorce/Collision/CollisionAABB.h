#pragma once

#include "DxLib.h"
#include "CollisionBase.h"

// AABBコリジョンクラス
class CollisionAABB : public CollisionBase
{
public:
	CollisionAABB(); // コンストラクタ
	~CollisionAABB(); // デストラクタ

public:
	void Draw(); // 描画関数

public:
	// ゲッター・セッター関数
	VECTOR GetTargetPos() { return *m_TargetPos; }
	VECTOR GetLocalPos() { return m_LocalPos; }
	VECTOR GetSize() { return m_Size; }
	CollsionType GetCollisonType() const override { return CollsionType::AABB; }
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetOffsetPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetSize(VECTOR size) { m_Size = size; }
	
	
public:
	// 当たり判定振り分け
	void CheckCollision(const CollisionBase* other,CollisionResult& o_collisionResult) const override;

	// AABBとAABB
	CollisionResult CheckCollisionAABB(CollisionAABB* other) const override;
	// AABBとSphere
	CollisionResult CheckCollisionSphere(CollisionSphere* other) const override;

private:
	// 縦横奥行き幅
	VECTOR m_Size;
};
