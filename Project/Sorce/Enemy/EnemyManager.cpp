#include "EnemyManager.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/RushEnemy.h"
#include "../Enemy/ShotEnemy.h"


EnemyManager::EnemyManager()
{
	// クローン元の敵を管理する配列を初期化
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		m_OriginalEnemy[i] = nullptr;
	}
}

EnemyManager::~EnemyManager()
{
	Fin();
}

void EnemyManager::Init()
{
	// クローン元の敵を管理する配列に各敵のインスタンスを作成して格納
	m_OriginalEnemy[RUSH_ENEMY] = MakeUnique<RushEnemy>();
	m_OriginalEnemy[SHOT_ENEMY] = MakeUnique<ShotEnemy>();
}

void EnemyManager::Load()
{
	// クローン元の敵を管理する配列の各敵のロード関数を呼び出す
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		m_OriginalEnemy[i]->Load();
	}
}

void EnemyManager::Start()
{
	// 敵のリストの各敵のスタート関数を呼び出す
	for (auto& enemy : m_Enemy)
	{
		enemy->Start();
	}
}

void EnemyManager::Step()
{
	// 敵のリストの各敵のステップ関数を呼び出す
	for (auto& enemy : m_Enemy)
	{
		enemy->Step();
	}
}

void EnemyManager::Update()
{
	// 敵のリストの各敵のアップデート関数を呼び出す
	for (auto& enemy : m_Enemy)
	{
		enemy->Update();
	}

	// 敵のリストを先頭から末尾までまわす
	for (auto it = m_Enemy.begin(); it != m_Enemy.end(); )
	{
		// 敵が非アクティブならリストから削除する
		if (!(*it)->GetActive())
		{
			it = m_Enemy.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EnemyManager::Draw()
{
	// 敵のリストの各敵のドロー関数を呼び出す
	for (auto& enemy : m_Enemy)
	{
		enemy->Draw();
	}
}

void EnemyManager::Fin()
{
	// 敵のリストをクリアする
	m_Enemy.clear();
}

void EnemyManager::CreateEnemy(EnemyType type, VECTOR pos, Quaternion rot, VECTOR scale)
{
	// タイプに合わせた敵を生成
    UniquePtr<Enemy> enemy = m_OriginalEnemy[type]->Clone();

	// 位置、回転、スケールを設定
	enemy->SetTransform(pos, rot, scale);

	// 生成した敵をリストに追加
	m_Enemy.push_back(std::move(enemy));
}
