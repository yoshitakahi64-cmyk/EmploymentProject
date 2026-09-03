#include "Goal.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"
#include "../Scene/SceneManager.h"

static constexpr float GOAL_HEIGHT = 1.0f;


Goal::Goal()
{

}

void Goal::Load()
{
	// 木箱のモデルをロード
	std::shared_ptr<MyResource> goalModel = MyResourceManager::GetInstance()->LoadModelResource("Data/StageObject/Goal/Goal.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (goalModel)
	{
		m_Handle = goalModel->GetHandle();
	}
}

void Goal::OnStart()
{
	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 1.1f, 0.0f));
	m_CapsuleCollision->SetHeight(GOAL_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.8f, 0.0f));
	m_Collision->SetSize(VGet(1.0f, 1.0f, 1.0f));

	IDManager::GetInstance()->Register(this);

	m_Goal = true;
}

UniquePtr<StageObject> Goal::Clone()
{
	// クローンを生成
	UniquePtr<Goal> clone = MakeUnique<Goal>();

	// 変数をコピー
	*clone = *this;

	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}
