#pragma once

#include "DxLib.h"

constexpr float EPS = 1e-5f;

class MyMath
{
public:
// ベクトル関連の計算関数
	static VECTOR VecCreate(VECTOR startPos, VECTOR endPos);
	static VECTOR VecSub(VECTOR vecA, VECTOR vecB);
	static VECTOR VecAdd(VECTOR vecA, VECTOR vecB);
	static VECTOR VecScale(VECTOR vecA, float scale);
	static float VecDot(VECTOR vecA, VECTOR vecB);
	static float VecCross2D(VECTOR vecA, VECTOR vecB);
	static VECTOR VecCross3D(VECTOR vecA, VECTOR vecB);
	static float VecLong(VECTOR vec);
	static VECTOR VecNormalize(VECTOR vec);
	static VECTOR VecForwardZX(float rotY);
	static VECTOR VecForward(float rotY, float rotX);
	static VECTOR VecLerp(VECTOR a, VECTOR b, float t);
	static VECTOR VecSlerp(VECTOR a, VECTOR b, float t);
	static VECTOR ProjectOnPlane(const VECTOR& v, const VECTOR& planeNormal);
	static bool VecEquals(const VECTOR& a, const VECTOR& b, float epsilon = 0.0001f);

// 行列関連の計算関数
	static MATRIX MatCreate();
	static MATRIX MatAdd(MATRIX matA, MATRIX matB);
	static MATRIX MatSubt(MATRIX matA, MATRIX matB);
	static MATRIX MatScale(MATRIX mat, float scale);
	static MATRIX MatMult(MATRIX matA, MATRIX matB);
	static MATRIX MatTransposition(MATRIX mat);
	static MATRIX MatTranslation(VECTOR trans);
	static MATRIX MatScale(VECTOR scale);
	static MATRIX MatRotationPitch(float pitch);
	static MATRIX MatRotationYaw(float pitch);
	static MATRIX MatRotationRoll(float pitch);
	static VECTOR MatTransform(MATRIX transMat, VECTOR pos);
	static void RotatedVec(VECTOR& o_move,const MATRIX& rotMat);
// 距離計算関数
	static float DistanceXZ(VECTOR a, VECTOR b);
	static float Lerp(float start, float end, float t);

// 値制限関数
	static float Clamp(float value, float minVal, float maxVal);

// 回転関連関数
	static float SigendAngle(const VECTOR& from, const VECTOR& to, const VECTOR& axis);
	static float NormalizeAngle(float angle);
	static float LerpAngle(float current, float target, float t);
};
