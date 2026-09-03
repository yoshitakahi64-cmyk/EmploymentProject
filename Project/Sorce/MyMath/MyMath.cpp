#include "MyMath.h"
#include "../Quatrenion/Quatrenion.h"
#include <math.h>

// ベクトルを作る
// startPos	作るベクトルの始点
// endPos	作るベクトルの終点
VECTOR MyMath::VecCreate(VECTOR startPos, VECTOR endPos)
{
	VECTOR result;

	result.x = endPos.x - startPos.x;
	result.y = endPos.y - startPos.y;
	result.z = endPos.z - startPos.z;

	return result;
}

VECTOR MyMath::VecSub(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.x - vecB.x;
	result.y = vecA.y - vecB.y;
	result.z = vecA.z - vecB.z;

	return result;
}

// ベクトルの足し算
// vecA ＋ vecB
VECTOR MyMath::VecAdd(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.x + vecB.x;
	result.y = vecA.y + vecB.y;
	result.z = vecA.z + vecB.z;

	return result;
}

// ベクトルのスカラー倍
// vecA * scale
VECTOR MyMath::VecScale(VECTOR vecA, float scale)
{
	VECTOR result;

	result.x = vecA.x * scale;
	result.y = vecA.y * scale;
	result.z = vecA.z * scale;

	return result;
}

// ベクトルの内積
// vecA ・ vecB
float MyMath::VecDot(VECTOR vecA, VECTOR vecB)
{
	return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

// ベクトルの外積(2D)
// vecA × vecB
float MyMath::VecCross2D(VECTOR vecA, VECTOR vecB)
{
	return vecA.x * vecB.y - vecA.y * vecB.x;
}

// ベクトルの外積(3D)
VECTOR MyMath::VecCross3D(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.y * vecB.z - vecA.z * vecB.y;
	result.y = vecA.z * vecB.x - vecA.x * vecB.z;
	result.z = vecA.x * vecB.y - vecA.y * vecB.x;

	return result;
}

// ベクトルの長さを計算する
float MyMath::VecLong(VECTOR vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

// ベクトルを正規化する
VECTOR MyMath::VecNormalize(VECTOR vec)
{
	float vecLong = VecLong(vec);

	// 長さ0の場合は0ベクトルを返す
	if (vecLong == 0.0f) return VGet(0.0f, 0.0f, 0.0f);

	return VecScale(vec, 1.0f / vecLong);
}

// ZX平面上の前方ベクトル(長さ１)を取得する
VECTOR MyMath::VecForwardZX(float rotY)
{
	VECTOR result;

	// X成分はsin(Θ)
	result.x = sinf(rotY);
	// ZX平面なのでYは無視
	result.y = 0.0f;
	// Z成分はcos(Θ)
	result.z = cosf(rotY);

	return result;
}

VECTOR MyMath::VecForward(float rotY, float rotX)
{
	VECTOR result;

	result.x = cosf(rotX) * cosf(rotY);
	result.y = cosf(rotX);
	result.z = cosf(rotX) * cosf(rotY);

	return result;
}

VECTOR MyMath::VecLerp(VECTOR start, VECTOR end, float t)
{
	return VGet(
		start.x + (end.x - start.x) * t,
		start.y + (end.y - start.y) * t,
		start.z + (end.z - start.z) * t
	);
}

VECTOR MyMath::VecSlerp(VECTOR a, VECTOR b, float t)
{
	// 1. from -> to への回転クォータニオンを作成
	Quaternion q = MyQuaternion::QfromVectorToVector(a, b);

	// 2. t だけ回転を補間
	Quaternion q_t = MyQuaternion::QSlerp(MyQuaternion::QIdentity(), q, t);

	// 3. 回転をベクトルに適用
	VECTOR result = MyQuaternion::QTransformVector(q_t, a);
	return MyMath::VecNormalize(result);
}

VECTOR MyMath::ProjectOnPlane(const VECTOR& v, const VECTOR& planeNormal)
{
	float d = MyMath::VecDot(v, planeNormal);
	return MyMath::VecCreate(MyMath::VecScale(planeNormal, d), v);
}

bool MyMath::VecEquals(const VECTOR& a, const VECTOR& b, float epsilon)
{
	return (fabsf(a.x - b.x) < epsilon &&
		    fabsf(a.y - b.y) < epsilon &&
		    fabsf(a.z - b.z) < epsilon);
}

// 単位行列を生成する
MATRIX MyMath::MatCreate()
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		result.m[i][i] = 1.0f;
	}

	return result;
}

// 行列同士の足し算
MATRIX MyMath::MatAdd(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = matA.m[i][j] + matB.m[i][j];
		}
	}

	return result;
}

// 行列同士の引き算
MATRIX MyMath::MatSubt(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = matA.m[i][j] - matB.m[i][j];
		}
	}

	return result;
}

