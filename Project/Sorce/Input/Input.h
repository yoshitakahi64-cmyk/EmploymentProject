#pragma once

// 入力ボタン定義
enum InputKey
{
	KEY_UP		= (1 << 0),	
	KEY_DOWN	= (1 << 1),	
	KEY_LEFT	= (1 << 2),	
	KEY_RIGHT	= (1 << 3),	
	KEY_Z		= (1 << 4),	
	KEY_A		= (1 << 5),	
	KEY_W		= (1 << 6),	
	KEY_S		= (1 << 7),	
	KEY_D		= (1 << 8),
	KEY_1       = (1 << 9),
	KEY_X       = (1 << 10),
    KEY_R       = (1 << 11),
	KEY_F       = (1 << 12),
	KEY_SPACE   = (1 << 18),
	KEY_C       = (1 << 19),
	KEY_V       = (1 << 21),
	kEY_Q       = (1 << 23),
	KEY_E       = (1 << 25),
	KEY_LSHIFT = (1 << 26),
	KEY_RCONTROL = (1 << 27),
	KEY_ESCAPE = (1 << 29),
	KEY_M = (1 << 30)
};

enum InputButton
{
	BUTTON_UP = (1 << 13),
	BUTTON_DOWN = (1 << 14),
	BUTTON_LEFT = (1 << 15),
	BUTTON_RIGHT = (1 << 16),
	BUTTON_A = (1 << 17),
	BUTTON_X = (1 << 20),
	BUTTON_Y = (1 << 22),
	BUTTON_B = (1 << 24),
	BUTTON_START = (1 << 28),
};

// 左スティックの入力状態
struct LeftStick
{
	float m_HorizontalInput_L;
	float m_VerticalInput_L;
};

// 右スティックの入力状態
struct RightStick
{
	float m_HorizontalInput_R;
	float m_VerticalInput_R;
};

// 入力管理クラス
class Input
{
public:
	static void Init(); // 初期化関数
	static void Update(); // 更新関数
	static void Draw(); // 描画関数
	static void Fin(); // 終了関数

	static bool IsInputKey(InputKey key);		// キー入力判定
	static bool IsTriggerKey(InputKey key);	// キー押した瞬間判定

	static bool IsInputButton(InputButton button); // キー入力判定
	static bool IsTriggerButton(InputButton button); // キー押した瞬間判定
	static LeftStick GetInputLeftStick(); // 左スティックの入力状態を取得
	static RightStick GetInputRightStick(); // 右スティックの入力状態を取得
	static bool IsInputButton_L(); // 左トリガーボタンの入力判定
	static bool IsInputButton_R(); // 右トリガーボタンの入力判定
	static bool IsTriggerButton_L(); // 左トリガーボタンの押した瞬間判定
	static bool IsTriggerButton_R(); // 右トリガーボタンの押した瞬間判定
	static bool IsReleaseButton_L(); // 左トリガーボタンの離した瞬間判定
	static bool IsReleaseButton_R(); // 右トリガーボタンの離した瞬間判定

	static bool GetConectingPad(); // コントローラーが接続されているかどうかを取得
private:
	static void UpdateInputKeyState(); // キー入力状態の更新関数
	static void UpdateInputPadState(); // パッド入力状態の更新関数

private:
	// 入力ビットフラグ
	static int m_InputState;
	// 前回の入力ビット
	static int m_PrevInputState;
	// 左スティック
	static LeftStick m_InputLeftStickState;
	// 右スティック
	static RightStick m_InputRightStickState;
	// 左トリガーボタン
	static bool m_IsTriggerButton_L;
	// 右トリガーボタン
	static bool m_IsTriggerButton_R;
	// 前回の左トリガーボタン
	static bool m_IsPrevTriggerButton_L;
	// 前回の右トリガーボタン
	static bool m_IsPrevTriggerButton_R;
	// コントローラー接続状態
    static bool m_IsConnectingPad;
};
