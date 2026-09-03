#include "Button.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"
#include "../Mechanism/MechanismBase.h"
#include "../Mechanism/DoorMechanism.h"
#include "../GimmickDriven/Door.h"
#include "../Sound/SoundManager.h"

static constexpr float BUTTON_HEIGHT = 2.0f;

Button::Button():
	  m_IsPush(false),
	  m_IsPressed(false),
	m_Mechanism(nullptr)
{
}

void Button::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> buttonModel = MyResourceManager::GetInstance()->LoadModelResource("Data/Gimmick/Button/Button.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (buttonModel)
	{
		m_Handle = buttonModel->GetHandle();
	}
}

void Button::OnStart()
{
	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 2.0f, 0.0f));
	m_CapsuleCollision->SetHeight(BUTTON_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.5f, 0.0f));
	m_Collision->SetSize(VGet(1.6f, 1.6f, 1.6f));

	// 重力を適用
	m_IsGravityApplied = true;

	// タイプ名を設定
	SetTypeName("Button");

	// IDを登録
	IDManager::GetInstance()->Register(this);

	// 動かしたいギミックの動きを設定
	SettingMechanism();
	// 動かしたいギミックのIDを設定
	m_Mechanism->SetTargetID(m_TargetID);

	// 押し出し処理を無効
	m_IsPushing = false;
}

UniquePtr<Gimmick> Button::Clone()
{
	// クローンを生成
	UniquePtr<Button> clone = MakeUnique<Button>();
	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}

void Button::OnPressed()
{
	// メカニズムが設定されていたら
	if (m_Mechanism)
	{		
		// メカニズムを作動させる
		m_Mechanism->Activate();
		// 押されたフラグをオン
		m_IsPressed = true;
	}
}

void Button::OnReleased()
{
	// メカニズムが設定されていたら
	if (m_Mechanism)
	{
		// メカニズムを停止させる
		m_Mechanism->Deactivate();
		// 押されたフラグをオフ
		m_IsPressed = false;
	}
}

void Button::SettingMechanism()
{
	// IDマネージャーからIDをもとに対象のギミックを検索
	GameObject3D* obj = IDManager::GetInstance()->FindByID(m_TargetID);

	// 対象のギミックがドアだったら、ドアの動きを管理するクラスを生成して保存
	if (dynamic_cast<Door*>(obj))
	{
		m_Mechanism = new DoorMechanism();
	}
}

void Button::ControllAnimation()
{
	// 押されたフラグがオンのとき、押されているフラグがオフのときでアニメーションを切り替える
	if (m_IsPressed)
	{
		// 押されているフラグがオフのとき、押されているフラグをオンにして押されているアニメーションを再生する
		if (!m_IsPush)
		{
			PlayAnimation(BUTTON_PUSH, false);
			m_IsPush = true;
		}
	}
	else
	{
		// 押されているフラグがオンのとき、押されているフラグをオフにして元に戻すアニメーションを再生する
		if (m_IsPush)
		{
			PlayAnimation(BUTTON_BUCKDEFALUT, false);
			m_IsPush = false;
		}
	}
}

void Button::OnCollisionEnter()
{
	// サウンド再生
	SoundManager::GetInstance()->Play_SE(SE_BUTTON_PUSH, false);
	// ボタンオン関数を呼び出す
	OnPressed();
}

void Button::OnCollisionExit()
{
}

