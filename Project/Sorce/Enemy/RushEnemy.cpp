#include "RushEnemy.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Mechanism/IDManager.h"
#include "../Player/Player.h"
#include "../FPS/FPS.h"
#include "../Camera/Camera.h"
#include "../Camera/CameraManager.h"
#include "../UI/UIManager.h"
#include "../UI/UIBase3D.h"
#include "../UI/UIEnemyHP.h"
#include "../Sound/SoundManager.h"
#include "../MyEffekseer/EffekseerManager.h"

static constexpr float CHASE_DISTANCE = 400.0f;
static constexpr float RUSH_DISTANCEMAX = 250.0f;
static constexpr float RUSH_DISTANCEMIN = 150.0f;
static constexpr float ATTACK_DISTANCE = 10.0f;
static constexpr float RUSH_SPEED = 0.5f;
static constexpr float RUSH_CHARGE_TIME = 4.0f;
static constexpr float RUSH_FINSH_TIME = 20.0f;
static constexpr float RUSHCOLLISON_DISTANCE = 1.0f;
static constexpr float ATTACKCOLLISON_DISTANCE = 3.0f;
static constexpr float ATTACK_RETIME = 120.0f;
static constexpr float ATTACK_COLLISION_START_TIME = 60.0f;
static constexpr float ATTACK_COLLISION_END_TIME = 90.0f;
static constexpr float EFFECT_OFFSET_ENEMYSLASH_Y = 2.0f;
static constexpr float RUSHENEMY_HPUI_OFFSET_Y = 3.5f;
static constexpr float RUSHENEMY_STATE_HYSTERESIS = 1.5f;
static constexpr float RUSHENEMY_ATTACKSTATE_HYSTERESIS = 0.5f;


RushEnemy::RushEnemy():
	m_RushChargeTime(0.0f),
	m_FinishRushTime(0.0f),
	m_StartRush(false),
	m_RushDir({}),
	m_IsRushing(false),
	m_CanAttack(false),
	m_CanRush(false),
	m_CanChase(false)
{
}

void RushEnemy::Load()
{
	// 敵のモデルをロード
	std::shared_ptr<MyResource> rushEnemyModel = MyResourceManager::GetInstance()->LoadModelResource("Data/Enemy/RushEnemy/RushEnemy.x", false, true);

	// モデルのハンドルを取得
	if (rushEnemyModel)
	{
		m_Handle = rushEnemyModel->GetHandle();
	}
}

void RushEnemy::OnStart()
{
	// タイプ名を設定
	SetTypeName("RushEnemy");

	// 再攻撃までの待機時間を初期化
	m_ReAttackTime = ATTACK_RETIME;
	// 突進チャージ時間を初期化
	m_RushChargeTime = RUSH_CHARGE_TIME;
	// 突進終了時間を初期化
	m_FinishRushTime = RUSH_FINSH_TIME;

	// HPを初期化
	m_MaxHP = 3;
	m_HP = m_MaxHP;

	// UIの生成
	UIBase3D* ui = UIManager::GetInstance()->CreateUI3D(UI_ID_ENEMY_HP, m_Transform.position_3D);
	// 敵HPUIの位置を提供する関数を設定
	ui->SetPositionProvider([this]()
		{
			return VGet(m_Transform.position_3D.x, m_Transform.position_3D.y + RUSHENEMY_HPUI_OFFSET_Y, m_Transform.position_3D.z);
		});

	// 敵HPUIのHPデータを提供する関数を設定
	m_UIEnemyHP = static_cast<UIEnemyHP*>(ui);
	// 敵のHPデータを提供するラムダ関数を設定
	m_UIEnemyHP->SetEnemyHPProvider([this]()
		{
			EnemyHPData data;
			data.value = static_cast<float>(m_HP);
			data.maxValue = static_cast<float>(m_MaxHP);
			return data;
		});

	// 敵の操作状態を提供するラムダ関数を設定
	m_UIEnemyHP->SetEnemyControlStateProvider([this]()
		{
			return m_IsControlling;
		});

	// ID登録
	IDManager::GetInstance()->Register(this);
}

UniquePtr<Enemy> RushEnemy::Clone()
{
	// クローンを生成
	UniquePtr<RushEnemy> clone = MakeUnique<RushEnemy>();
	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}

