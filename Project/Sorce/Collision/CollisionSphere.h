#pragma once

#include "DxLib.h"
#include "CollisionBase.h"

// 球コリジョンクラス
class CollisionSphere : public CollisionBase
{
public:
	CollisionSphere(); // コンストラクタ
	~CollisionSphere(); // デストラクタ

public:
	void Draw(); // 描画関数

public:
	// ゲッター・セッター関数
	VECTOR GetTargetPos() { return *m_TargetPos; }
	VECTOR GetLocalPos() { return m_LocalPos; }
	float GetRadius() { return m_Radius; }
	CollsionType GetCollisonType() const override { return CollsionType::SPHERE; }
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetOffsetPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetRadius(float radius) { m_Radius = radius; }

public:
	// 当たり判定振り分け
	void CheckCollision(const CollisionBase* other, CollisionResult& collisionResult) const override;

	// AABBとAABB
	CollisionResult CheckCollisionAABB(CollisionAABB* other) const override;
	// AABBとSphere
	CollisionResult CheckCollisionSphere(CollisionSphere* other) const override;

private:
	// 球の半径
	float m_Radius;
};
