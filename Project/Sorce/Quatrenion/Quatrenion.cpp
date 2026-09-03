#include "Quatrenion.h"
#include "../MyMath/MyMath.h"

static constexpr double M_PI = 3.14159265358979323846;

Quaternion MyQuaternion::Qnormalize(const Quaternion& q)
{
	// クォータニオンの長さを計算
	float len = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	// 長さが0に近いときは単位クォータニオンを返す
	if (len < EPS) return QIdentity();
	// 正規化
	float inv = 1.0f / len;
	// 正規化したクォータニオンを返す
	return Quaternion(q.x * inv, q.y * inv, q.z * inv, q.w * inv);
}

Quaternion MyQuaternion::QMultiply(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(
		a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
		a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
		a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
	);
}

Quaternion MyQuaternion::QfromAxisAngle(VECTOR axis, float angleDeg)
{
	float axisLen = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	if (axisLen < EPS) return QIdentity();

	// 正規化
	float inv = 1.0f / axisLen;
	float nx = axis.x * inv, ny = axis.y * inv, nz = axis.z * inv;

	float angleRad = (float)(angleDeg * (M_PI / 180.0f));

	float s = sinf(angleRad * 0.5f);

	float c = cosf(angleRad * 0.5f);

	return Qnormalize(Quaternion(nx* s,ny* s,nz* s,c));
}

Quaternion MyQuaternion::QfromAxisAngleLH(VECTOR axis, float angleRad)
{
	VECTOR invAxis = MyMath::VecScale(axis, -1.0f);

	return QfromAxisAngle(invAxis, angleRad);
}

Quaternion MyQuaternion::QfromVectorToVector(VECTOR from, VECTOR to)
{
	float flen = sqrtf(from.x * from.x + from.y * from.y + from.z * from.z);
	float tlen = sqrtf(to.x * to.x + to.y * to.y + to.z * to.z);

	if (flen < EPS || tlen < EPS) return QIdentity();
	VECTOR f = VGet(from.x / flen, from.y / flen, from.z / flen);
	VECTOR t = VGet(to.x / tlen, to.y / tlen, to.z / tlen);

	float dot = f.x * t.x + f.y * t.y + f.z * t.z;

	// 完全反対(180度)の場合、適当な直行軸で回転を作る
	if (dot < -0.999999f) {
		// fと直行する軸を選ぶ
		VECTOR ortho = VGet(1, 0, 0);
		if (fabs(f.x) > 0.9f) ortho = VGet(0, 1, 0);
		VECTOR axis = MyMath::VecCross3D(f, ortho);
		return QfromAxisAngle(axis, (float)M_PI);
	}
	VECTOR cross = VCross(f, t);
	Quaternion q(cross.x, cross.y, cross.z, 1.0f + dot);
	return Quaternion(q);
}

Quaternion MyQuaternion::QSlerp(const Quaternion& a, const Quaternion& b, float t)
{
	// dot (cos theta)
	float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	Quaternion b2 = b;

	if (dot < 0.0f)
	{
		dot = -dot;
		b2 = Quaternion(-b.x, -b.y, -b.z, -b.w);
	}

	if (dot > 0.9995f) // ほぼ同じ -> 線形補間
	{
		Quaternion result(
			a.x + t * (b2.x - a.x),
			a.y + t * (b2.y - a.y),
			a.z + t * (b2.z - a.z),
			a.w + t * (b2.w - a.w)
		);
		return Qnormalize(result);
	}

	float theta = acosf(dot);
	float sinT = sinf(theta);
	float w1 = sinf((1.0f - t) * theta) / sinT;
	float w2 = sinf(t * theta) / sinT;

	Quaternion result(
		a.x * w1 + b2.x * w2,
		a.y * w1 + b2.y * w2,
		a.z * w1 + b2.z * w2,
		a.w * w1 + b2.w * w2
	);

	return Qnormalize(result);
}

