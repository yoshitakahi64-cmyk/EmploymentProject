#include "Player.h"
#include "../MyMath/MyMath.h"
#include "../Quatrenion/Quatrenion.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"
#include "../Collision/CollisionManager.h"
#include "../Camera/Camera.h"
#include "../Camera/CameraManager.h"
#include "../Collision/Capsule.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionSphere.h"
#include "../Collision/CollisionStage.h"
#include "../Mechanism/IDManager.h"
#include "../Enemy/Enemy.h"
#include "../Target/Target.h"
#include "../Target/TargetManager.h"
#include "../StageObject/StageObject.h"
#include "../TestStage/TestStage.h"
#include "../TestStage/TestStageManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Sound/SoundManager.h"
#include "../Scene/SceneManager.h"
#include "../MyEffekseer/EffekseerManager.h"

static constexpr float WALK_SPEED = 0.2f;
static constexpr float RUN_SPEED = 0.3f;
static constexpr float PLAYER_HEIGHT = 2.0f;
static constexpr float JUMP_FORCE = 0.3f;
static constexpr float COYOTETIMER = 0.01f;
static constexpr float ABLERESHOT_TIME = 3.0f;
static constexpr float ENABLE_CONTROLL_ENEMY_DIST = 400.0f;
static constexpr float TARGET_DIST = 2.0f;
static constexpr float TARGETING_ABLE_DIST = 20.0f;
static constexpr float SPRING_K = 0.15f;
static constexpr float TARGET_FORCE = 0.8f;
static constexpr float HOVER_HEIGHT = 3.0f;
static constexpr int HP = 25;
static constexpr float PLAYER_SHOT_SPEED = 10.0f;

Player::Player():
	// 変数の初期化リスト
	m_Mode(MODE_NONE),
	m_MoveSpeed(0.0f),
	m_JumpForce(0.0f),
	m_GravityPower(0.0f),
	m_CoyoteTimer(0.0f),
	m_InputH(0.0f),
	m_InputV(0.0f),
	m_inputLeftState({ 0.0f, 0.0f }),
	m_inputRightState({ 0.0f, 0.0f }),
	m_IsWalk(false),
	m_InputJump(false),
	m_IsAttack(false),
	m_IsShotting(false),
	m_Camera(nullptr),
	m_ControlEnemy(nullptr),
	m_AbleReShotTime(0.0f),
	m_ControllForceDir(VGet(0.0f, 0.0f, 0.0f)),
	m_ControllForce(0.0f),
	m_LastMoveDir(VGet(0.0f, 0.0f, 0.0f)),
	m_MoveDir(VGet(0.0f, 0.0f, 0.0f)),
	m_ShotMode(SHOT_NORMAL),
	m_UseNormalCamera(false),
	m_UseTargetCamera(false),
	m_UseShotCamera(false),
	m_IsDamaged(false),
	m_IsControllEnemy(false)
{
}

Player::~Player()
{

}

void Player::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> playerModel = MyResourceManager::GetInstance()->LoadModelResource("Data/Player/Player.x", false, true);

	// モデルのハンドルを取得して保存
	if (playerModel)
	{
		m_Handle = playerModel->GetHandle();
	}
}

void Player::Start()
{

	m_AnimationSpeed = 0.5f;

	// 生存フラグをオン
	m_IsActive = true;

	// ステージ用当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetUsePlayer(true);
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 2.0f, 0.0f));
	m_CapsuleCollision->SetHeight(PLAYER_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ターゲット用当たり判定を設定
	m_TargetCollision = CollisionManager::GetInstance()->CreateSphere();
	m_TargetCollision->SetTargetPos(&m_Transform.position_3D);
	m_TargetCollision->SetOffsetPos(VGet(0.0f, 1.7f, 0.0f));
	m_TargetCollision->SetRadius(TARGETING_ABLE_DIST);

	// 汎用当たり判定を設定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 1.7f, 0.0f));
	m_Collision->SetSize(VGet(1.0f, PLAYER_HEIGHT*1.2f, 1.0f));

	// 重力の方向を設定
	m_GravityDir = VGet(0.0f, -1.0f, 0.0f);
	// 重力の回転を初期化
	m_GravityRotation = MyQuaternion::QIdentity();
	// 重力の強さを設定
	m_GravityPower = 0.01f;

	// プレイヤーのスピードを設定
	m_MoveSpeed = WALK_SPEED;
	// プレイヤーの状態を通常に設定
	m_Mode = MODE_NORMAL;
	// タイプネームをプレイヤーに設定
	SetTypeName("Player");

	// 向きのベクトルを初期化
	m_Up = VGet(0.0f, 1.0f, 0.0f);
	m_Right = VGet(1.0f, 0.0f, 0.0f);
	m_Forward = VGet(0.0f, 0.0f, 1.0f);

	// 体力を設定
	m_HP = HP;

	// 接地判定を有効にする
	m_EnableFloorCheck = true;

	// 敵操作中フラグをオフ
	m_IsControllEnemy = false;

	// IDを登録
	IDManager::GetInstance()->Register(this);
}

