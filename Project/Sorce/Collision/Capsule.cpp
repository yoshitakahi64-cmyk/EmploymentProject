#include "DxLib.h"
#include "Capsule.h"
#include "../MyMath/MyMath.h"



Capsule::Capsule():
	m_TargetPos(nullptr),
	m_OffsetPos(VGet(0, 0, 0)),
	m_Height(0),
	m_Radius(0),
	m_Center(VGet(0, 0, 0)),
	m_Gravity(VGet(0, -1, 0)),
	m_Up(VGet(0, 1, 0)),
	m_IsActive(true),
	m_UsePlayer(false)
{
}

Capsule::~Capsule()
{
}

void Capsule::Draw()
{
}

VECTOR Capsule::GetWorldPos()
{
	// ワールド座標を計算
	return MyMath::VecAdd(*m_TargetPos, m_OffsetPos);
}

Quaternion Capsule::MakeRotationFromGravity(const VECTOR& gravityDir)
{
	// Upベクトル
	VECTOR up = MyMath::VecNormalize(MyMath::VecScale(gravityDir, -1.0f));

	// ワールド前方を基準
	VECTOR forward = VGet(0, 0, 1);

	// 前方ベクトル補正
	if (fabsf(MyMath::VecDot(up, forward)) > 0.99f)
	{
		forward = VGet(1, 0, 0);
	}

	// 右ベクトル
	VECTOR right = MyMath::VecNormalize(MyMath::VecCross3D(up, forward));

	// 前方ベクトル
	forward = MyMath::VecNormalize(MyMath::VecCross3D(right, up));

	// 回転行列
	MATRIX mat;
	mat.m[0][0] = right.x;   mat.m[0][1] = right.y;   mat.m[0][2] = right.z;   mat.m[0][3] = 0;
	mat.m[1][0] = up.x;      mat.m[1][1] = up.y;      mat.m[1][2] = up.z;      mat.m[1][3] = 0;
	mat.m[2][0] = forward.x; mat.m[2][1] = forward.y; mat.m[2][2] = forward.z; mat.m[2][3] = 0;
	mat.m[3][0] = 0;         mat.m[3][1] = 0;         mat.m[3][2] = 0;         mat.m[3][3] = 1;

	return MyQuaternion::QFromMatrix(mat);
}

VECTOR Capsule::GetBottomPos()
{
	// ターゲットのBottomの座標を計算
	VECTOR targetBottomPos = *m_TargetPos;
	targetBottomPos.y += m_Radius / 2;

	return targetBottomPos;
}

VECTOR Capsule::GetTopPos()
{
	// ターゲットのTopの座標を計算
	VECTOR targetTopPos = *m_TargetPos;
	targetTopPos.y += m_Height;

	return targetTopPos;
}

VECTOR Capsule::GetRotatedTopPos()
{
	// 回転後のTopの座標を計算
	VECTOR up = MyQuaternion::QTransformVector(m_Rotation, VGet(0, 1, 0));
	return MyMath::VecAdd(GetRotatedWorldPos(), MyMath::VecScale(up, m_Height * 0.5f));
}

VECTOR Capsule::GetRotatedBottomPos()
{
	// 回転後のBottomの座標を計算
	VECTOR down = MyQuaternion::QTransformVector(m_Rotation, VGet(0, -1, 0));
	return MyMath::VecAdd(GetRotatedWorldPos(), MyMath::VecScale(down, m_Height * 0.5f));
}

VECTOR Capsule::GetLowestPos()
{
	// 回転後のBottomの座標から半径分下げた位置を計算
	return MyMath::VecSub(GetRotatedBottomPos(),MyMath::VecScale(m_Up,m_Radius));
}

VECTOR Capsule::GetBottomAlongGravity()
{
	// ターゲットの座標から重力方向に半分の高さ分下げた位置を計算
	VECTOR center = GetWorldPos();
	VECTOR halfHeight = MyMath::VecScale(m_Gravity, m_Height * 0.5f);
	return VSub(center,halfHeight);
}

VECTOR Capsule::GetRotatedWorldPos()
{
	// 回転後のワールド座標を計算
	VECTOR rotateOffSet = MyQuaternion::QTransformVector(m_Rotation, m_OffsetPos);
	return MyMath::VecAdd(*m_TargetPos,rotateOffSet);
}

VECTOR Capsule::GetUpDirection()
{
	// Upベクトルを計算
	return MyQuaternion::QTransformVector(m_Rotation,VGet(0,1,0));
}

void Capsule::RotateCapsule(const VECTOR& axsis, float rad)
{
	// カプセルの回転を更新
	Quaternion rot = MyQuaternion::QfromAxisAngle(axsis, rad);
	m_Rotation = MyQuaternion::QMultiply(rot, m_Rotation);
}

