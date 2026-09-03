#include "DxLib.h"
#include "CollisionSphere.h"
#include "../MyMath/MyMath.h"

CollisionSphere::CollisionSphere():
	m_Radius(0.0f)
{
}


CollisionSphere::~CollisionSphere()
{

}

void CollisionSphere::Draw()
{
}

void CollisionSphere::CheckCollision(const CollisionBase* other, CollisionResult& collisionResult) const
{
	if (other == nullptr)
	{
		return;
	}
	// 当たり判定の形によって判別
	switch (other->GetCollisonType())
	{
	case CollsionType::AABB:  
		collisionResult = CheckCollisionAABB((CollisionAABB*)other);
		break;
	case CollsionType::SPHERE: 
		collisionResult = CheckCollisionSphere((CollisionSphere*)other);
		break;
	default: 
		return;
	}
}

CollisionResult CollisionSphere::CheckCollisionAABB(CollisionAABB* other) const
{
	CollisionResult result;

	return result;
}

CollisionResult CollisionSphere::CheckCollisionSphere(CollisionSphere* other) const
{
	CollisionResult result;

	// 自分のコライダーの中心と相手のコライダーの中心を計算する
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	VECTOR otherCenterPos = MyMath::VecAdd(other->GetTargetPos(), other->GetLocalPos());
	float otherRadius = other->GetRadius();

	// 中心間のベクトルと距離を計算する
	VECTOR dir = MyMath::VecSub(otherCenterPos, centerPos);
	float distance = MyMath::VecLong(dir);
	float radiusSum = m_Radius + otherRadius;

	// 計算した距離が２つの球の半径の合計以下であれば当たっている
	if (distance <= radiusSum && m_IsActive)
	{
		result.isHit = true;
		result.hitNormal = MyMath::VecNormalize(dir);
		result.panetration = radiusSum - distance;
	}

	return result;
}
