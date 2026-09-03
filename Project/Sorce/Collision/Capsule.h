#pragma once

#include "DxLib.h"
#include "../Quatrenion/Quatrenion.h"

// カプセルコリジョンクラス
class Capsule
{
public:
	Capsule(); // コンストラクタ
	~Capsule(); // デストラクタ

public:
	void Draw(); // 描画関数

public:
	// ゲッター・セッター関数
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetOffsetPos(VECTOR offSetPos) { m_OffsetPos = offSetPos; }
	void SetHeight(float height) { m_Height = height; }
	void SetRadius(float radius) { m_Radius = radius; }
	void SetRotation(const Quaternion& q) { m_Rotation = q; }
	Quaternion MakeRotationFromGravity(const VECTOR& gravityDir);
	VECTOR GetBottomPos();
	VECTOR GetWorldPos();
	VECTOR GetTopPos();
	VECTOR GetRotatedTopPos();
	VECTOR GetRotatedBottomPos();
	VECTOR GetLowestPos();
	VECTOR GetBottomAlongGravity();
	VECTOR GetRotatedWorldPos();
	float GetHeight(){ return m_Height; }
	VECTOR GetUpDirection();
	float GetRadius() { return m_Radius; }
	void RotateCapsule(const VECTOR& axsis,float rad);
	bool GetIsActive() { return m_IsActive; }
	void SetIsActive(bool isActive) { m_IsActive = isActive; }
	void SetUsePlayer(bool usePlayer) { m_UsePlayer = usePlayer; }
	void SetGravity(VECTOR gravity) { m_Gravity = gravity; }

public:
	// 生存フラグ
	bool m_IsActive;

private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_OffsetPos;
	// 球の半径
	float m_Radius;
	// 球の高さ
	float m_Height;
	// 回転
	Quaternion m_Rotation;
	// 回転の中心
	VECTOR m_Center;
	// Upベクトル
	VECTOR m_Up;
	// 重力
	VECTOR m_Gravity;

	// プレイヤー
	bool m_UsePlayer;
};