void Player::Step()
{
	// 前回の座標を保存
	m_PrevPos = m_Transform.position_3D;

	// 死亡判定
	if (m_HP <= 0)
	{
		Die();
		return;
	}

	// 射撃リキャスト時間の更新
	if (m_AbleReShotTime >= 0.0f)
	{
		m_AbleReShotTime -= 0.1f;
	}

	// キーの入力を取得
	m_InputV = (float)((Input::IsInputKey(KEY_UP) - Input::IsInputKey(KEY_DOWN)));
	m_InputH = (float)((Input::IsInputKey(KEY_RIGHT) - Input::IsInputKey(KEY_LEFT)));
	// パッドの入力を取得
	m_inputLeftState = Input::GetInputLeftStick();
	m_inputRightState = Input::GetInputRightStick();

	// カメラの使用フラグを取得
	CameraManager::GetInstance()->GetUseFlag(m_UseNormalCamera, m_UseShotCamera, m_UseTargetCamera);

	// コントロール可能な敵の更新
	GetEnableControllEnemy();
	// 敵操作更新
	CheckControllEnemy();

	// ジャンプ関数
	Jump();

	// 移動関数
	Move();

	// ダメージを受けているかどうかのフラグを更新
	if(m_IsDamaged) UpdateDamageFlag();

	// モードごとの処理
	if (m_Mode == MODE_NORMAL)
	{
		// 入力関数
		Input();	
		// 攻撃関数
		if (m_IsAttack)	Attack();
	}
	else if (m_Mode == MODE_CONTROLOBJECT)
	{
		// 重力操作入力関数
		HandleObjectGravityInput();
		// ターゲット操作入力関数
		HandleTargetInput();
	}

	// 重力処理
	VECTOR gravity = MyMath::VecScale(m_GravityDir, m_GravityPower);
	m_Move = MyMath::VecAdd(m_Move, gravity);

	// 速度制限
	float maxFallSpeed = 1.5f;
	float fallSpeed = MyMath::VecDot(m_Move, m_GravityDir);
	if (fallSpeed > maxFallSpeed)
	{
		m_Move = MyMath::VecAdd(
			m_Move,
			MyMath::VecScale(m_GravityDir, maxFallSpeed - fallSpeed)
		);
	}

	// 座標更新
	m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, m_Move);

	// 重力の方向を当たり判定に反映
	m_CapsuleCollision->SetGravity(m_GravityDir);

	// アニメーション再生
	ControllAnimation();
}

void Player::UpdateGroundState()
{
	// 接地判定を更新
	bool grounded = IsGrounded();

	// コヨーテタイマーの更新
	if (grounded)
	{
		m_CoyoteTimer = 0.1f;
	}
	else
	{
		// 空中にいる場合、コヨーテタイマーを減少させる
		m_CoyoteTimer -= COYOTETIMER;
	}

	// ジャンプしていない状態で、コヨーテタイマーが0以下なら落下状態にする
	if (!grounded && m_CoyoteTimer <= 0.0f)
	{
		m_IsFall = true;
	}
	else if (grounded)
	{
		m_IsFall = false;
	}

	// 着地状態の更新
	if (grounded)
	{
		// 着地した瞬間
		if (m_IsJump || m_IsFall)
		{
			m_IsJump = false;
			m_IsFall = false;
			m_Move = {}; // 着地時にY方向速度リセット（必要に応じて）
		}
	}
	else
	{
		// 空中にいて、ジャンプしていないなら＝落下中
		if (!m_IsJump)
		{
			m_IsFall = true;
		}
	}
}

