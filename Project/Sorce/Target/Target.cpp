#include "Target.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"
#include "../MyMath/MyMath.h"
#include "../Camera/CameraManager.h"
#include "../Camera/Camera.h"

Target::Target():
	m_CurrentTargetEnemy(nullptr),
	m_CurrentIndex(-1),
	m_IsCompulsionTargeting(false),
	m_IsTargeting(false)
{
}

void Target::Update()
{
	// ターゲット可能な敵を検索
	SearchEnableTarget();
}

void Target::SearchEnableTarget()
{
	// ターゲット可能な敵のリストをクリア
	m_EnableTargetList.clear();
	// 敵のリストを取得
	const List<UniquePtr<Enemy>>& enemyList = EnemyManager::GetInstance()->GetEnemyList();

	for (auto& enemy : enemyList)
	{
		// ターゲット可能な敵をリストに追加
		if (enemy->GetEnableTarget() && !enemy->GetGravityCommand().IsHolding)
		{
			m_EnableTargetList.push_back(enemy.get());
		}
	}
	// ターゲット可能な敵を距離順にソート
	SortNearlyTarget();
}

void Target::SortNearlyTarget()
{
	// プレイヤーを参照
	Player* player = PlayerManager::GetInstance()->GetPlayer().get();

	// ターゲット可能な敵を距離順にソート
	m_EnableTargetList.sort([&](Enemy* a, Enemy* b)
		{
			float distA = MyMath::DistanceXZ(player->GetPos3D(), a->GetPos3D());
			float distB = MyMath::DistanceXZ(player->GetPos3D(), b->GetPos3D());
			return distA < distB;
		});
}

void Target::StartTargetEnemy()
{
	// ターゲット強制解除フラグをリセット
	m_IsCompulsionTargeting = false;
	// ターゲット中フラグをオン
	m_IsTargeting = true;

	// ターゲット可能な敵のリストが空であれば
	if (m_EnableTargetList.empty())
	{
		m_CurrentTargetEnemy = nullptr;
		return;
	}

	// ターゲット可能な敵を取得
	int size = static_cast<int>(m_EnableTargetList.size());

	m_CurrentIndex = 0;

	// 最初のターゲット敵を設定
	auto targetEnemy = m_EnableTargetList.begin();
	m_CurrentTargetEnemy = *targetEnemy;

	Camera* camera = CameraManager::GetInstance()->GetCamera().get();

	// 敵とプレイヤーとの間に障害物があればターゲットはしない
	if (camera->HitCkeckCameraToEnemyRay())
	{
		// ターゲット中フラグをオフ
		m_IsTargeting = false;
		// ターゲット可能な敵のリストをクリア
		m_EnableTargetList.clear();
		// 現在のターゲット敵をリセット
		m_CurrentTargetEnemy = nullptr;
		// インデックスをリセット
		m_CurrentIndex = -1;
		return;
	}

	// ターゲット中フラグをオン
	m_CurrentTargetEnemy->SetIsTargeting(true);
	// カメラをターゲットカメラに変更
	CameraManager::GetInstance()->SetTarget();
	// プレイヤーのショットモードをターゲット用に
	PlayerManager::GetInstance()->GetPlayer().get()->SetShotMode(SHOT_TARGET);
}

void Target::ChangeTargetEnemy(int delta)
{
	// 現在のターゲット敵のターゲットフラグをオフ
	if (m_CurrentTargetEnemy)
	{
		m_CurrentTargetEnemy->SetIsTargeting(false);
	}

	// ターゲット可能な敵のリストが空であれば
	if(m_EnableTargetList.empty())
	{
		m_CurrentTargetEnemy = nullptr;
		return;
	}

	// ターゲット可能な敵の数を取得
	int size = static_cast<int>(m_EnableTargetList.size());

	// インデックスを変更
	m_CurrentIndex += delta;

	// インデックスが範囲外ならループさせる
	if (m_CurrentIndex < 0) m_CurrentIndex = size - 1;
	if (m_CurrentIndex >= size) m_CurrentIndex = 0;
	
	// 現在のターゲット敵を更新
	auto it = m_EnableTargetList.begin();

	std::advance(it, m_CurrentIndex);
	m_CurrentTargetEnemy = *it;
	m_CurrentTargetEnemy->SetIsTargeting(true);
}

void Target::ClearTarget(bool isCompulsion)
{
	// 現在ターゲット中の敵がnullなら終了
	if (!m_CurrentTargetEnemy)return;

	// ターゲット中フラグをオフ
	m_IsTargeting = false;
	// ターゲット可能な敵のリストをクリア
	m_EnableTargetList.clear();
	// ターゲットフラグをオフ
	m_CurrentTargetEnemy->SetIsTargeting(false);
	// 現在のターゲット敵をリセット
	m_CurrentTargetEnemy = nullptr;
	// インデックスをリセット
	m_CurrentIndex = -1;
	// ターゲットカメラをリセット
	CameraManager::GetInstance()->SetNormal();
	// プレイヤーのショットモードをノーマルに
	PlayerManager::GetInstance()->GetPlayer().get()->SetShotMode(SHOT_NORMAL);
	// 強制解除フラグを取得
	m_IsCompulsionTargeting = isCompulsion;
}