// 行列のスカラー倍
MATRIX MyMath::MatScale(MATRIX mat, float scale)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = mat.m[i][j] * scale;
		}
	}

	return result;
}

// 行列同士の掛け算
MATRIX MyMath::MatMult(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += matA.m[i][k] * matB.m[k][j];
			}
		}
	}

	return result;
}

// 行列の転置
MATRIX MyMath::MatTransposition(MATRIX mat)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = mat.m[j][i];
		}
	}

	return result;
}

// 平行移動行列を取得
MATRIX MyMath::MatTranslation(VECTOR trans)
{
	MATRIX result = MatCreate();

	result.m[0][3] = trans.x;
	result.m[1][3] = trans.y;
	result.m[2][3] = trans.z;

	return result;
}

// 拡縮行列を取得
MATRIX MyMath::MatScale(VECTOR scale)
{
	MATRIX result = MatCreate();

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

// ピッチ行列を取得
MATRIX MyMath::MatRotationPitch(float pitch)
{
	MATRIX result = MatCreate();

	result.m[1][1] = cosf(pitch);
	result.m[1][2] = -sinf(pitch);
	result.m[2][1] = sinf(pitch);
	result.m[2][2] = cosf(pitch);

	return result;
}

// ヨー行列を取得
MATRIX MyMath::MatRotationYaw(float pitch)
{
	MATRIX result = MatCreate();

	result.m[0][0] = cosf(pitch);
	result.m[0][2] = sinf(pitch);
	result.m[2][0] = -sinf(pitch);
	result.m[2][2] = cosf(pitch);

	return result;
}

// ロール行列を取得
MATRIX MyMath::MatRotationRoll(float pitch)
{
	MATRIX result = MatCreate();

	result.m[0][0] = cosf(pitch);
	result.m[0][1] = -sinf(pitch);
	result.m[1][0] = sinf(pitch);
	result.m[1][1] = cosf(pitch);

	return result;
}

// 座標に変換行列を掛けて変換後の座標を計算する
VECTOR MyMath::MatTransform(MATRIX transMat, VECTOR pos)
{
	VECTOR result = { 0 };

	// 計算をfor文で回したいため、いったん配列に格納する
	float posBuffer[4] = {pos.x, pos.y, pos.z, 1.0f};
	// 計算結果もいったん配列に格納する
	float resultBuffer[4] = { 0 };

	// 行列変換の計算
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			resultBuffer[i] += transMat.m[i][j] * posBuffer[j];
		}
	}

	// 計算結果をベクトルに格納
	result.x = resultBuffer[0];
	result.y = resultBuffer[1];
	result.z = resultBuffer[2];

	return result;
}

void MyMath::RotatedVec(VECTOR& o_move, const MATRIX& rotMat)
{
	rotMat.m[0][0] * o_move.x + rotMat.m[0][1] * o_move.y + rotMat.m[0][2] * o_move.z,
	rotMat.m[1][0] * o_move.x + rotMat.m[1][1] * o_move.y + rotMat.m[1][2] * o_move.z,
	rotMat.m[2][0] * o_move.x + rotMat.m[2][1] * o_move.y + rotMat.m[2][2] * o_move.z;
}

// XZ平面上の距離を計算する
float MyMath::DistanceXZ(VECTOR a, VECTOR b)
{
	float result = 0.0f;

	float dx = a.x - b.x;
	float dz = a.z - b.z;

	result = dx * dx + dz * dz;

	return result;
}

float MyMath::Lerp(float start, float end, float t)
{
	return start + (end - start) * t;
}

// 値をminVal～maxValの範囲に制限する
float MyMath::Clamp(float value, float minVal, float maxVal)
{
	if (value < minVal) return minVal;
	if (value > maxVal) return maxVal;
	return value;
}

// fromからtoへの回転角を、axisを基準にして符号付きで計算する
float MyMath::SigendAngle(const VECTOR& from, const VECTOR& to, const VECTOR& axis)
{
	VECTOR f = VecNormalize(from);
	VECTOR t = VecNormalize(to);

	float dot = VecDot(f, t);
	dot = MyMath::Clamp(dot, -1.0f, 1.0f);
	float unsignedAngle = acosf(dot);

	VECTOR cross = MyMath::VecCross3D(f, t);

	float sign = MyMath::VecDot(axis, cross) < 0.0f ? -1.0f : 1.0f;

	return unsignedAngle * sign;
}

// 角度を-180～180の範囲に収める
float MyMath::NormalizeAngle(float angle)
{
	while (angle > 180.0f) angle -= 360.0f;
	while (angle < -180.0f) angle += 360.0f;
	return angle;
}

// currentからtargetへの回転をtの割合だけ補間する
float MyMath::LerpAngle(float current, float target, float t)
{
	float delta = NormalizeAngle(target - current);
	return current + delta * t;
}

