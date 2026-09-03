#include "BulletManager.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

BulletManager::BulletManager()
{
	// クローン元のバレットのポインタをnullptrで初期化
	for (int i = 0; i < BULLET_OWNER_MAX; i++)
	{
		m_OriginalBullet[i] = nullptr;
	}
}

BulletManager::~BulletManager()
{
	Fin();
}

void BulletManager::Init()
{
	// クローン元のバレットのインスタンスを生成
	m_OriginalBullet[PLAYER] = MakeUnique<PlayerBullet>();
	m_OriginalBullet[ENEMY] = MakeUnique<EnemyBullet>();
}

void BulletManager::Load()
{
	// クローン元のバレットのロード関数を呼び出す
	for(int i = 0; i < BULLET_OWNER_MAX; i++)
	{
		m_OriginalBullet[i]->Load();
	}
}

void BulletManager::Start()
{
	// クローン元のバレットのスタート関数を呼び出す
	m_OriginalBullet[PLAYER]->Start();
	m_OriginalBullet[ENEMY]->Start();
}

void BulletManager::Step()
{
	// バレットリストの全てのバレットのステップ関数を呼び出す
	for (auto& bullet : m_BulletList)
	{
		bullet->Step();
	}
}

void BulletManager::Update()
{
	// バレットリストの全てのバレットのアップデート関数を呼び出す
	for (auto& bullet : m_BulletList)
	{
		bullet->Update();
	}

	// バレットリストの中で生存フラグがオフのバレットを削除する
	for (auto it = m_BulletList.begin(); it != m_BulletList.end(); )
	{
		if (!(*it)->GetActive())
		{
			it = m_BulletList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void BulletManager::Draw()
{
	// バレットリストの全てのバレットの描画関数を呼び出す
	for (auto& bullet : m_BulletList)
	{
		bullet->Draw();
	}
}

void BulletManager::Fin()
{
	// バレットリストをクリアする
	m_BulletList.clear();
}

Bullet* BulletManager::CreateBullet(BULLETOWNER_TYPE owner)
{
	// 生成したバレットのポインタを格納する変数
	Bullet* result = nullptr;

	// バレットリストの中で生存フラグがオフのバレットを探す
	for (auto& bullet : m_BulletList)
	{
		// 生存フラグがオフで、発射主が同じバレットが見つかったとき
		if (!bullet->GetActive() && bullet->GetOwner() == owner)
		{
			// 使われていないバレットのポインタを取得
			result = bullet.get();
			return result;
		}
	}

	// 使われていないバレットが見つからなかったときは、クローン元のバレットをクローンしてバレットリストに追加する
    m_BulletList.push_back(std::move(m_OriginalBullet[owner]->Clone()));

	// 追加したバレットのポインタを取得
	result = m_BulletList.back().get();
	return result;
}