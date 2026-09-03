#include "Camera.h"
#include "../MyMath/MyMath.h"
#include "../Quatrenion/Quatrenion.h"
#include "../TestStage/TestStageManager.h"
#include "../TestStage/TestStage.h"
#include "CameraManager.h"
#include "../Target/TargetManager.h"
#include "../Target/Target.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Target/TargetManager.h"
#include "../Target/Target.h"


static constexpr float CAMERA_NEAR_CLIP = 0.01f;
static constexpr float CAMERA_FAR_CLIP = 1000.0f;

static constexpr float MIN_PITCH_ANGLE = 20.0f;
static constexpr float MAX_PITCH_ANGLE = 50.0f;

static constexpr int SCREEN_WIDTH = 1600;
static constexpr int SCREEN_HEIGHT = 900;

static constexpr float SMOOTHSPEED = 4.0f;
static constexpr float SMOOTHSPEED_ROTY = 20.0f;

static constexpr float WALL_OFFSET = 0.1f;
static constexpr float MIN_HEIGHT_ABOVE_TARGET = 0.5f;

static constexpr float WALL_HIT_RAY_OFFSET_PLAYER = 0.2f;
static constexpr float WALL_HIT_RAY_OFFSET_ENEMY = 1.0f;

Camera::Camera():
	m_CurrentState(nullptr),
	m_Enemy(nullptr),
	m_HitEffectDir({}),
	m_HitEffectDuration(0),
	m_HitEffectTimer(0),
	m_HitShakePower(0),
	m_HitZoomPower(0)
{
	m_Parame = {};
}

Camera::~Camera()
{

}

void Camera::Init()
{
	// ニア、ファークリップ面設定
	SetCameraNearFar(CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);

	// カメラのベクトル初期化
	m_Parame.up = VGet(0, 1, 0);
	m_Parame.forward = VGet(0, 0, 1);
	m_Parame.right = VGet(1, 0, 0);
}

void Camera::Update()
{
	// 状態の更新
	if (m_CurrentState)
	{
		m_CurrentState->Update(*this);
	}

	// distanceとheightを目標値へ補間
	float t = 1.0f - expf(-SMOOTHSPEED * (1.0f / 60.0f));
	m_Parame.distance = MyMath::Lerp(m_Parame.distance, m_Parame.targetdistance, t);
	m_Parame.height = MyMath::Lerp(m_Parame.height, m_Parame.targetHeight, t);

	// 注視点を補間
	if (TargetManager::GetInstance()->GetTarget().get()->IsCompulsionTargeting())
	{
		m_Parame.target = m_Parame.targetPos;
	}
	else
	{
		m_Parame.target = MyMath::VecLerp(m_Parame.target, m_Parame.targetPos, t);

	}
	
	// 回転を補間
	m_Parame.rot.x = MyMath::LerpAngle(m_Parame.rot.x, m_Parame.targetRotX, t);

	if (CameraManager::GetInstance()->GetUseTargetCamera())
	{
		float t_rotY = 1.0f - expf(-SMOOTHSPEED_ROTY * (1.0f / 60.0f));
		m_Parame.rot.y = MyMath::LerpAngle(m_Parame.rot.y, m_Parame.targetRotY, t);
	}
	else
	{
		m_Parame.rot.y = m_Parame.targetRotY;
	}
	
	
	// カメラの位置と回転を再計算
	RecalculateTransform();
	// カメラの当たり判定を解決
	SoliveCollisions();

	// カメラの位置と注視点を設定
	SetCameraPositionAndTargetAndUpVec(
		m_Parame.pos,
		m_Parame.target,
		m_Parame.up
	);
}

void Camera::Draw()
{
}

void Camera::ChangeState(CameraState* newState)
{
	// 現在の状態を終了
	if (m_CurrentState)
	{
		m_CurrentState->Exit(*this);
	}
	// 新しい状態に遷移
	m_CurrentState = newState;

	// 新しい状態を開始
	if (m_CurrentState)
	{
		m_CurrentState->Enter(*this);
	}
}

