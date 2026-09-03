#include "PlayerBullet.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"

PlayerBullet::PlayerBullet()
{

}

PlayerBullet::~PlayerBullet()
{

}

void PlayerBullet::Load()
{
	// バレットのモデルをロード
	std::shared_ptr<MyResource> bulletModel = MyResourceManager::GetInstance()->LoadModelResource("Data/GravityBall/GravityBall.x", false, true);

	// モデルのハンドルを取得して保存
	if (bulletModel)
	{
		m_Handle = bulletModel->GetHandle();
	}
}

void PlayerBullet::Hit()
{
	// 生存フラグをオフにして当たり判定もオフにする
	m_IsActive = false;
	m_Collision->SetIsActive(false);
	m_Life = ACTIVE_TIME;
}

UniquePtr<Bullet> PlayerBullet::Clone()
{
	// クローンを作成
	UniquePtr<PlayerBullet> clone = MakeUnique<PlayerBullet>();
	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);
	// 生成したクローンを返す
	return std::move(clone);
}
