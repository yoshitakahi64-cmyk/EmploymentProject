#include "ShotEnemy.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Mechanism/IDManager.h"
#include "../Player/Player.h"
#include "../FPS/FPS.h"
#include "../UI/UIManager.h"
#include "../UI/UIBase3D.h"
#include "../UI/UIEnemyHP.h"

static constexpr float SHOT_DISTANCE = 300.0f;
static constexpr float ESCAPE_DISTANCE = 50.0f;
static constexpr float ABLE_ESCAPE_DISTANCE = 50.0f;
static constexpr float ESCAPE_TIME = 100.0f;
static constexpr float SHOTENEMY_SHOT_SPEED = 12.0f;
static constexpr float ATTACKCOLLISON_DISTANCE = 1.5f;
static constexpr float SHOT_RETIME = 120.0f;
static constexpr float SHOT_OFFSET_Y = 1.0f;
static constexpr float ESCAPE_FORCE = 0.1f;
static constexpr int SHOT_ANIMTIMER = 35;
static constexpr float SHOTENEMY_HPUI_OFFSET_Y = 2.5f;
static constexpr float SHOTENEMY_STATE_HYSTERESIS = 1.5f;

ShotEnemy::ShotEnemy():
	m_ReShotTime(0),
	m_EscapeTime(0),
	m_EscapeDir({}),
	m_EscapeStartPos({}),
	m_IsdecideEscapeDir(false),
	m_IsShooting(false),
	m_ShotAnimTimer(0)
{
}

void ShotEnemy::OnStart()
{
	// 射撃の発射間隔を初期化
	m_ReShotTime = SHOT_RETIME;
	// 逃走継続時間を初期化
	m_EscapeTime = ESCAPE_TIME;
	// 逃走方向決定フラグを初期化
	m_IsdecideEscapeDir = false;

	// HPを設定
	m_HP = 1;
	m_MaxHP = m_HP;

	// UIの生成
	UIBase3D* ui = UIManager::GetInstance()->CreateUI3D(UI_ID_ENEMY_HP, m_Transform.position_3D);
	// UIの位置を提供するラムダ関数を設定
	ui->SetPositionProvider([this]()
		{
			return VGet(m_Transform.position_3D.x, m_Transform.position_3D.y + SHOTENEMY_HPUI_OFFSET_Y, m_Transform.position_3D.z);
		});

	// 敵HPUIのHPデータを提供するラムダ関数を設定
	m_UIEnemyHP = static_cast<UIEnemyHP*>(ui);
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

	// IDを登録
	IDManager::GetInstance()->Register(this);
}

void ShotEnemy::Load()
{
	// 敵のモデルをロード
	std::shared_ptr<MyResource> shotEnemyModel = MyResourceManager::GetInstance()->LoadModelResource("Data/Enemy/ShotEnemy/ShotEnemy.x", false, true);

	// モデルのハンドルを取得
	if (shotEnemyModel)
	{
		m_Handle = shotEnemyModel->GetHandle();
	}
}

UniquePtr<Enemy> ShotEnemy::Clone()
{
	// クローンを生成
	UniquePtr<ShotEnemy> clone = MakeUnique<ShotEnemy>();
	// 変数をコピー
	*clone = *this;
	// モデルのハンドルを複製
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}

void ShotEnemy::ControllAnimation()
{
	// 状態によってアニメーションを切り替える
	switch (m_State)
	{
	case IDEL:
	case SHOT:
		if (m_IsShooting)
		{
			// タイマーを毎フレーム減らす
			m_ShotAnimTimer--;
			if (m_ShotAnimTimer <= 0)
			{
				m_IsShooting = false;
			}
			PlayAnimation(SHOTENEMY_SHOTING, false);
		}
		else
		{
			PlayAnimation(SHOTENEMY_IDLEING, true);
		}
		break;
	case ESCAPE:
		PlayAnimation(SHOTENEMY_RUNNING, true);
		break;
	case GRAVTYMODE:
		PlayAnimation(SHOTENEMY_FLOATING, true);
		break;
	default:
		break;
	}
}

