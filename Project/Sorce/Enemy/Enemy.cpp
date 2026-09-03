#include "Enemy.h"
#include "../MyMath/MyMath.h"
#include "../Quatrenion/Quatrenion.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionStage.h"
#include "../Collision/Capsule.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionSphere.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../TestStage/TestStage.h"
#include "../MyEffekseer/EffekseerManager.h"
#include "../Score/ScoreManager.h"
#include "../Score/Score.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIEnemyHP.h"
#include "../Camera/CameraManager.h"
#include "../Target/TargetManager.h"
#include "../Target/Target.h"


static constexpr float GRAVITY = 0.01f;
static constexpr float ENEMY_HEIGHT = 2.7f;
static constexpr float VANISH_TIME = 20.0f;

static constexpr float EFFECT_INTERVAL = 60.0f;
static constexpr float ANIMATION_SPEED = 1.0f;

static constexpr float STATECHANGE_ENABLE_HEIGHT_DIFF = 5.0f;

Enemy::Enemy() :
	// 変数の初期化リスト
	m_AttackCollison(nullptr),
	m_RushCollison(nullptr),
	m_UIEnemyHP(nullptr),
	m_Player(nullptr),
	m_HitAttack(false),
	m_IsTargeting(false),
	m_IsControlling(false),
	m_IsVanishWaitTimer(false),
	m_EnableTarget(false),
	m_EnableControll(false),
	m_distance(0),
	m_ReAttackTime(0),
	m_VanishTime(0),
	m_StateTable{},
	m_State(IDEL),
	m_NextState(IDEL),
	m_EffectInterval(0)
{
}

void Enemy::Start()
{
	// プレイヤーを参照
	m_Player = PlayerManager::GetInstance()->GetPlayer().get();

	// アニメーションの再生速度を設定
	m_AnimationSpeed = ANIMATION_SPEED;
	// 生存フラグをオン
	m_IsActive = true;

	// 重力の方向を設定
	m_GravityDir = VGet(0.0f, -1.0f, 0.0f);

	// ステージ用当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 2.3f, 0.0f));
	m_CapsuleCollision->SetHeight(ENEMY_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ターゲット用当たり判定を設定
	m_TargetCollision = CollisionManager::GetInstance()->CreateSphere();
	m_TargetCollision->SetTargetPos(&m_Transform.position_3D);
	m_TargetCollision->SetOffsetPos(VGet(0.0f, 1.5f, 0.0f));
	m_TargetCollision->SetRadius(3.0f);

	// 汎用当たり判定を設定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 1.5f, 0.0f));
	m_Collision->SetSize(VGet(1.0f, ENEMY_HEIGHT*1.2f, 1.0f));

	// 上ベクトル
	m_Up = VGet(0.0f, 1.0f, 0.0f);
	// 前方ベクトル
	m_Forward = VGet(0.0f, 0.0f, 1.0f);
	// 右ベクトル
	m_Right = VGet(1.0f, 0.0f, 0.0f);

	// 消滅待機タイマーを初期化
	m_VanishTime = VANISH_TIME;

	// 床との当たり判定を有効にする
	m_EnableFloorCheck = true;

	// HPUIを初期化
	m_UIEnemyHP = nullptr;

	// 敵の状態関数ポインタを初期化
	m_StateTable[IDEL] = &Enemy::Idel;
	m_StateTable[CHASE] = &Enemy::Move;
	m_StateTable[ATTACK] = &Enemy::Attack;
	m_StateTable[RUSHCHARGE] = &Enemy::RushCharge;
	m_StateTable[RUSHATTACK] = &Enemy::RushMove;
	m_StateTable[SHOT] = &Enemy::EnemyShot;
	m_StateTable[ESCAPE] = &Enemy::EnemyEscape;
	m_StateTable[GRAVTYMODE] = &Enemy::GravityModeUpdate;

	// 各敵のStart関数
	OnStart();
}

void Enemy::Step()
{
	// プレイヤーとの距離を計算
	m_distance = MyMath::DistanceXZ(m_Transform.position_3D, m_Player->GetPos3D());

	// 状態遷移の更新
	if ((m_State == IDEL || m_State == CHASE) && m_EnableControll == false)
	{
		UpdateState(m_distance);
	}

	// アニメーション関数
	ControllAnimation();

	// 状態によって関数を呼び出す
	(this->*m_StateTable[m_State])();

	// 状態の更新
	if (m_State != m_NextState)
	{
		// エネミーとプレイヤーのいるｙ座標の差
		float heightDiff = fabsf(m_Transform.position_3D.y - m_Player->GetPos3D().y);
		// プレイヤーが自分よりも一定以上高い位置にいる場合
		if (heightDiff > STATECHANGE_ENABLE_HEIGHT_DIFF)return;

		m_State = m_NextState;
	}

	// 通常時の重力
	if (m_State != GRAVTYMODE)
	{
		m_Move.y -= GRAVITY;
	}

	// 移動
	m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, m_Move);
	//前回の位置を保持
	m_PrevPos = m_Transform.position_3D;

	// 死亡判定
	if (m_HP <= 0 && !m_EnableControll)
	{
		// 死亡処理関数
		Die();
	}

	// 敵操作中エフェクト再生
	if (m_State == GRAVTYMODE)
	{
		VECTOR effectPos = m_Transform.position_3D;
		effectPos.y += 1.0f;
	
		EffekseerManager::GetInstance()->PlayEffect(5, effectPos);
	}

	// 敵ターゲットされ中
	if (m_IsTargeting)
	{
		VECTOR effectPos = m_Transform.position_3D;
		effectPos.y += 1.0f;

		m_EffectInterval -= 1.0f;

		if (m_EffectInterval <= 0.0f)
		{
			EffekseerManager::GetInstance()->PlayEffectTracking(7, &m_Transform.position_3D,&m_IsActive);
			m_EffectInterval = EFFECT_INTERVAL;
			SoundManager::GetInstance()->Play_SE(SE_TARGET, false);
		}
	}
	else
	{
		if (m_EffectInterval > 0.0f)
		{
			m_EffectInterval = 0.0f;
		}
	}

	// 消滅フラグが立っていて、操作されていないときは消滅待機タイマーを減らす
	if (m_IsVanishWaitTimer && !m_IsControlling)
	{
		m_VanishTime -= 0.1f;
	}

	// 消滅待機タイマーが0以下になったら消滅
	if (m_VanishTime <= 0)
	{
		// 消滅関数
		Vanish();
	}
}

