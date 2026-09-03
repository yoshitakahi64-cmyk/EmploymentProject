#include "Ball.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"
#include "../MyMath/MyMath.h"

static constexpr float BALL_HEIGHT = 1.0f;

Ball::Ball()
{

}

void Ball::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> ballModel = MyResourceManager::GetInstance()->LoadModelResource("Data/StageObject/Ball/Ball.x", false, true);
	// モデルがロードできたらハンドルを取得
	if (ballModel)
	{
		m_Handle = ballModel->GetHandle();
	}
}

void Ball::OnStart()
{
	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 0.5f, 0.0f));
	m_CapsuleCollision->SetHeight(BALL_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 1.0f, 0.0f));
	m_Collision->SetSize(VGet(1.9f, 1.9f, 1.9f));

	IDManager::GetInstance()->Register(this);
}

UniquePtr<PullObjectBase> Ball::Clone()
{
	// クローンを生成
	UniquePtr<Ball> clone = MakeUnique<Ball>();

	// 変数をコピー
	*clone = *this;

	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}

void Ball::OnPulling(float pullAmount, const VECTOR& pulleyPos)
{
	// 引っ張られる方向を取得
	VECTOR dir = MyMath::VecCreate(m_Transform.position_3D, pulleyPos);
	// 正規化
	dir = MyMath::VecNormalize(dir);

	// 引っ張られる力を加算
	m_Move = MyMath::VecAdd(m_Move, MyMath::VecScale(dir, pullAmount));
}

void Ball::OnStopPulling()
{
	// 移動量をリセット
	m_Move = {};
}

