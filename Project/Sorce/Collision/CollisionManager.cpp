#include <list>
#include "CollisionManager.h"
#include "CollisionAABB.h"
#include "CollisionSphere.h"
#include "Capsule.h"
#include "CollisionStage.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Enemy.h"
#include "../Bullet/BulletManager.h"
#include "../Bullet/Bullet.h"
#include "../StageObject/StageObjectManager.h"
#include "../StageObject/StageObject.h"
#include "../Gimmick/GimmickManager.h"
#include "../Gimmick/Gimmick.h"
#include "../PullObject/PullObjectManager.h"
#include "../PullObject/PullObjectBase.h"
#include "../Camera/CameraManager.h"
#include "../Sound/SoundManager.h"
#include "../MyEffekseer/EffekseerManager.h"

static constexpr int INVINCIBLE_DURATION = 120;
static constexpr float EFFECT_OFFSET_PLAYERSHOT = 1.0f;
static constexpr float EFFECT_OFFSET_ENEMYSHOT = 1.0f;
static constexpr float EFFECT_OFFSET_CONTROLLSHOT = 1.0f;
static constexpr float EFFECT_OFFSET_CONTROLLENEMYBUMP = 3.0f;

// コンストラクタ
CollisionManager::CollisionManager():
	m_PlayerInvincibleTime(0)
{
	// ステージの当たり判定を生成
	CollisionStage::CreateInstance();

}

CollisionManager::~CollisionManager()
{
	Fin();
}

void CollisionManager::Update()
{
	// 無敵状態の更新
	UpdateInvincible();
}

void CollisionManager::Draw()
{
	// m_AABBを先頭から末尾までまわす
	for (auto& aabb : m_AABB)
	{
		if (aabb)
		{
			aabb->Draw();
		}
	}

	// m_Sphereを先頭から末尾までまわす
	for (auto& sphere : m_Sphere)
	{
		if (sphere)
		{
			sphere->Draw();
		}
	}

	// m_Capsuleを先頭から末尾までまわす
	for (auto& capsule : m_Capsule)
	{
		if (capsule)
		{
			capsule->Draw();
		}
	}
}

void CollisionManager::Fin()
{
	// カプセル、球、AABBのリストをクリアする
	m_Capsule.clear();
	m_Sphere.clear();
	m_AABB.clear();
}

// AABBを生成する
CollisionAABB* CollisionManager::CreateAABB()
{
	CollisionAABB* result = nullptr;

	for (auto& aabb : m_AABB)
	{
		// 未使用のAABBか
		if (!aabb->GetIsActive())
		{
			// 使用中にする
			aabb->SetIsActive(true);
			// ポインタを取得して返す
			result = aabb.get();

			return result;
		}
	}

	// 空きがなかった場合は新しく生成して返す
	m_AABB.push_back(MakeUnique<CollisionAABB>());

	// 追加した要素のポインタを取得
	result = m_AABB.back().get();

	return result;
}

// AABBを未使用にする
void CollisionManager::ReuseAABB(CollisionAABB* targetAABB)
{
	// m_AABBを先頭から末尾までまわす
	for (auto& aabb : m_AABB)
	{
		// ポインタが一致したら未使用にする
		if (aabb.get() == targetAABB)
		{
			aabb->SetIsActive(false);

			return;
		}
	}
}

// Sphereを生成する
CollisionSphere* CollisionManager::CreateSphere()
{
	// 結果のポインタ
	CollisionSphere* result = nullptr;

	// 未使用のSphereを探す
	for (auto& sphere : m_Sphere)
	{
		// 未使用のAABBか
		if (!sphere->GetIsActive())
		{
			// 使用中にする
			sphere->SetIsActive(true);
			// ポインタを取得して返す
			result = sphere.get();

			return result;
		}
	}

	// 空きがなかった場合は新しく生成して返す
	m_Sphere.push_back(MakeUnique<CollisionSphere>());

	// 追加した要素のポインタを取得
	result = m_Sphere.back().get();

	return result;
}

// Sphereを未使用にする
void CollisionManager::ReuseSphere(CollisionSphere* targetSphere)
{
	// m_Sphereを先頭から末尾までまわす
	for (auto& sphere : m_Sphere)
	{
		// ポインタが一致したら未使用にする
		if (sphere.get() == targetSphere)
		{
			sphere->SetIsActive(false);
			return;
		}
	}
}

// Capsuleを生成する
Capsule* CollisionManager::CreateCapsule()
{
	Capsule* result = nullptr;

	for (auto& capsule : m_Capsule)
	{
		// 未使用のAABBか
		if (!capsule->GetIsActive())
		{
			// 使用中にする
			capsule->SetIsActive(true);
			// ポインタを取得して返す
			result = capsule.get();

			return result;
		}
	}

	// 空きがなかった場合は新しく生成して返す
    m_Capsule.push_back(MakeUnique<Capsule>());

	// 追加した要素のポインタを取得
	result = m_Capsule.back().get();

	return result;
}

