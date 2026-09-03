#include "DoorOpen.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"
#include "../Scene/SceneManager.h"

static constexpr float DOOROPEN_HEIGHT = 2.0f;

DoorOpen::DoorOpen()
{
}

void DoorOpen::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> openDoorModel = MyResourceManager::GetInstance()->LoadModelResource("Data/StageObject/Door/Door_newOpen.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (openDoorModel)
	{
		m_Handle = openDoorModel->GetHandle();
	}
}

void DoorOpen::OnStart()
{
	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 1.8f, 0.0f));
	m_CapsuleCollision->SetHeight(DOOROPEN_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.0f, 0.0f));
	m_Collision->SetSize(VGet(0.0f, 0.0f, 0.0f));

	// タイプ名を設定
	SetTypeName("Door_Open");

	// 生存フラグをオフにする
	m_IsActive = false;

	// IDを登録
	IDManager::GetInstance()->Register(this);
}

UniquePtr<StageObject> DoorOpen::Clone()
{
	// クローンを生成
	UniquePtr<DoorOpen> clone = MakeUnique<DoorOpen>();
	// 変数をコピー
	*clone = *this;

	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}
