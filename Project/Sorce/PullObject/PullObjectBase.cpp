#include "PullObjectBase.h"
#include "../MyMath/MyMath.h"

static constexpr float GRAVITY = 0.01f;


void PullObjectBase::Start()
{
	// 生存フラグをオン
	m_IsActive = true;

	m_GravityDir = VGet(0.0f, -1.0f, 0.0f);

	// Y軸ベクトル
	m_Up = VGet(0.0f, 1.0f, 0.0f);

	// 各オブジェクトのStart関数
	OnStart();
}

void PullObjectBase::Step()
{
	// 通常時の重力
	m_Move.y -= GRAVITY;
	// 移動
	m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, m_Move);
	//前回の位置を保持
	m_PrevPos = m_Transform.position_3D;

	m_Move = {};
}