// 入力関数
void Player::Input()
{
	// ダメージを受けている場合は入力を無効化
	if(m_IsDamaged) return;

	// ジャンプ入力
	if (Input::IsTriggerKey(KEY_Z) || Input::IsTriggerButton(BUTTON_A))
	{			
			m_InputJump = true;
			m_IsWalk = false;
	}
	
	// 重力玉発射入力
	if (Input::IsInputKey(KEY_C) || Input::IsInputButton_R() && m_ShotMode == SHOT_NORMAL)
	{
		// 射撃フラグをオンにする
		m_IsShotting = true;
		m_IsWalk = false;
		// 重力玉発射関数を呼び出す
		ShootGravityOrb();
	}
	else
	{
		// 射撃フラグをオフにする
		m_IsShotting = false;
	}

	// エイム切り替え入力
	if (Input::IsInputKey(KEY_SPACE) || Input::IsInputButton_L())
	{
		CameraManager::GetInstance()->SetShot();
	}
	else if(!m_UseNormalCamera)
	{
		CameraManager::GetInstance()->SetNormal();
	}
	


}
// 移動関数
void Player::Move()
{
	// ダメージを受けている場合は移動を無効化
	if(m_IsDamaged) return;

	// 移動方向
	VECTOR moveDir = {};
	// ワールドの上方向ベクトルを計算
	VECTOR worldUp = MyMath::VecScale(m_GravityDir, -1.0f);

	// カメラの状態を取得
	Camera* camera = CameraManager::GetInstance()->GetCamera().get();
	CameraParame camParame = camera->GetParame();

	// カメラ基準の前方向ベクトルを計算
	VECTOR cameraForward = camParame.forward;

	// 重力方向に投影
	VECTOR moveForward = MyMath::ProjectOnPlane(cameraForward, worldUp);
	moveForward = MyMath::VecNormalize(moveForward);

	// 外積で右向きベクトルを計算
	VECTOR moveRight = MyMath::VecCross3D(worldUp, moveForward);
	moveRight = MyMath::VecNormalize(moveRight);

	// 移動方向を合成
	// Key入力の場合
	if ((m_InputV != 0 || m_InputH != 0))
	{
	    // 移動方向を計算
		moveDir = MyMath::VecAdd(MyMath::VecScale(moveForward, m_InputV), MyMath::VecScale(moveRight, m_InputH));
		// 歩くフラグをオンにする
		if (!m_IsJump)
		{
			m_IsWalk = true;
		}
	}
	// Pad入力の場合
	else if ((m_inputLeftState.m_VerticalInput_L != 0 || m_inputLeftState.m_HorizontalInput_L != 0 || 0))
	{
		// 移動方向を計算
		moveDir = MyMath::VecAdd(MyMath::VecScale(moveForward, m_inputLeftState.m_VerticalInput_L), MyMath::VecScale(moveRight, m_inputLeftState.m_HorizontalInput_L));
		// 
		if (!m_IsJump)
		{
			m_IsWalk = true;
		}
	}	
	else
	{
		m_IsWalk = false;
	}

	// 移動方向の長さを計算
	float moveDirLength = MyMath::VecLong(moveDir);

	// 正規化
	if (moveDirLength > 0.0f)
	{			
		// プレイヤーの回転を考慮して移動方向をワールド空間に変換
		MyMath::RotatedVec(moveDir, MyQuaternion::QToMatrix(m_Transform.rotation));
		// 移動方向を正規化
		moveDir = MyMath::VecNormalize(moveDir);
		// プレイヤーの回転を考慮して移動方向をワールド空間に変換
		VECTOR rotateMoveDir = MyQuaternion::QTransformVector(m_GravityRotation, moveDir);
		// 移動方向から目標の回転角度を計算
		float angleTarget = atan2f(rotateMoveDir.x, rotateMoveDir.z);

		// ラジアン → 度
		float angleDeg = angleTarget * 180.0f / DX_PI_F;

		
		UpdateRotate(angleDeg);
	}
	
	// 移動速度変更
	if (m_IsWalk)
	{
		m_MoveSpeed = WALK_SPEED;
	}

	// 移動方向を設定
	m_MoveDir = moveDir;

	m_Move.x = moveDir.x * m_MoveSpeed;
	m_Move.z = moveDir.z * m_MoveSpeed;
}

// ジャンプ関数
void Player::Jump()
{	
	// ジャンプ入力がない場合は処理しない
	if (!m_InputJump)return;

	SoundManager::GetInstance()->Play_SE(SE_JUMP,false);
	
	m_InputJump = false;

	// 接地しているか、コヨーテタイマーが残っている場合にジャンプする
	if (IsGrounded() || m_CoyoteTimer > 0.0f)
	{
		m_Move = MyMath::VecAdd(m_Move, MyMath::VecScale(m_GravityDir, -JUMP_FORCE));
		m_IsJump = true;
		m_IsFall = false;
		m_CoyoteTimer = 0.0f;
	}
}