void RushEnemy::ControllAnimation()
{
	// 状態によってアニメーションを切り替える
	switch (m_State)
	{
	case IDEL:
		PlayAnimation(RUSHENEMY_IDLEING, true);
		break;
	case CHASE:
		PlayAnimation(RUSHENEMY_WALKING, true);
		break;
	case RUSHCHARGE:
		PlayAnimation(RUSHENEMY_RUSHCHARGEING, false);
		break;
	case RUSHATTACK:
		PlayAnimation(RUSHENEMY_RUSHATTACKING, false);
		break;
	case ATTACK:
		PlayAnimation(RUSHENEMY_ATTACKING, false);
		break;
	case GRAVTYMODE:
		PlayAnimation(RUSHENEMY_FLOATING, true);
		break;
	default:
		break;
	}
}

void RushEnemy::HitRush()
{
	// 移動量を初期化
	m_Move = {};
	// 当たる前の位置に戻す
	m_Transform.position_3D = m_PrevPos;
	// 当たり判定をなくす
	m_RushCollison->SetIsActive(false);
	m_RushCollison = nullptr;
	// 待機状態に移行
	m_State = IDEL;
}

void RushEnemy::HitAttack()
{
	// 攻撃の当たり判定と攻撃フラグをリセット
	m_AttackCollison->SetIsActive(false);
	m_AttackCollison = nullptr;
	m_HitAttack = false;
}

void RushEnemy::UpdateState(float distance)
{
	// 突進チャージ、突進攻撃、攻撃中は状態を更新しない
	if (m_State == RUSHCHARGE || m_State == RUSHATTACK || m_State == ATTACK) return;


	// 距離によってフラグを更新
	if (!CameraManager::GetInstance()->GetCamera()->IsInView(m_Transform.position_3D))return;

	EnemyState newState;

	// フラグによって状態を更新
    if (distance <= ATTACK_DISTANCE)
		newState = ATTACK;
    else if (distance >= RUSH_DISTANCEMIN && distance <= RUSH_DISTANCEMAX)
		newState = RUSHCHARGE;
	else if (distance <= CHASE_DISTANCE)
		newState = CHASE;
	else
		newState = IDEL;

	// ヒステリシスを適用
	if (newState != m_State)
	{
		switch (m_State)
		{
		case IDEL:
			if (newState == ATTACK && distance > ATTACK_DISTANCE - RUSHENEMY_ATTACKSTATE_HYSTERESIS)
				return;
			break;
		case CHASE:
			if (newState == IDEL && distance <= CHASE_DISTANCE + RUSHENEMY_STATE_HYSTERESIS)
				return;
			break;
		case ATTACK:
			if (newState == CHASE && distance <= ATTACK_DISTANCE + RUSHENEMY_STATE_HYSTERESIS)
				return;
			break;
		case RUSHCHARGE:
		case RUSHATTACK:
			if (newState == CHASE && distance <= RUSH_DISTANCEMIN + RUSHENEMY_STATE_HYSTERESIS)
				return;
			break;
		default:
			break;
		}
		m_NextState = newState;
	}

	
}

