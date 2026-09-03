#include "DxLib.h"
#include "Input.h"

static constexpr float STICK_STATE_NORMALIZE = 32767.0f;
static constexpr float STICK_DEADZORN = 2000.0f;

int Input::m_InputState = 0;
int Input::m_PrevInputState = 0;
LeftStick Input::m_InputLeftStickState = {};
RightStick Input::m_InputRightStickState = {};
bool Input::m_IsTriggerButton_L = false;
bool Input::m_IsTriggerButton_R = false;
bool Input::m_IsPrevTriggerButton_L = false;
bool Input::m_IsPrevTriggerButton_R = false;
bool Input::m_IsConnectingPad = false;

void Input::Init()
{
	m_InputState = 0;
	m_PrevInputState = 0;
	m_InputLeftStickState = {};
	m_InputRightStickState = {};
	m_IsTriggerButton_L = false;
	m_IsTriggerButton_R = false;
	m_IsPrevTriggerButton_L = false;
	m_IsPrevTriggerButton_R = false;
}

void Input::Update()
{
	// 前回の入力を覚えておく
	m_PrevInputState = m_InputState;

	// 入力状態をクリア
	m_InputState = 0;

	// パッドの接続状態を確認
	int padCount = GetJoypadNum();

	if (padCount > 0)
	{
		// ボタン入力をアップデート
		UpdateInputPadState();

		m_IsConnectingPad = true;
	}
	else
	{
		// キー入力をアップデート
		UpdateInputKeyState();

		m_IsConnectingPad = false;
	}
}

void Input::Draw()
{
}

void Input::Fin()
{
}

bool Input::IsInputKey(InputKey key)
{
	// 入力状態をビットフラグで判定
	return m_InputState & key;
}

bool Input::IsTriggerKey(InputKey key)
{
	// 入力状態をビットフラグで判定
	return (m_InputState & key) && !(m_PrevInputState & key);
}

bool Input::IsInputButton(InputButton button)
{
	// 入力状態をビットフラグで判定
	return m_InputState & button;
}

bool Input::IsTriggerButton(InputButton button)
{
	// 入力状態をビットフラグで判定
	return  (m_InputState & button) && !(m_PrevInputState & button);
}

LeftStick Input::GetInputLeftStick()
{
	// 入力値を格納する構造体
	LeftStick result = {};

	// それぞれの入力値を正規化
	m_InputLeftStickState.m_VerticalInput_L = m_InputLeftStickState.m_VerticalInput_L / STICK_STATE_NORMALIZE;
	m_InputLeftStickState.m_HorizontalInput_L = m_InputLeftStickState.m_HorizontalInput_L / STICK_STATE_NORMALIZE;

	// 正規化した値を構造体に格納
	result = m_InputLeftStickState;

	return result;
}

RightStick Input::GetInputRightStick()
{
	// 入力値を格納する構造体
	RightStick result = {};

	// それぞれの入力値を正規化
	m_InputRightStickState.m_VerticalInput_R = m_InputRightStickState.m_VerticalInput_R / STICK_STATE_NORMALIZE;
	m_InputRightStickState.m_HorizontalInput_R = m_InputRightStickState.m_HorizontalInput_R / STICK_STATE_NORMALIZE;

	// 正規化した値を構造体に格納
    result = m_InputRightStickState;

	return result;
}

bool Input::IsInputButton_L()
{
	// 左トリガーボタンの入力状態を返す
	bool result;
	result = m_IsTriggerButton_L;

	return result;
}

bool Input::IsInputButton_R()
{
	// 右トリガーボタンの入力状態を返す
	bool result;
	result = m_IsTriggerButton_R;

	return result;
}

bool Input::IsTriggerButton_L()
{
	// 左トリガーボタンの押した瞬間の状態を返す
	return (!m_IsPrevTriggerButton_L && m_IsTriggerButton_L);
}

bool Input::IsTriggerButton_R()
{
	// 右トリガーボタンの押した瞬間の状態を返す
	return (!m_IsPrevTriggerButton_R && m_IsTriggerButton_R);
}

bool Input::IsReleaseButton_L()
{
	// 左トリガーボタンの離した瞬間の状態を返す
	return (m_IsPrevTriggerButton_L && !m_IsTriggerButton_L);
}

bool Input::IsReleaseButton_R()
{
	// 右トリガーボタンの離した瞬間の状態を返す
	return (m_IsPrevTriggerButton_R && !m_IsTriggerButton_R);
}

bool Input::GetConectingPad()
{
	// コントローラーが接続されているかどうかを返す
	return m_IsConnectingPad;
}

