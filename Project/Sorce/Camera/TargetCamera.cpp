#include "TargetCamera.h"
#include "../Target/TargetManager.h"
#include "../Target/Target.h"
#include "../Enemy/Enemy.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"
#include "../TestStage/TestStage.h"
#include "../TestStage/TestStageManager.h"


static constexpr float ROTATION_SPEED = 1.0f;
static constexpr float TARGET_DISTANCE = 5.0f;
static constexpr float TARGET_HEIGHT = 15.0f;
static constexpr float MAX_TARGET_DISTANCE = 20.0f;
static constexpr float TRAGET_DISTANCE_RATE = 1.1f;
static constexpr float TARGET_LOOK_RATE = 0.8f;


TargetCamera::TargetCamera()
{
}

TargetCamera::~TargetCamera()
{
}

void TargetCamera::Enter(Camera& camera)
{
	CameraParame parame = camera.GetParame();

	// カメラの回転を設定
	parame.targetRotX = 10.0f;

	Target* target = TargetManager::GetInstance()->GetTarget().get();
	VECTOR playerPos = PlayerManager::GetInstance()->GetPlayer()->GetPos3D();

	// ターゲットが存在する場合はターゲットに向かうように注視点を設定
	if (target && target->GetCurrentTargetEnemy())
	{
		// ターゲットの位置を取得
		VECTOR targetPos = target->GetCurrentTargetEnemy()->GetPos3D();
		// プレイヤーとターゲットの中間地点を注視点に設定
		const float lookRate = TARGET_LOOK_RATE;
		parame.targetPos = MyMath::VecLerp(playerPos, targetPos, lookRate);

		// プレイヤーとターゲットの距離に応じてカメラの距離を調整
		VECTOR toEnemy = MyMath::VecSub(targetPos, playerPos);
		float enemyDist = MyMath::VecLong(toEnemy);
		parame.targetdistance = max(TARGET_DISTANCE, enemyDist * 0.85f);
		parame.targetHeight = TARGET_HEIGHT;
	}
	else
	{
		// ターゲットが存在しない場合はプレイヤーの後方を注視点に設定
		parame.targetdistance = TARGET_DISTANCE;
		parame.targetHeight = TARGET_HEIGHT;
	}

	// カメラの回転をプレイヤー→ターゲット方向に設定
	CalcRot(parame);
	camera.SetParame(parame);
}

void TargetCamera::Update(Camera& camera)
{
	CameraParame parame = camera.GetParame();
	Target* target = TargetManager::GetInstance()->GetTarget().get();

	// ターゲットが存在しない、またはターゲットに敵がいない場合はプレイヤーの後方を注視点に設定して終了
	if (!target || !target->GetCurrentTargetEnemy())
	{
		camera.SetParame(parame);
		return;
	}

	VECTOR playerPos = PlayerManager::GetInstance()->GetPlayer()->GetPos3D();
	VECTOR targetPos = target->GetCurrentTargetEnemy()->GetPos3D();

	// プレイヤーとターゲットの中間地点を注視点に設定
	const float lookRate = 0.6f;
	parame.targetPos = MyMath::VecLerp(playerPos, targetPos, lookRate);

	// プレイヤーとターゲットの距離に応じてカメラの距離を調整
	VECTOR toEnemy = MyMath::VecSub(targetPos, playerPos);
	float enemyDist = MyMath::VecLong(toEnemy);

		parame.targetdistance = MyMath::Clamp(
			max(TARGET_DISTANCE, enemyDist * TRAGET_DISTANCE_RATE),
			TARGET_DISTANCE,
			MAX_TARGET_DISTANCE);
	
	// カメラの回転を再計算
	CalcRot(parame);

	camera.SetParame(parame);
}

void TargetCamera::Exit(Camera& camera)
{
	
	CameraParame parame = camera.GetParame();

	Target* target = TargetManager::GetInstance()->GetTarget().get();
	VECTOR playerPos = PlayerManager::GetInstance()->GetPlayer()->GetPos3D();

	// ターゲットが存在する場合はターゲットに向かうように注視点を設定
	if (target && target->GetCurrentTargetEnemy())
	{
		// 敵の位置を取得
		VECTOR enemyPos = target->GetCurrentTargetEnemy()->GetPos3D();
		// プレイヤーから敵へのベクトルを計算
		VECTOR toEnemy = MyMath::VecSub(enemyPos, playerPos);
		// 水平方向のベクトルに変換
		toEnemy.y = 0.0f;
		// ベクトルの長さを計算
		float len = MyMath::VecLong(toEnemy);
		// 長さが十分に大きい場合のみ回転を計算
		if (len > 0.001f)
		{
			toEnemy = MyMath::VecScale(toEnemy, 1.0f / len);
			// プレイヤー→敵方向の逆 = カメラはプレイヤー後方
			float yaw = atan2f(toEnemy.x, toEnemy.z) * (180.0f / DX_PI_F);
			// 回転角を-180～180度の範囲に正規化
			if (yaw > 180.0f) yaw -= 360.0f;
			if (yaw < -180.0f) yaw += 360.0f;
			parame.targetRotY = yaw;
		}
	}

	camera.SetParame(parame);
}

void TargetCamera::CalcRot(CameraParame& camParam)
{
	// プレイヤーの位置を取得
	VECTOR playerPos = PlayerManager::GetInstance()->GetPlayer()->GetPos3D();
	// プレイヤーから注視点へのベクトルを計算
	VECTOR toEnemy = MyMath::VecSub(camParam.targetPos, playerPos);
	// 水平方向のベクトルに変換
	toEnemy.y = 0.0f;
	// ベクトルの長さを計算
	float len = MyMath::VecLong(toEnemy);
	// 長さが十分に大きい場合のみ回転を計算
	if (len < 0.001f)
		return;

	// プレイヤー→敵方向の単位ベクトルを計算
	toEnemy = MyMath::VecScale(toEnemy, 1.0f / len);
	// プレイヤー→敵方向の逆 = カメラはプレイヤー後方
	float targetYaw = atan2f(toEnemy.x, toEnemy.z) * (180.0f / DX_PI_F);
	// 回転角を-180～180度の範囲に正規化
	float behindYaw = targetYaw;
	if (behindYaw > 180.0f) behindYaw -= 360.0f;
	if (behindYaw < -180.0f) behindYaw += 360.0f;

	// 角度を設定
	camParam.targetRotY = behindYaw;
}

void TargetCamera::Input(CameraParame& parame)
{
	if (Input::IsInputKey(KEY_S) || Input::GetInputRightStick().m_VerticalInput_R < 0.0f)
	{
		parame.rot.x += ROTATION_SPEED;
		parame.targetRotX += ROTATION_SPEED;
	}
	if (Input::IsInputKey(KEY_W) || Input::GetInputRightStick().m_VerticalInput_R > 0.0f)
	{
		parame.rot.x -= ROTATION_SPEED;
		parame.targetRotX -= ROTATION_SPEED;
	}
}