void RushEnemy::Attack()
{
	// 移動量をリセット
	m_Move = {};

	// 攻撃の向きを決定
	VECTOR attackDir = MyMath::VecCreate(m_Transform.position_3D, m_Player->GetPos3D());
	attackDir = MyMath::VecNormalize(attackDir);

	// 回転の目標値を決定
	Quaternion targetRot = MyQuaternion::QGetTargetRotXZ(attackDir,m_Up);

	// 回転を補間
	m_Transform.rotation = targetRot;

	// 再攻撃までの待機時間を減少
	m_ReAttackTime -= 1.0f;

	// 指定タイミングになったら当たり判定を生成
	if (!m_HitAttack && m_ReAttackTime <= ATTACK_RETIME - ATTACK_COLLISION_START_TIME)
	{
		// 攻撃の当たり判定を生成
		m_AttackCollison = CollisionManager::GetInstance()->CreateAABB();
		// 当たり判定の位置を敵の位置に設定
		m_AttackCollison->SetTargetPos(&m_Transform.position_3D);
		// 当たり判定のオフセットを攻撃の向きに離れた位置に設定
		VECTOR offsetPos = MyMath::VecScale(attackDir, ATTACKCOLLISON_DISTANCE);
		// オフセットの高さを調整
		offsetPos.y = 1.5f;
		// 当たり判定のオフセットを設定
		m_AttackCollison->SetOffsetPos(offsetPos);
		// 当たり判定のサイズを設定
		m_AttackCollison->SetSize(VGet(3.0f, 3.0f, 3.0f));
		VECTOR effectPos = VGet(m_Transform.position_3D.x, m_Transform.position_3D.y + EFFECT_OFFSET_ENEMYSLASH_Y, m_Transform.position_3D.z);
		// エフェクトを再生
		EffekseerManager::GetInstance()->PlayEffect(8, effectPos);
		// 攻撃を行ったフラグを立てる
		m_HitAttack = true;
	}

	// 指定タイミングになったら当たり判定をなくす
	if (m_HitAttack && m_ReAttackTime <= ATTACK_RETIME - ATTACK_COLLISION_END_TIME && m_AttackCollison)
	{
		// 当たり判定をなくす
		m_AttackCollison->SetIsActive(false);
		m_AttackCollison = nullptr;
	}

	
	// 時間が0以下で攻撃関数を抜ける
	if (m_ReAttackTime < 0)
	{
		// 再攻撃までの待機時間をリセット
		m_ReAttackTime = ATTACK_RETIME;
		// 攻撃の当たり判定をなくす
		m_HitAttack = false;
		// 待機状態に移行
		m_NextState = IDEL;
	}
}

void RushEnemy::RushMove()
{
	// 移動量を設定
	m_Move = MyMath::VecScale(m_RushDir, RUSH_SPEED);

	// 突進用の当たり判定を作成
	if (m_StartRush)
	{	
		// 当たり判定を生成
		m_RushCollison = CollisionManager::GetInstance()->CreateAABB();
		// 当たり判定の位置を敵の位置に設定
		m_RushCollison->SetTargetPos(&m_Transform.position_3D);
		// 当たり判定のオフセットを突進の向きに離れた位置に設定
		VECTOR offsetPos = MyMath::VecScale(m_RushDir, RUSHCOLLISON_DISTANCE);
		// オフセットの高さを調整
		offsetPos.y = 1.5f;
		// 当たり判定のオフセットを設定
		m_RushCollison->SetOffsetPos(offsetPos);
		// 当たり判定のサイズを設定
		m_RushCollison->SetSize(VGet(3.0f, 3.0f, 3.0f));
		// 突進開始フラグを下ろす
		m_StartRush = false;	
	}

	// 突進終了時間を減少
	m_FinishRushTime -= 0.5f;

	// 突進終了時間が0以下になったら突進終了
	if (m_FinishRushTime < 0)
	{
		// 移動量を初期化
		m_Move = {};

		// 当たり判定が生きてたら
		if (m_RushCollison && m_RushCollison->GetIsActive())
		{
			// 当たり判定をなくす
			m_RushCollison->SetIsActive(false);
			m_RushCollison = nullptr;
		}
		// 突進終了時間をリセット
		m_FinishRushTime = RUSH_FINSH_TIME;
		// 突進中フラグを下ろす
		m_IsRushing = false;
		// 待機状態に移行
		m_NextState = IDEL;		
	}
}

// 突進チャージ関数
void RushEnemy::RushCharge()
{
	// 移動量を初期化
	m_Move = {};
	// 突進の向きを決定
	m_RushDir = MyMath::VecCreate(m_Transform.position_3D, m_Player->GetPos3D());
	// 突進の向きのy成分を0にして水平なベクトルにする
	m_RushDir = VGet(m_RushDir.x, 0.0f, m_RushDir.z);
	// 突進の向きを正規化
	m_RushDir = MyMath::VecNormalize(m_RushDir);

	// 回転の目標値を決定
	Quaternion targetRot = MyQuaternion::QGetTargetRotXZ(m_RushDir, m_Up);
	// 回転を補間
	m_Transform.rotation = targetRot;

	// 突進チャージタイムを減少
	m_RushChargeTime -= 0.1f;
	// 突進チャージタイムが0以下になったら突進開始
	if (m_RushChargeTime < 0)
	{
		// 突進スタート
		m_StartRush = true;
		//　突進中フラグ
		m_IsRushing = true;
		// 突進に移行
		m_NextState = RUSHATTACK;
		// 突進のチャージ時間をリセット
		m_RushChargeTime = RUSH_CHARGE_TIME;		
	}
}