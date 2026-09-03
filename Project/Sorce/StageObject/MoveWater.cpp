#include "MoveWater.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"

static constexpr float MOVEWATER_HEIGHT = 2.0f;

MoveWater::MoveWater()
{
}

void MoveWater::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> moveWaterModel = MyResourceManager::GetInstance()->LoadModelResource("Data/StageObject/MoveWater/MoveWater.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (moveWaterModel)
	{
		m_Handle = moveWaterModel->GetHandle();
	}
}

void MoveWater::OnStart()
{
	// 動かせる水の位置を決定
	m_Transform.position_3D = VGet(-20.0f, 10.0f, 20.0f);

	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 0.5f, 0.0f));
	m_CapsuleCollision->SetHeight(MOVEWATER_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.0f, 0.0f));
	m_Collision->SetSize(VGet(11.5f, 1.8f, 16.0f));

	// ID登録
	IDManager::GetInstance()->Register(this);
}

UniquePtr<StageObject> MoveWater::Clone()
{
	// クローンを生成
	UniquePtr<MoveWater> clone = MakeUnique<MoveWater>();

	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}