void ShotEnemy::UpdateState(float distance)
{
	// 逃走状態に遷移
	if (distance < ESCAPE_DISTANCE)
	{
		m_NextState = ESCAPE;
		m_EscapeStartPos = m_Transform.position_3D;
	}
	// 射撃状態に遷移
	if (distance < SHOT_DISTANCE)
	{
		if (m_State == ESCAPE)
		{
			// ヒステリシスを適用
			if (distance > ESCAPE_DISTANCE + SHOTENEMY_STATE_HYSTERESIS)
				m_NextState = SHOT;
			return;
		}

		m_NextState = SHOT;
	}
	// 待機状態に遷移
	else
	{
		// ヒステリシスを適用
		if (m_State == SHOT && distance <= SHOT_DISTANCE + SHOTENEMY_STATE_HYSTERESIS) return;

		m_NextState = IDEL;
		m_ReShotTime = SHOT_RETIME;
	}
}

void ShotEnemy::EnemyShot()
{
	// 攻撃の向きを決定
	VECTOR shotDir = MyMath::VecCreate(m_Transform.position_3D, m_Player->GetPos3D());
	// 攻撃の向きを正規化
	shotDir = MyMath::VecNormalize(shotDir);

	// 回転の目標値を決定
	Quaternion targetRot = MyQuaternion::QGetTargetRotXZ(shotDir, m_Up);
	// 回転を補間
	m_Transform.rotation = targetRot;

	// 水平なベクトルにする
	shotDir.y = 0.0f;

	// 射撃の発射間隔を減少
	m_ReShotTime -= 1.0f;

	// 弾を発射
	if (m_ReShotTime < 0)
	{
		// 弾の発射位置を決定
		VECTOR shotPos = m_Transform.position_3D;
		// 弾の発射位置を敵の前方にオフセット
		shotPos.y += SHOT_OFFSET_Y;
		// 弾を発射
		m_Shot.Fire(shotPos, shotDir, ENEMY, SHOTENEMY_SHOT_SPEED);
		// 射撃の発射間隔をリセット
		m_ReShotTime = SHOT_RETIME;
		// 待機状態に遷移
		m_NextState = IDEL;

		// 発射フラグをオン
		m_IsShooting = true;
		m_ShotAnimTimer = SHOT_ANIMTIMER;
	}
}

void ShotEnemy::EnemyEscape()
{
	// 逃走方向が決まってない場合は逃走方向を決定
	if (!m_IsdecideEscapeDir)
	{
		// 敵からプレイヤーへのベクトルを計算
		VECTOR toPlayer = MyMath::VecCreate(m_Transform.position_3D, m_Player->GetPos3D());

		toPlayer = MyMath::VecNormalize(toPlayer);
		toPlayer.y = 0.0f;

		// プレイヤーから敵へのベクトルを逃走方向に設定
		m_EscapeDir = MyMath::VecScale(toPlayer, -1.0f);
		
		// 軸と方向ベクトルから回転を計算
		Quaternion targetRot = MyQuaternion::QGetTargetRotXZ(m_EscapeDir, m_Up);
		// 回転を補間
		m_Transform.rotation = targetRot;
		// 逃走方向が決定したフラグを立てる
	    m_IsdecideEscapeDir = true;
	}
	
	// 逃走方向に移動
	m_Move = MyMath::VecScale(MyMath::VecNormalize(m_EscapeDir), ESCAPE_FORCE);
	// 逃走開始位置からの距離を計算
	float escapeDistance = MyMath::DistanceXZ(m_EscapeStartPos, m_Transform.position_3D);

	// 逃走開始位置から一定距離以上離れたら逃走終了
	if (escapeDistance >= ABLE_ESCAPE_DISTANCE)
	{
		// 移動量を初期化
		m_Move = {};
		// 逃走方向決定フラグを初期化
		m_IsdecideEscapeDir = false;
		// 逃走継続時間をリセット
		m_NextState = IDEL;
	}

	// 逃走継続時間を減少
	m_EscapeTime -= 1.0f;
	// 逃走継続時間が0以下になったら逃走終了
	if (m_EscapeTime < 0)
	{
		// 移動量を初期化
		m_Move = {};
		// 逃走方向決定フラグを初期化
		m_IsdecideEscapeDir = false;
		// 逃走継続時間をリセット
		m_EscapeTime = ESCAPE_TIME;
		// 待機状態に遷移
		m_State = IDEL;
	}
}
