#include "Gimmick.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionStage.h"
#include "../Collision/Capsule.h"
#include "../Player/Player.h"

static constexpr float GRAVITY = 0.01f;
static constexpr float ANIMATION_SPEED = 0.5f;

Gimmick::Gimmick()
{
	m_IsGravityApplied = false;
}

Gimmick::~Gimmick()
{

}

void Gimmick::Start()
{
	// アニメーションの再生速度を設定
	m_AnimationSpeed = ANIMATION_SPEED;
	// 生存フラグをオン
	m_IsActive = true;
	// 重力の方向を設定
	m_GravityDir = VGet(0.0f, -1.0f, 0.0f);

	// 床との当たり判定を有効にする
	m_EnableFloorCheck = true;

	// 各オブジェクトのスタート
	OnStart();

}

void Gimmick::Step()
{
	if (m_IsGravityApplied)
	{
		// 通常時の重力
		m_Move.y -= GRAVITY;
	}

	// 各オブジェクトのステップ
	OnStep();

	// 移動
	m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, m_Move);
	//前回の位置を保持
	m_PrevPos = m_Transform.position_3D;

	ControllAnimation();
}
