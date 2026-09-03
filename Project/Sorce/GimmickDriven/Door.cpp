#include "Door.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"
#include "../StageObject/DoorOpen.h"
#include "../StageObject/StageObjectManager.h"
#include "../Mechanism/IDManager.h"

static constexpr float DOOR_HEIGHT = 2.0f;

Door::Door():
	m_OpenDoorHandle(0),
	m_IsOpen(false)
{
}

void Door::OnStart()
{
	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 1.8f, 0.0f));
	m_CapsuleCollision->SetHeight(DOOR_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.5f, 0.0f));
	m_Collision->SetSize(VGet(12.0f, 25.0f, 1.0f));

	// タイプ名を設定
	SetTypeName("Door");

	// IDを登録
	IDManager::GetInstance()->Register(this);

	// 押し出し処理を有効
	m_IsPushing = true;
}

void Door::Load()
{
	// 閉まっているドアのモデルをロード
    SharedPtr doorModel = MyResourceManager::GetInstance()->LoadModelResource("Data/StageObject/Door/Door_new.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (doorModel)
	{
		m_Handle = doorModel->GetHandle();
	}
}

UniquePtr<Gimmick> Door::Clone()
{
	// クローンを生成
	UniquePtr<Door> clone = MakeUnique<Door>();

	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}

void Door::Open()
{
	// すでに開いている場合は何もしない
	if (m_IsOpen) return;
	
	// IDから対象のオブジェクトを取得
	GameObject3D* obj = IDManager::GetInstance()->FindByID(m_TargetID);
	
	// 開いているドアのオブジェクトを取得
	DoorOpen* doorOpen = dynamic_cast<DoorOpen*>(obj);

	// ドアを開く
	if (doorOpen)
	{
		doorOpen->SetActive(true);
	}

	// 自分は非アクティブにする
	m_IsActive = false;
	m_Collision = nullptr;
}

void Door::Close()
{
	// すでに閉まっている場合は何もしない
	if (!m_IsOpen) return;
	m_IsOpen = false;
}

