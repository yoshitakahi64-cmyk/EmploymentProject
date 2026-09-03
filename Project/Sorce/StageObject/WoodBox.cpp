#include "WoodBox.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"

static constexpr float WOODBOX_HEIGHT = 1.0f;

WoodBox::WoodBox()
{

}

void WoodBox::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> woodBoxModel = MyResourceManager::GetInstance()->LoadModelResource("Data/StageObject/WoodBox/WoodBox.x",false,true);

	// モデルがロードできたらハンドルを取得
	if (woodBoxModel)
	{
		m_Handle = woodBoxModel->GetHandle();
	}
}

void WoodBox::OnStart()
{
	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 1.1f, 0.0f));
	m_CapsuleCollision->SetHeight(WOODBOX_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.8f, 0.0f));
	m_Collision->SetSize(VGet(1.5f, 1.4f, 1.5f));

	// ID登録
	IDManager::GetInstance()->Register(this);
}

UniquePtr<StageObject> WoodBox::Clone()
{
	// クローンを生成
	UniquePtr<WoodBox> clone = MakeUnique<WoodBox>();

	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}
