#include "NormalCamera.h"
#include "Camera.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"
#include "../Input/Input.h"

static constexpr float ROTATION_SPEED = 2.0f;
static constexpr float CAMERA_UP_OFFSET = 9.0f;

static constexpr float PLAYER_DISTANCE = -7.0f;
static constexpr float PLAYER_HEAD_OFFSET = 1.5f;

static constexpr float CAMERA_DEFORT_ROT_X = 30.0f;

NormalCamera::NormalCamera()
{

}

NormalCamera::~NormalCamera()
{

}

void NormalCamera::Enter(Camera& camera)
{
	// 初期注視点をプレイヤーの頭位置に設定
	CameraParame parame = camera.GetParame();
	VECTOR target = MyMath::VecAdd(
		PlayerManager::GetInstance()->GetPlayer()->GetPos3D(),
		MyMath::VecScale(parame.up, PLAYER_HEAD_OFFSET)
	);
	// 注視点の目標値を設定
	parame.targetPos = target;

	// カメラの距離と高さの目標値を設定
	parame.targetdistance = -PLAYER_DISTANCE;
	parame.targetHeight = CAMERA_UP_OFFSET;

	// カメラの回転の目標値を設定
	parame.targetRotX = CAMERA_DEFORT_ROT_X;
	// 注視点の方向を使用しない設定
	parame.isUseLookDir = false;

	// カメラのパラメータをカメラに設定
	camera.SetParame(parame);
}

void NormalCamera::Update(Camera& camera)
{
	CameraParame parame = camera.GetParame();

	// 注視点をプレイヤーの頭位置に設定
	VECTOR target = MyMath::VecAdd(PlayerManager::GetInstance()->GetPlayer()->GetPos3D(), MyMath::VecScale(parame.up, PLAYER_HEAD_OFFSET));
	parame.targetPos = target;

	Input(parame);

	camera.SetParame(parame);
}

void NormalCamera::Exit(Camera& camera)
{

}

void NormalCamera::Input(CameraParame& parame)
{

	// キー入力処理
	if (Input::IsInputKey(KEY_A) || Input::GetInputRightStick().m_HorizontalInput_R < 0.0f)
	{
		parame.rot.y -= ROTATION_SPEED;
		parame.targetRotY -= ROTATION_SPEED;
	}
	if (Input::IsInputKey(KEY_D) || Input::GetInputRightStick().m_HorizontalInput_R > 0.0f)
	{
		parame.rot.y += ROTATION_SPEED;
		parame.targetRotY += ROTATION_SPEED;
	}
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
