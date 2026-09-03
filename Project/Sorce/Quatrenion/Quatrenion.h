#pragma once

#include <DxLib.h>
#include <cmath>

struct Quaternion
{
	// ベクトルとスカラー
	float x, y, z, w;

	Quaternion() : x(0), y(0), z(0), w(1) {};
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
};

class MyQuaternion
{
public:
// 基本関数
	// 単位クォータニオンを返す関数
	static Quaternion QIdentity() { return Quaternion(0, 0, 0, 1); }
	// クォータニオンの正規化関数
	static Quaternion Qnormalize(const Quaternion& q);
	// クォータニオンの共役を返す関数
	static Quaternion Qconjugate(const Quaternion& q) { return Quaternion(-q.x, -q.y, -q.z, q.w); }
	// クォータニオンの乗算関数
	static Quaternion QMultiply(const Quaternion& a, const Quaternion& b);

	// 生成関数
	// 軸回転からクォータニオンを導く関数
	static Quaternion  QfromAxisAngle(VECTOR axis, float angleDeg);
	// 左手座標系用
	static Quaternion  QfromAxisAngleLH(VECTOR axis, float angleDeg);
	// ベクトルからクォータニオンを導く関数
	static Quaternion  QfromVectorToVector(VECTOR from, VECTOR to);

	// 補間関数
	// クォータニオンの球面線形補間関数
	static Quaternion QSlerp(const Quaternion& a, const Quaternion& b, float t);

	// 変換関数
	// クォータニオンから行列への変換関数
	static MATRIX QToMatrix(const Quaternion& q);
	// クォータニオンでベクトルを回転させる関数
	static VECTOR QTransformVector(const Quaternion& q, VECTOR v);
	// 行列からクォータニオンに変換する関数
	static Quaternion QFromMatrix(const MATRIX& m);

	// 回転関数
	// LookRotation関数
	static Quaternion QLookRotation(VECTOR forward, VECTOR up);
	// 軸と方向ベクトルから回転を求める関数
	static Quaternion QGetTargetRotXZ(VECTOR dir, VECTOR axis);
	static void QRotatation(Quaternion& a,const Quaternion & b);
};