MATRIX MyQuaternion::QToMatrix(const Quaternion& q)
{
	MATRIX m;

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	m.m[0][0] = 1.0f - 2.0f * (yy + zz);
	m.m[0][1] = 2.0f * (xy - wz);
	m.m[0][2] = 2.0f * (xz + wy);
	m.m[0][3] = 0.0f;

	m.m[1][0] = 2.0f * (xy + wz);
	m.m[1][1] = 1.0f - 2.0f * (xx + zz);
	m.m[1][2] = 2.0f * (yz - wx);
	m.m[1][3] = 0.0f;

	m.m[2][0] = 2.0f * (xz - wy);
	m.m[2][1] = 2.0f * (yz + wx);
	m.m[2][2] = 1.0f - 2.0f * (xx + yy);
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	// 転置して返す
	MATRIX t;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			t.m[i][j] = m.m[j][i];

	return t;
}

VECTOR MyQuaternion::QTransformVector(const Quaternion& q, VECTOR v)
{
	Quaternion qv(v.x, v.y, v.z, 0.0f);
	Quaternion qc = Qconjugate(q);
	Quaternion r = QMultiply(QMultiply(q, qv), qc);
	return VGet(r.x, r.y, r.z);
}

Quaternion MyQuaternion::QFromMatrix(const MATRIX& m)
{
	Quaternion q;
	float tr = m.m[0][0] + m.m[1][1] + m.m[2][2];
	if (tr > 0.0f) {
		float s = sqrtf(tr + 1.0f) * 2.0f;
		q.w = 0.25f * s;
		q.x = (m.m[2][1] - m.m[1][2]) / s;
		q.y = (m.m[0][2] - m.m[2][0]) / s;
		q.z = (m.m[1][0] - m.m[0][1]) / s;
	}
	else {
		if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
			float s = sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]) * 2.0f;
			q.w = (m.m[2][1] - m.m[1][2]) / s;
			q.x = 0.25f * s;
			q.y = (m.m[0][1] + m.m[1][0]) / s;
			q.z = (m.m[0][2] + m.m[2][0]) / s;
		}
		else if (m.m[1][1] > m.m[2][2]) {
			float s = sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]) * 2.0f;
			q.w = (m.m[0][2] - m.m[2][0]) / s;
			q.x = (m.m[0][1] + m.m[1][0]) / s;
			q.y = 0.25f * s;
			q.z = (m.m[1][2] + m.m[2][1]) / s;
		}
		else {
			float s = sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]) * 2.0f;
			q.w = (m.m[1][0] - m.m[0][1]) / s;
			q.x = (m.m[0][2] + m.m[2][0]) / s;
			q.y = (m.m[1][2] + m.m[2][1]) / s;
			q.z = 0.25f * s;
		}
	}
	return Qnormalize(q);

}

Quaternion MyQuaternion::QLookRotation(VECTOR forward, VECTOR up)
{
	// z = 前方向
	VECTOR z = VNorm(forward);
	// x = up × z
	VECTOR x = VNorm(VCross(up, z));
	// y = z × x
	VECTOR y = VCross(z, x);

	MATRIX mat;
	mat.m[0][0] = x.x; mat.m[0][1] = y.x; mat.m[0][2] = z.x; mat.m[0][3] = 0;
	mat.m[1][0] = x.y; mat.m[1][1] = y.y; mat.m[1][2] = z.y; mat.m[1][3] = 0;
	mat.m[2][0] = x.z; mat.m[2][1] = y.z; mat.m[2][2] = z.z; mat.m[2][3] = 0;
	mat.m[3][0] = 0;   mat.m[3][1] = 0;   mat.m[3][2] = 0;   mat.m[3][3] = 1;


	return MyQuaternion::QFromMatrix(mat);
}

Quaternion MyQuaternion::QGetTargetRotXZ(VECTOR dir, VECTOR axis)
{
	// y成分を無視
	dir.y = 0.0f;

	// 正規化
	dir = MyMath::VecNormalize(dir);

	// エネミーの向きをプレイヤーの方向に向ける
	float angleTarget = atan2f(dir.x, dir.z);
	// ラジアン → 度
	float angleDeg = angleTarget * 180.0f / DX_PI_F;
	// プレイヤーの方向への回転を取得
	Quaternion targetRot = MyQuaternion::QfromAxisAngleLH(axis, angleDeg);

	return targetRot;
}

void MyQuaternion::QRotatation(Quaternion& a, const Quaternion& b)
{
	a = Qnormalize(QMultiply(a, b));
}