// アタック関数
void Player::Attack()
{
	// アニメーションの再生が終わったら
	if (m_AnimationNowTime >= m_AnimationTotalTime)
	{
		m_IsAttack = false;
	}
}

void Player::UpdateDamageFlag()
{
	// ダメージのアニメーションの再生が終わったら
	if (m_AnimationNowTime >= m_AnimationTotalTime)
	{
		m_IsDamaged = false;
	}
}

void Player::ControllAnimation()
{
	// 歩くフラグがたったら
	if (m_IsWalk)
	{
		PlayAnimation(PLAYER_WALKING, true);
	}

	// ジャンプフラグがたったら
	if (m_IsJump)
	{
		PlayAnimation(PLAYER_JUMPING, false);
	}

	
	// すべてのフラグが折れていたら
	if (!m_IsWalk && !m_IsJump && !m_IsAttack && !m_IsDamaged && !m_IsShotting)
	{
		PlayAnimation(PLAYER_IDLEING, true);
	}

	if(m_IsShotting)
	{
		PlayAnimation(PLAYER_SHOTING, true);
	}

	if (m_IsDamaged)
	{
		m_IsWalk = false;
		m_IsShotting = false;
		PlayAnimation(PLAYER_GETHITTING, false);
	}
}

// 当たり判定同期関数
void Player::SyncCollision()
{
	// 当たり判定の位置と回転をプレイヤーの位置と回転に合わせる
	m_CapsuleCollision->SetRotation(MyQuaternion::Qconjugate(m_Transform.rotation));
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
}

void Player::UpdateRotate(float angle)
{
	// プレイヤーの回転を更新
	Quaternion targetRot = MyQuaternion::QfromAxisAngleLH(m_Up, angle);
	// 重力の回転を考慮して回転を更新
	targetRot = MyQuaternion::QMultiply(m_GravityRotation, targetRot);
	// 現在の回転から目標の回転に向かって回転を更新
	m_Transform.rotation = MyQuaternion::QSlerp(targetRot, m_Transform.rotation, 0.1f);
}

void Player::ChangeShotMode(ShotMode shotmode)
{
	m_ShotMode = shotmode;
}

void Player::GetEnableControllEnemy()
{
	// コントロール可能な敵のリストをクリア
	m_EnableControllEnemy.clear();
	// 敵のリストを取得
	const List<UniquePtr<Enemy>>& enemyList = EnemyManager::GetInstance()->GetEnemyList();

	// 各敵について、プレイヤーからの距離を計算し、コントロール可能な敵のリストに追加
	for (auto& enemy : enemyList)
	{
		// プレイヤーから敵までの距離を計算
		float dist = MyMath::DistanceXZ(m_Transform.position_3D, enemy.get()->GetPos3D());

		// 距離が一定以下で、敵がコントロール可能な状態であれば、リストに追加
		if (dist < ENABLE_CONTROLL_ENEMY_DIST && enemy.get()->GetEnableControll())
		{
			// コントロール可能な敵のリストに追加
			m_EnableControllEnemy.push_back(enemy.get());
		}
	}

	// コントロール可能な敵のリストを距離が近い順にソート
	m_EnableControllEnemy.sort([&](Enemy* a, Enemy* b)
		{
			float distA = MyMath::DistanceXZ(m_Transform.position_3D, a->GetPos3D());
			float distB = MyMath::DistanceXZ(m_Transform.position_3D, b->GetPos3D());
			return distA < distB;
		});
}

void Player::CheckControllEnemy()
{
	if (m_IsControllEnemy)return;

	// 操作可能な敵がいる場合、最初の敵を操作対象にする
	if (0 < m_EnableControllEnemy.size())
	{
		// 操作モードに切り替え
		m_Mode = MODE_CONTROLOBJECT;
		// 最初の敵を操作対象にする
		m_ControlEnemy = m_EnableControllEnemy.front();
		// 敵の状態を重力操作モードにする
		m_ControlEnemy->SetState(GRAVTYMODE);
		// 敵を操作中にする
		m_ControlEnemy->SetIsControlling(true);
		
		TargetManager::GetInstance()->GetTarget()->ClearTarget(false);

		VECTOR EnemytoPlayer = MyMath::VecCreate(m_ControlEnemy->GetPos3D(), m_Transform.position_3D);
		EnemytoPlayer.y = 0.0f;
		EnemytoPlayer = MyMath::VecNormalize(EnemytoPlayer);

		// 敵を空中に浮かせる
		m_ControlEnemy->SetTransform(MyMath::VecAdd(MyMath::VecAdd(m_ControlEnemy->GetPos3D(), MyMath::VecScale(m_Up, 3.0f)), MyMath::VecScale(EnemytoPlayer, 8.0f)), m_ControlEnemy->GetRotation(), m_ControlEnemy->GetScale());
		// カメラを切り替える
		CameraManager::GetInstance()->SetShot();
		// 敵操作中フラグをオンにする
		m_IsControllEnemy = true;

		if (m_IsShotting)m_IsShotting = false;
	}
}