void Input::UpdateInputKeyState()
{
	// 入力状態をビットフラグで設定
	if (CheckHitKey(KEY_INPUT_UP))
	{
		m_InputState |= KEY_UP;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		m_InputState |= KEY_DOWN;
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		m_InputState |= KEY_LEFT;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		m_InputState |= KEY_RIGHT;
	}
	if (CheckHitKey(KEY_INPUT_Z))
	{
		m_InputState |= KEY_Z;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		m_InputState |= KEY_A;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		m_InputState |= KEY_W;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		m_InputState |= KEY_S;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		m_InputState |= KEY_D;
	}
	if (CheckHitKey(KEY_INPUT_X))
	{
		m_InputState |= KEY_X;
	}
	if (CheckHitKey(KEY_INPUT_R))
	{
		m_InputState |= KEY_R;
	}
	if (CheckHitKey(KEY_INPUT_F))
	{
		m_InputState |= KEY_F;
	}
	if (CheckHitKey(KEY_INPUT_1))
	{
		m_InputState |= KEY_1;
	}
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		m_InputState |= KEY_SPACE;
	}
	if (CheckHitKey(KEY_INPUT_C))
	{
		m_InputState |= KEY_C;
	}
	if (CheckHitKey(KEY_INPUT_V))
	{
		m_InputState |= KEY_V;
	}
	if (CheckHitKey(KEY_INPUT_Q))
	{
		m_InputState |= kEY_Q;
	}
	if (CheckHitKey(KEY_INPUT_E))
	{
		m_InputState |= KEY_E;
	}
	if (CheckHitKey(KEY_INPUT_LSHIFT))
	{
		m_InputState |= KEY_LSHIFT;
	}
	if (CheckHitKey(KEY_INPUT_RCONTROL))
	{
		m_InputState |= KEY_RCONTROL;
	}
	if(CheckHitKey(KEY_INPUT_ESCAPE))
	{
		m_InputState |= KEY_ESCAPE;
	}
	if (CheckHitKey(KEY_INPUT_M))
	{
		m_InputState |= KEY_M;
	}
}

void Input::UpdateInputPadState()
{
	XINPUT_STATE StickInput;

	// 前回のトリガーボタンの状態を覚えておく
	m_IsPrevTriggerButton_L = m_IsTriggerButton_L;
	m_IsPrevTriggerButton_R = m_IsTriggerButton_R;

	GetJoypadXInputState(DX_INPUT_PAD1, &StickInput);

	int padInput = GetJoypadInputState(DX_INPUT_PAD1);

	if (padInput & PAD_INPUT_UP)
	{
		m_InputState |= BUTTON_UP;
	}
	if (padInput & PAD_INPUT_DOWN)
	{
		m_InputState |= BUTTON_DOWN;
	}
	if (padInput & PAD_INPUT_LEFT)
	{
		m_InputState |= BUTTON_LEFT;
	}
	if (padInput & PAD_INPUT_RIGHT)
	{
		m_InputState |= BUTTON_RIGHT;
	}
	if (padInput & PAD_INPUT_A)
	{
		m_InputState |= BUTTON_A;
	}
	if (padInput & PAD_INPUT_C)
	{
		m_InputState |= BUTTON_X;
	}
	if (padInput & PAD_INPUT_X)
	{
		m_InputState |= BUTTON_Y;
	}
	if (padInput & PAD_INPUT_B)
	{
		m_InputState |= BUTTON_B;
	}
	if(padInput & PAD_INPUT_R)
	{
		m_InputState |= BUTTON_START;
	}
	

	// 左スティックの入力を取得
	if (StickInput.ThumbLY <= - STICK_DEADZORN || StickInput.ThumbLY >= STICK_DEADZORN)	
	{
		m_InputLeftStickState.m_VerticalInput_L = (float)(StickInput.ThumbLY);
	}

	if (StickInput.ThumbLX <= -STICK_DEADZORN || StickInput.ThumbLX >= STICK_DEADZORN)
	{
		m_InputLeftStickState.m_HorizontalInput_L = (float)(StickInput.ThumbLX);
	}


	//　右スティックの入力を取得
	if (StickInput.ThumbRY <= -STICK_DEADZORN || StickInput.ThumbRY >= STICK_DEADZORN)
	{
		m_InputRightStickState.m_VerticalInput_R = (float)(StickInput.ThumbRY);
	}

	if (StickInput.ThumbRX <= -STICK_DEADZORN || StickInput.ThumbRX >= STICK_DEADZORN)
	{
		m_InputRightStickState.m_HorizontalInput_R = (float)(StickInput.ThumbRX);
	}

	if (StickInput.LeftTrigger >= 1)
	{
		m_IsTriggerButton_L = true;
	}
	else
	{
		m_IsTriggerButton_L = false;
	}

	if (StickInput.RightTrigger >= 1)
	{
		m_IsTriggerButton_R = true;
	}
	else
	{
		m_IsTriggerButton_R = false;
	}
}
