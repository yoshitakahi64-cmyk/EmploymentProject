#include "DxLib.h"
#include "CollisionAABB.h"
#include "../MyMath/MyMath.h"
#include <cmath>


CollisionAABB::CollisionAABB():
	m_Size(VGet(0, 0, 0))
{
}

CollisionAABB::~CollisionAABB()
{

}

void CollisionAABB::Draw()
{
	// デバッグ用の当たり判定の可視化

	VECTOR posA, posB;
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);

	posA.x = centerPos.x - m_Size.x * 0.5f;
	posA.y = centerPos.y - m_Size.y * 0.5f;
	posA.z = centerPos.z - m_Size.z * 0.5f;
	posB.x = centerPos.x + m_Size.x * 0.5f;
	posB.y = centerPos.y + m_Size.y * 0.5f;
	posB.z = centerPos.z + m_Size.z * 0.5f;

	if (m_IsActive)
	{
		DrawCube3D(posA, posB, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
	}
}

void CollisionAABB::CheckCollision(const CollisionBase* other, CollisionResult& o_collisionResult) const
{
	if (other == nullptr || !other->m_IsActive)
	{
		return;
	}

	// 当たり判定の形によって判別
	switch (other->GetCollisonType())
	{
	case CollsionType::AABB: 
		o_collisionResult = CheckCollisionAABB((CollisionAABB*)other);
		break;
	case CollsionType::SPHERE:
		o_collisionResult = CheckCollisionSphere((CollisionSphere*)other);
		break;
	default:
		return;
	}
}

CollisionResult CollisionAABB::CheckCollisionAABB(CollisionAABB* other) const
{
	// 衝突結果
	CollisionResult result;

	// 自分のコライダーの中心
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	// 相手のコライダーの中心
	VECTOR otherCenterPos = MyMath::VecAdd(other->GetTargetPos(), other->GetLocalPos());
	// 相手のコライダーのサイズ
	VECTOR otherSize = other->GetSize();

	// 中心点の差
	VECTOR diff = MyMath::VecSub(otherCenterPos, centerPos);
	// 中心点の差の絶対値
	VECTOR absDiff = VGet(fabsf(diff.x), fabsf(diff.y), fabsf(diff.z));
	// サイズの半分の合計
	VECTOR halfSize = VGet(
		m_Size.x * 0.5f + otherSize.x * 0.5f,
		m_Size.y * 0.5f + otherSize.y * 0.5f,
		m_Size.z * 0.5f + otherSize.z * 0.5f
	);

	// 軸ごとのめり込み量
	VECTOR overlap = VGet(halfSize.x - absDiff.x, halfSize.y - absDiff.y, halfSize.z - absDiff.z);

	// 衝突している場合
	if (overlap.x >= 0.0f && overlap.y >= 0.0f && overlap.z >= 0.0f && m_IsActive)
	{
		result.isHit = true;

		// 最もめり込みの小さい軸を押し戻し方向にする
		if (overlap.x < overlap.y && overlap.x < overlap.z)
		{
			// 衝突法線の確定
			result.hitNormal = VGet((diff.x > 0 ? 1.0f : -1.0f), 0.0f, 0.0f);
			// めり込み量を確定
			result.panetration = overlap.x;
		}
		else if (overlap.y < overlap.x && overlap.y < overlap.z)
		{
			// 衝突法線の確定
			result.hitNormal = VGet(0.0f, (diff.y > 0 ? 1.0f : -1.0f), 0.0f);
			// めり込み量を確定
			result.panetration = overlap.y;

		}
		else if (overlap.z < overlap.x && overlap.z < overlap.y)
		{
			// 衝突法線の確定
			result.hitNormal = VGet(0.0f, 0.0f, (diff.z > 0 ? 1.0f : -1.0f));
			// めり込み量を確定
			result.panetration = overlap.z;
		}
	}
	else
	{
		result.isHit = false;
	}

	return result;
	
}

CollisionResult CollisionAABB::CheckCollisionSphere(CollisionSphere* other) const
{
	CollisionResult result;
	return result;
}