void Player::Die()
{
	SoundManager::GetInstance()->Stop_BGM(BGM_GAMEPLAY);
	SceneManager::GetInstance()->ChangeScene(GAMEOVER);
}

void Player::ShootGravityOrb()
{
	// カメラの状態を取得
	Camera* camera = CameraManager::GetInstance()->GetCamera().get();
	CameraParame camParame = camera->GetParame();

	// 発射位置をプレイヤーの位置から少し上にオフセットした位置に設定
	VECTOR shotPos = m_Transform.position_3D;
	shotPos.y = shotPos.y + 2.0f;

	// 発射方向をカメラの前方向に設定
	VECTOR shotDir = camParame.forward;
	shotDir.y = 0.0f;

	
	// プレイヤーの回転を考慮して発射方向をワールド空間に変換
	VECTOR rotateShotDir = MyQuaternion::QTransformVector(m_GravityRotation, shotDir);
	// 発射方向から目標の回転角度を計算
	float angleTarget = atan2f(rotateShotDir.x, rotateShotDir.z);

	// ラジアン → 度
	float angleDeg = angleTarget * 180.0f / DX_PI_F;

	// プレイヤーの回転を更新
	UpdateRotate(angleDeg);

	// 射撃リキャスト時間が0以下の場合に発射する
	if (m_AbleReShotTime < 0)
	{
		SoundManager::GetInstance()->Play_SE(SE_GRAVITYORB_SHOT, false);
		m_Shot.Fire(shotPos, shotDir, PLAYER,PLAYER_SHOT_SPEED);
		m_AbleReShotTime = ABLERESHOT_TIME;
	}
}

void Player::HandleTargetInput()
{
	// ターゲットを取得
	Target* target = TargetManager::GetInstance()->GetTarget().get();
	// カメラマネージャーを取得
	CameraManager* cameraManager = CameraManager::GetInstance();

	if (target == nullptr) return;
	if (!target->IsEnableTarget())return;

	// ターゲット操作解除
	if ((Input::IsTriggerButton(BUTTON_Y) || Input::IsTriggerKey(KEY_F)))
	{
		target->ClearTarget(false);
	}

	// ターゲット開始
	if (Input::IsTriggerButton_R() || Input::IsTriggerKey(KEY_C))
	{
		if (!target->IsTargeting())
		{
			target->StartTargetEnemy();
		}
	}

	// 次のターゲットへ切り替え
	if (Input::IsTriggerButton_R() || Input::IsTriggerKey(KEY_V))
	{
		target->ChangeTargetEnemy(+1);
	}
}


