#include "EnemyBullet.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Load()
{
	// バレットのモデルをロード
	std::shared_ptr<MyResource> bulletModel = MyResourceManager::GetInstance()->LoadModelResource("Data/Nife/Nife.x", false, true);

	// モデルのハンドルを取得して保存
	if (bulletModel)
	{
		m_Handle = bulletModel->GetHandle();
	}
}

void EnemyBullet::Hit()
{
	// 生存フラグをオフにして当たり判定もオフにする
	m_IsActive = false;
	m_Collision->SetIsActive(false);
	m_Life = ACTIVE_TIME;
}

UniquePtr<Bullet> EnemyBullet::Clone()
{
	// クローンを作成
	UniquePtr<EnemyBullet> clone = MakeUnique<EnemyBullet>();
	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);
	// 生成したクローンを返す
	return std::move(clone);
}
