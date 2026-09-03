#include "Bullet.h"
#include "../FPS/FPS.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Quatrenion/Quatrenion.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Mechanism/IDManager.h"

// 弾の速度
static constexpr float SHOT_SPEED = 0.03f;

Bullet::Bullet():
	// 各変数の初期化
	m_Life(0),
	m_Owner(NONE)
{
}


Bullet::~Bullet()
{
	// 終了関数の呼び出し
	Fin();
}

void Bullet::Start()
{
	// 弾の初期設定
	m_Transform.rotation = MyQuaternion::QIdentity();
	m_Transform.scale = VGet(1.0f, 1.0f, 1.0f);
	m_IsActive = true;

	// IDを登録
	IDManager::GetInstance()->Register(this);
	
	// 生存時間を初期化
	m_Life = ACTIVE_TIME;
}

void Bullet::Step()
{
	// 寿命がなくなったら
	if (m_Life <= 0.0f)
	{
		// 生存フラグをオフにして当たり判定もオフにする
		if (m_IsActive)
		{
			m_IsActive = false;
			m_Collision->SetIsActive(false);
		}

		// 寿命を初期化
		m_Life = ACTIVE_TIME;
	}

	// 寿命を減らす
	m_Life -= 1.0f;

	// 移動を確定
	m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, MyMath::VecScale(m_Move,SHOT_SPEED));
}

void Bullet::HitStageObject()
{
	// 生存フラグをオフにして当たり判定もオフにする
	m_IsActive = false;
	m_Collision->SetIsActive(false);
	// 寿命を初期化
	m_Life = ACTIVE_TIME;
}

void Bullet::BulletSetting(VECTOR front, BULLETOWNER_TYPE owner)
{
	// 弾の当たり判定を生成
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.5f, 0.0f));
	m_Collision->SetSize(VGet(1.0f, 1.0f, 1.0f));
	// 弾の回転を計算
	Quaternion objectRot = MyQuaternion::QGetTargetRotXZ(front, VGet(0.0f, 1.0f, 0.0f));
	m_Transform.rotation = objectRot;
	m_Owner = owner;
}