// Capsuleを未使用にする
void CollisionManager::ReuseCapsule(Capsule* targetCapsule)
{
	// m_Capsuleを先頭から末尾までまわす
	for (auto& capsule : m_Capsule)
	{
		// ポインタが一致したら未使用にする
		if (capsule.get() == targetCapsule)
		{
			capsule->SetIsActive(false);
			return;
		}
	}
}

// 当たり判定のチェック
void CollisionManager::CheckCollision()
{
	// プレイヤーを参照
	Player* player = PlayerManager::GetInstance()->GetPlayer().get();
	// 敵を参照
	const List<UniquePtr<Enemy>> &enemyList = EnemyManager::GetInstance()->GetEnemyList();
	// 弾を参照
	const List<UniquePtr<Bullet>>& bulletList = BulletManager::GetInstance()->GetBulletList();
	// ステージオブジェクトを参照
	const List<UniquePtr<StageObject>>& stageObjectList = StageObjectManager::GetInstance()->GetStageObjectList();
	//　ギミックを参照
	const List<UniquePtr<Gimmick>>& gimmickList = GimmickManager::GetInstance()->GetGimmickList();
	// プルオブジェクトを参照
	const List<UniquePtr<PullObjectBase>>& pullObjectList = PullObjectManager::GetInstance()->GetPullObjectList();


	// 敵との押し出し処理
	for (auto& enemy : enemyList)
	{
		CollisionResult result;

		player->GetCollision()->CheckCollision(enemy->GetCollision(), result);

		player->HitBodyAnyObject(&result);
	}

	// ステージオブジェクトとの押し出し処理
	for (auto& stageObject : stageObjectList)
	{
		CollisionResult result;

		// プレイヤーとステージオブジェクトの当たり判定と押し出し処理
		player->GetCollision()->CheckCollision(stageObject->GetCollision(), result);	

		if (result.isHit)
		{
			int test = 0;
		}

		player->HitBodyAnyObject(&result);

		if (result.hitNormal.y > 0 || result.hitNormal.y < 0)
		{
			player->SetIsFall(false);
			player->SetIsJamp(false);
		}

		// ゴールに当たっていたらステージクリア
		if (result.isHit && stageObject->m_Goal)
		{
			SoundManager::GetInstance()->Stop_BGM(BGM_GAMEPLAY);
			stageObject->FinishStage();
		}	
	}

	// 各オブジェクトと弾との当たり判定
	for (auto& bullet : bulletList)
	{
		CollisionResult playerResult;
		CollisionResult enemyResult;
		CollisionResult stageObjectResult;

		// プレイヤーと弾の当たり判定
		bullet->GetCollision()->CheckCollision(player->GetCollision(), playerResult);
		
		// プレイヤーが弾に当たっていて、弾の発射主が敵だったらダメージを与える
		if (playerResult.isHit && bullet->GetOwner() == ENEMY && m_PlayerInvincibleTime <= 0)
		{
			bullet->Hit();
			player->HitDamage(1);
			player->SetMove({});
			player->SetGetDamage(true);
			player->SetIsHitAttack(true);
			player->SetIsHitAttackFrame(INVINCIBLE_DURATION);
			VECTOR effectPos = VGet(player->GetPos3D().x, player->GetPos3D().y + EFFECT_OFFSET_ENEMYSHOT, player->GetPos3D().z);
			EffekseerManager::GetInstance()->PlayEffect(3,effectPos);
			SoundManager::GetInstance()->Play_SE(SE_ENEMY_SHOT_HIT,false);
			// 無敵状態にする
			m_PlayerInvincibleTime = INVINCIBLE_DURATION;
		}	

		
		for (auto& enemy : enemyList)
		{
			// 敵が弾に当たっていて、弾の発射主がプレイヤーだったらダメージを与える
			bullet->GetCollision()->CheckCollision(enemy->GetCollision(), enemyResult);
			if (enemyResult.isHit && bullet->GetOwner() == PLAYER)
			{
				bullet->Hit();
				enemy->HitDamage(1);
				VECTOR effectPos = VGet(enemy->GetPos3D().x, enemy->GetPos3D().y + EFFECT_OFFSET_PLAYERSHOT, enemy->GetPos3D().z);
				EffekseerManager::GetInstance()->PlayEffect(6, effectPos);
				SoundManager::GetInstance()->Play_SE(SE_GRAVITYORB_HIT,false);
			}
		}
	}

	// プレイヤーと敵の当たり判定
	for (auto& enemy : enemyList)
	{
		CollisionResult attackResult;
		CollisionResult rushResult;
		CollisionResult targetResult;


		// プレイヤーと敵の攻撃当たり判定
		if (enemy->GetAttackCollison())
		{
			player->GetCollision()->CheckCollision(enemy->GetAttackCollison(), attackResult);
		}
		
		// プレイヤーが攻撃に当たっていたらダメージを与える
		if (attackResult.isHit && m_PlayerInvincibleTime <= 0)
		{
			enemy->HitAttack();
			player->HitDamage(1);
			player->SetMove({});
			player->SetGetDamage(true);
			player->SetIsHitAttack(true);
			player->SetIsHitAttackFrame(INVINCIBLE_DURATION);
			SoundManager::GetInstance()->Play_SE(SE_ENEMY_ATTACK, false);
			
			// 無敵状態にする
			m_PlayerInvincibleTime = INVINCIBLE_DURATION;
		}
		
		// プレイヤーと敵の突進当たり判定
		player->GetCollision()->CheckCollision(enemy->GetRushCollision(),rushResult);
		
		// プレイヤーが突進に当たっていたらダメージを与える
		if (rushResult.isHit && m_PlayerInvincibleTime <= 0)
		{
			enemy->HitRush();
			player->HitDamage(2);
			player->SetMove({});
			player->SetGetDamage(true);
			player->SetIsHitAttack(true);
			player->SetIsHitAttackFrame(INVINCIBLE_DURATION);
			// SEとエフェクトを再生
			SoundManager::GetInstance()->Play_SE(SE_ENEMY_BUMP, false);
			EffekseerManager::GetInstance()->PlayEffect(2, player->GetPos3D());
			// 無敵状態にする
			m_PlayerInvincibleTime = INVINCIBLE_DURATION;
		}		

		// プレイヤーのターゲット可能範囲との当たり判定
		player->GetTargetCollision()->CheckCollision(enemy->GetTargetCollision(),targetResult);

		// ターゲット可能範囲に入っていたらターゲット可能にする
		if (targetResult.isHit)
		{
			enemy->SetEnableTarget(true);
		}
		else
		{
			enemy->SetEnableTarget(false);
		}
	}

	// エネミーとエネミーの当たり判定
	for (auto& enemy : enemyList)
	{
		for (auto& otherEnemy : enemyList)
		{
			// 自分自身とは当たり判定をしない
			if (enemy.get() == otherEnemy.get())
			{
				continue;
			}

			CollisionResult enemyResult;
			// エネミー同士の当たり判定
			enemy->GetTargetCollision()->CheckCollision(otherEnemy->GetTargetCollision(), enemyResult);

			// エネミー同士が当たっていたら
			if (enemyResult.isHit)
			{
				// どちらかがターゲットされていて、どちらかが重力操作の発射状態だったら両方消滅させる
				if (enemy->GetIsTargeting() && otherEnemy->GetGravityCommand().IsShot)
				{
					// プレイヤーの状態を変更
					player->SetMode(MODE_NORMAL);
					player->SetShotMode(SHOT_NORMAL);
					// プレイヤーの敵操作中フラグを解除
					player->SetIsControllEnemy(false);
					// ヒット時のカメラエフェクトを起動
					CameraManager::GetInstance()->GetCamera()->AddHitEffect(0.36f, 2.0f, 7.0f);
					// カメラを通常カメラに変更
					CameraManager::GetInstance()->SetNormal();
					// 敵を消滅
					enemy->Vanish();
					otherEnemy->Vanish();
					// エフェクトを再生
					VECTOR effectPos = VGet(otherEnemy->GetPos3D().x, otherEnemy->GetPos3D().y + EFFECT_OFFSET_CONTROLLENEMYBUMP, otherEnemy->GetPos3D().z);
					VECTOR effectPos2 = VGet(enemy->GetPos3D().x, enemy->GetPos3D().y + EFFECT_OFFSET_CONTROLLSHOT, enemy->GetPos3D().z);
					EffekseerManager::GetInstance()->PlayEffect(0,effectPos);
					EffekseerManager::GetInstance()->PlayEffect(1,effectPos2);
					// SEを再生
					SoundManager::GetInstance()->Play_SE(SE_CONTROLOBJECT_HIT, false);
				}
			}
		}
	}

	// プレイヤーとギミックの当たり判定
	for (auto& gimmick : gimmickList)
	{
		CollisionResult gimmickResult;
		// プレイヤーとギミックの当たり判定
		player->GetCollision()->CheckCollision(gimmick->GetCollision(), gimmickResult);
		// プレイヤーとギミックの当たり判定を更新する
		gimmick->UpdateCollision(gimmickResult.isHit);

		if (gimmickResult.isHit)
		{
			int test = 0;
		}

		if (gimmick->GetIsPushing())
		{
			player->HitBodyAnyObject(&gimmickResult);
		}
	}

	// 各オブジェクトのステージとの当たり判定
	player->CheckStageCollision();
	for (auto& enemy : enemyList)
	{
		if (!enemy->GetIsControlling())
		{
			enemy->CheckStageCollision();
		}
	}
	for (auto& stageObject : stageObjectList)
	{
		stageObject->CheckStageCollision();
	}
	for (auto& gimmick : gimmickList)
	{
		gimmick->CheckStageCollision();
	}
	for (auto& pullObject : pullObjectList)
	{
		pullObject->CheckStageCollision();
	}

	player->UpdateGroundState();
}

void CollisionManager::UpdateInvincible()
{
	// 無敵時間が残っている場合は減らす
	if (m_PlayerInvincibleTime > 0)
	{
		m_PlayerInvincibleTime--;
	}
}
