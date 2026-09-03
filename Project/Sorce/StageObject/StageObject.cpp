#include "StageObject.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionStage.h"
#include "../Collision/Capsule.h"
#include "../Collision/CollisionAABB.h"
#include "../Player/Player.h"
#include "../Scene/SceneManager.h"

static constexpr float GRAVITY = 0.01f;

StageObject::StageObject():
	m_Mode(OBJ_MODE_NORMAL),
	m_Goal(false)
{
}

StageObject::~StageObject()
{
}

void StageObject::Load()
{
}

void StageObject::Start()
{
	// ゴールフラグをオフ
	m_Goal = false;

	// 生存フラグをオン
	m_IsActive = true;

	m_GravityDir = VGet(0.0f, -1.0f, 0.0f);

	// Y軸ベクトル
	m_Up = VGet(0.0f, 1.0f, 0.0f);

	// 床との当たり判定を有効にする
	m_EnableFloorCheck = true;

	// 各オブジェクトのStart関数
	OnStart();
}

void StageObject::Step()
{
	// モードによって処理を分ける
	if (m_Mode == OBJ_MODE_NORMAL)
	{
		// 通常時の重力
		m_Move.y -= GRAVITY;
	}
	else if (m_Mode == OBJ_MODE_GRAVITY)
	{	 
		GravityModeUpdate();
	}

	// 移動
	m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, m_Move);
	//前回の位置を保持
	m_PrevPos = m_Transform.position_3D;
}

bool StageObject::HitWeherFace(const Player& player)
{
	// プレイヤーとオブジェクトの位置ベクトル
	VECTOR dir = MyMath::VecCreate(m_Transform.position_3D, player.GetPos3D());
	dir = MyMath::VecNormalize(dir);
	dir.x = 0.0f;
	dir.z = 0.0f;

	// プレイヤーの上方向
	VECTOR up = MyMath::VecScale(player.GetGravityDir(), -1.0f);

	// 方向ベクトルと上方向の内積
	float dot = MyMath::VecDot(dir, up);

	if (dot > 0.7f)
	{
		return true;
	}

	return false;
}

void StageObject::FinishStage()
{
	// クリアシーンに遷移
	SceneManager::GetInstance()->ChangeScene(CLEAR);
}

void StageObject::GravityModeUpdate()
{
	GameObject3D::GravityModeUpdate();
}