// ビュー内判定関数
bool Camera::IsInView(const VECTOR& pos) const
{
	return CheckCameraViewClip(pos) == false;
}
 
// ビュー外判定関数
bool Camera::IsOutOfView(const VECTOR& pos) const
{
	return IsInView(pos);
}

// ワールド座標をスクリーン座標に変換する関数
VECTOR Camera::GetScreenPos(const VECTOR& worldPos) const
{
	return ConvWorldPosToScreenPos(worldPos);
}

// マージン付きのビュー内判定関数
bool Camera::IsInViewWithMargin(const VECTOR& pos, float margin) const
{
	VECTOR screenPos = GetScreenPos(pos);

	if (screenPos.z < 0.0f || screenPos.z > 1.0f)
		return false;

	return screenPos.x >= -margin && screenPos.x <= SCREEN_WIDTH + margin &&
		   screenPos.y >= -margin && screenPos.y <= SCREEN_HEIGHT + margin;
}

void Camera::AddHitEffect(float duration, float shakePower, float zoomPower)
{
	m_HitEffectTimer = duration;
	m_HitEffectDuration = duration;
	m_HitShakePower = shakePower;
	m_HitZoomPower = zoomPower;
	m_HitEffectDir = m_Parame.forward;
}

bool Camera::HitCkeckCameraToEnemyRay()
{
	// ステージのコリジョンを取得
	TestStage* testStage = TestStageManager::GetInstance()->GetTestStage();
	int collisionHandle = testStage->Gethandle();

	Target* target = TargetManager::GetInstance()->GetTarget().get();
	if (target && target->GetCurrentTargetEnemy())
	{
		// プレイヤーとターゲット中の敵の座標を取得
		VECTOR playerPos = PlayerManager::GetInstance()->GetPlayer().get()->GetPos3D();
		VECTOR enemyPos = target->GetCurrentTargetEnemy()->GetPos3D();

		// レイの始点と終点を設定
		VECTOR rayStart = VGet(playerPos.x, playerPos.y + WALL_HIT_RAY_OFFSET_PLAYER, playerPos.z);
		VECTOR rayEnd = VGet(enemyPos.x, enemyPos.y + WALL_HIT_RAY_OFFSET_ENEMY, enemyPos.z);

		// 壁とのレイで当たり判定
		MV1_COLL_RESULT_POLY wallCheck = MV1CollCheck_Line(
			collisionHandle, -1, rayStart, rayEnd
		);

		// 壁と当たったかを返す
		return wallCheck.HitFlag;
	}

	return false;
}