// 待機関数
void Enemy::Idel()
{	
}

// 移動関数
void Enemy::Move()
{
	// エネミーからプレイヤーへの方向ベクトル
	VECTOR dir = MyMath::VecCreate(m_Transform.position_3D, m_Player->GetPos3D());
	// 移動方向を正規化
	dir = MyMath::VecNormalize(dir);
	// 移動方向のy成分を消す
	dir.y = 0.0f;
	// 回転の目標値を決定
	Quaternion targetRot = MyQuaternion::QGetTargetRotXZ(dir, m_Up);	
	// 回転を補間
	m_Transform.rotation = targetRot;
	// 移動量を設定
	m_Move = MyMath::VecScale(dir, 0.05f);
}

// 当たり判定同期関数
void Enemy::SyncCollision()
{
	// 当たり判定の回転と位置を更新
	if (m_CapsuleCollision)
	{
		m_CapsuleCollision->SetRotation(MyQuaternion::Qconjugate(m_Transform.rotation));
		m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	}
}

// 重力操作の更新関数
void Enemy::GravityModeUpdate()
{
	// 基底クラスの重力操作の更新関数を呼び出す
	GameObject3D::GravityModeUpdate();	
}

// 死亡処理関数
void Enemy::Die()
{
	// 操作可能フラグをオン
	m_EnableControll = true;
	// ターゲット可能フラグをオン
	m_IsVanishWaitTimer = true;
	// 状態を待機にする
	m_State = IDEL;

	// 死ぬ前に発生させていた攻撃用コリジョンをnullに
	if (m_RushCollison)
	{
		m_RushCollison->SetIsActive(false);
		m_RushCollison = nullptr;
	}

	if (m_AttackCollison)
	{
		m_AttackCollison->SetIsActive(false);
		m_AttackCollison = nullptr;
	}
	
}

// 消滅関数
void Enemy::Vanish()
{
	// 生存フラグをオフ
	m_IsActive = false;
	VECTOR effectPos = m_Transform.position_3D;
	effectPos.y += 2.0f;
	// エフェクトを再生
	EffekseerManager::GetInstance()->PlayEffect(9, effectPos);

	// UIを消す
	m_UIEnemyHP->Hide();

	// ターゲットをクリア
	TargetManager::GetInstance()->GetTarget().get()->ClearTarget(false);
	// スコアを加算
	ScoreManager::GetInstance()->GetScore().get()->AddScore(100);
}