void Player::HandleObjectGravityInput()
{
	// キーの入力を取得
	m_InputV = (float)((Input::IsInputKey(KEY_UP) - Input::IsInputKey(KEY_DOWN)));
	m_InputH = (float)((Input::IsInputKey(KEY_RIGHT) - Input::IsInputKey(KEY_LEFT)));
	// パッドの入力を取得
	m_inputLeftState = Input::GetInputLeftStick();
	m_inputRightState = Input::GetInputRightStick();

	// ジャンプ入力
	if (Input::IsTriggerKey(KEY_Z) || Input::IsTriggerButton(BUTTON_A))
	{
		m_InputJump = true;
		m_IsWalk = false;
	}

	GravityControlCommand command = {};
	// コントロールフラグをオンにする
	command.IsHolding = true;
	// カメラの状態を取得	
	CameraParame camParame = CameraManager::GetInstance()->GetCamera()->GetParame();

	// 敵を打ち出したら向きと力をリセットする
	if (command.IsShot)
	{
		m_ControllForceDir = {};
	}
	
	// 敵までの距離を計算
	float distance = MyMath::DistanceXZ(m_ControlEnemy->GetPos3D(), m_Transform.position_3D);

	// 敵を追従させる
	FollowEnemy(m_ControllForceDir, m_ControllForce);
	
	// 敵を打ち出す入力
	if ((Input::IsTriggerButton(BUTTON_X) || Input::IsTriggerKey(KEY_C)) && m_ShotMode == SHOT_TARGET)
	{
		// 打ち出しフラグをオンにする
		command.IsShot = true;
		command.IsHolding = false;
		m_Mode = MODE_NORMAL;
		m_AbleReShotTime = ABLERESHOT_TIME;
	
		// ターゲットを取得
		Target* target = TargetManager::GetInstance()->GetTarget().get();
		// ターゲットが存在する場合、ターゲットの現在の敵を取得して打ち出し方向を計算する
		if (target)
		{
			// ターゲットの現在の敵を取得
			Enemy* targetEnemy = target->GetCurrentTargetEnemy();

			// ターゲットしている敵が存在する場合、打ち出し方向を計算する
			if (targetEnemy)
			{
				// コントロールしている敵が存在する場合、ターゲットの敵に向かう方向を計算して力の方向にする
				if (m_ControlEnemy)
				{
					VECTOR toTarget = MyMath::VecSub(targetEnemy->GetPos3D(), m_ControlEnemy->GetPos3D());
					m_ControllForceDir = MyMath::VecNormalize(toTarget);
					m_ControllForce = 0.2f;
				}
			}
		}		
	}

	// 力の方向と大きさをコントロールしている敵に適用する
	command.ForceDir = m_ControllForceDir;
	command.ForcePower = m_ControllForce;

	// コントロールしている敵が存在する場合、重力操作コマンドを適用する
	if (m_ControlEnemy)
	{
		m_ControlEnemy->ApplyGravityCommand(command);
	}

}

void Player::FollowEnemy(VECTOR& dir, float& force)
{
	// 浮いている敵の目標位置
	VECTOR anchorPos = VGet(
		m_Transform.position_3D.x,
		m_Transform.position_3D.y + HOVER_HEIGHT,
		m_Transform.position_3D.z
	);

	// プレイヤーから敵へのベクトル
	VECTOR toEnemy = MyMath::VecCreate(anchorPos, m_ControlEnemy->GetPos3D());
	float currentDist = MyMath::VecLong(toEnemy);

	if (currentDist < 0.001f)return;

	// 敵までのベクトルの大きさ
	VECTOR toEnemyNorm = MyMath::VecScale(toEnemy, 1.0f / currentDist);

	float distError = currentDist - TARGET_DIST;
	VECTOR springForce = MyMath::VecScale(toEnemyNorm, -distError * SPRING_K);

	// カメラの前方と右方向を取得
	CameraParame camParame = CameraManager::GetInstance()->GetCamera()->GetParame();
	VECTOR camForward = camParame.forward;
	VECTOR camRight = camParame.right;
	camForward.y = 0.0f;
	camRight.y = 0.0f;
	camForward = MyMath::VecNormalize(camForward);
	camRight = MyMath::VecNormalize(camRight);

	// 入力ベクトルをワールド空間に変換
	float inputV = (float)(Input::IsInputKey(KEY_W) - Input::IsInputKey(KEY_S));
	float inputH = (float)(Input::IsInputKey(KEY_D) - Input::IsInputKey(KEY_A));
	if (m_inputLeftState.m_VerticalInput_L != 0.0f)  inputV = m_inputLeftState.m_VerticalInput_L;
	if (m_inputLeftState.m_HorizontalInput_L != 0.0f) inputH = m_inputLeftState.m_HorizontalInput_L;

	VECTOR inputWorld = MyMath::VecAdd(
		MyMath::VecScale(camForward, inputV),
		MyMath::VecScale(camRight, inputH)
	);

	// 入力ベクトルからプレイヤーから敵への成分を除去
	float dot = MyMath::VecDot(inputWorld, toEnemyNorm);
	VECTOR tangent = MyMath::VecSub(inputWorld, MyMath::VecScale(toEnemyNorm, dot));
	float tangentLen = MyMath::VecLong(tangent);

	VECTOR tangentForce = {};
	if (tangentLen > 0.001f)
	{
		tangentForce = MyMath::VecScale(tangent, TARGET_FORCE);
	}

	// 合成
	VECTOR totalForce = MyMath::VecAdd(springForce, tangentForce);
	float totalLen = MyMath::VecLong(totalForce);

	if (totalLen > 0.001f)
	{
		dir = MyMath::VecScale(totalForce, 1.0f / totalLen);
		force = min(totalLen, 0.5f);
	}
}