// カメラの位置と注視点を再計算する関数
void Camera::RecalculateTransform()
{
	VECTOR target = m_Parame.target;

	// ピッチ角を制限
	m_Parame.rot.x = MyMath::Clamp(
		m_Parame.rot.x,
		MIN_PITCH_ANGLE,
		MAX_PITCH_ANGLE
	);
	
	// 回転角を取得
	float yaw = m_Parame.rot.y;
	float pitch = m_Parame.rot.x;

	// ワールドの上ベクトル
	VECTOR worldUp = VGet(0, 1, 0);

	// ヨー
	Quaternion qYaw = MyQuaternion::QfromAxisAngle(worldUp, yaw);
	VECTOR baseForward = VGet(0, 0, 1);
	VECTOR yawForward = MyQuaternion::QTransformVector(qYaw, baseForward);

	// 右向きベクトル
	VECTOR right = MyMath::VecNormalize(
		MyMath::VecCross3D(yawForward, worldUp)
	);

	// ピッチ
	Quaternion qPitch = MyQuaternion::QfromAxisAngle(right, pitch);
	Quaternion camRot = MyQuaternion::QMultiply(qPitch, qYaw);

	// カメラのローカルオフセットを計算
	VECTOR localOffset;

	/// カメラが注視点の方向を使用するかどうかでオフセットの計算方法を切り替える
	if (m_Parame.isUseLookDir)
	{
		// 注視点の方向を使用する場合、lookDirを基にオフセットを計算
		localOffset =
			MyMath::VecAdd(
				MyMath::VecScale(m_Parame.lookDir, -m_Parame.distance),
				MyMath::VecScale(worldUp, m_Parame.height)
			);
	}
	else
	{
		// 注視点の方向を使用しない場合、回転から前方ベクトルを計算してオフセットを計算
		localOffset =
			MyMath::VecAdd(
				MyMath::VecScale(baseForward, -m_Parame.distance),
				MyMath::VecScale(worldUp, m_Parame.height)
			);
	}

	// ローカルオフセットをワールド空間に変換
	VECTOR worldOffset = MyQuaternion::QTransformVector(camRot, localOffset);
	m_Parame.pos = MyMath::VecAdd(target, worldOffset);

	// 注視点を使用するかどうかで前方ベクトルの計算方法を切り替える
	if (m_Parame.isUseLookDir)
	{
		// 注視点の方向を使用する場合、lookDirを前方ベクトルとして使用
		m_Parame.forward = m_Parame.lookDir;
	}
	else
	{
		// 注視点の方向を使用しない場合、カメラ位置と注視点から前方ベクトルを計算
		m_Parame.forward = MyMath::VecNormalize(
			MyMath::VecCreate(m_Parame.pos, target)
		);
	}

	m_Parame.right = MyMath::VecNormalize(
		MyMath::VecCross3D(worldUp, m_Parame.forward)
	);

	// ヒットエフェクトの適用
	if (m_HitEffectTimer > 0.0f)
	{
		// ヒットエフェクトの進行度を更新
		float progress = 1.0f - (m_HitEffectTimer / m_HitEffectDuration);
		// ヒットエフェクトの減衰度を設定
		float decay = powf(1.0f - progress, 2.0f);

		// カメラのズーム量を計算
		m_Parame.pos = MyMath::VecAdd(m_Parame.pos, MyMath::VecScale(m_HitEffectDir, m_HitZoomPower * decay));

		// 左右にランダムに揺れる値を計算
		float shakeAmount = m_HitShakePower * decay;
		float shakeRand = ((rand() % 200) / 100.0f - 1.0f);
		VECTOR shakeOffset = MyMath::VecAdd(
			MyMath::VecScale(m_Parame.right, shakeRand * shakeAmount),
			VGet(0.0f, ((rand() % 200) / 100.0f - 1.0f) * shakeAmount * 0.5f, 0.0f)
		);

		// カメラの位置に適用
		m_Parame.pos = MyMath::VecAdd(m_Parame.pos, shakeOffset);

		// ヒットエフェクトの発生時間を進める
		m_HitEffectTimer -= 1.0f / 60.0f;
	}
}

// カメラの当たり判定を解決する関数
void Camera::SoliveCollisions()
{
	// カメラからのレイの始点と終点を計算
	VECTOR start = m_Parame.target;
	VECTOR end = m_Parame.pos;
	
	// レイの方向と距離を計算
	VECTOR dir = MyMath::VecSub(end, start);
	dir = MyMath::VecNormalize(dir);

	TestStage* testStage = TestStageManager::GetInstance()->GetTestStage();
	int collisionHandle = testStage->Gethandle();

	// ターゲット中にカメラとターゲットする敵の間に障害物があれば強制解除
	if (HitCkeckCameraToEnemyRay())
	{
		TargetManager::GetInstance()->GetTarget().get()->ClearTarget(true);
		return;
	}

	// レイとステージの当たり判定をチェック
	MV1_COLL_RESULT_POLY hitResult = MV1CollCheck_Line(collisionHandle, -1, start, end);
	if (hitResult.HitFlag)
	{
		// ヒット位置から注視点方向に少し戻す
		VECTOR safePos = MyMath::VecAdd(
			hitResult.HitPosition,
			MyMath::VecScale(dir, -WALL_OFFSET)
		);

		// 最低高さを保証
		if (safePos.y < m_Parame.target.y + MIN_HEIGHT_ABOVE_TARGET)
		{
			safePos.y = m_Parame.target.y + MIN_HEIGHT_ABOVE_TARGET;
		}

		m_Parame.pos = safePos;
	}
}
