#include "UIManual.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"
#include "../Font/FontManager.h"
#include "../Font/FontParam.h"
#include "../Input/Input.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"

constexpr int MOVE_MANUAL_CHAR_OFFSET_X = 150;
constexpr int MOVE_MANUAL_CHAR_OFFSET_Y = 70;

constexpr int JUMP_KEY_MANUAL_OFFSET_Y = 270;
constexpr int JUMP_KEY_CHAR_OFFSET_X = 150;
constexpr int JUMP_KEY_CHAR_OFFSET_Y = 300;

constexpr int MANUAL_OFFSET_Y_1 = 140;
constexpr int CHAR_OFFSET_X_1 = 150;
constexpr int CHAR_OFFSET_Y_1 = 180;

constexpr int _MANUAL_OFFSET_Y_2 = 400;
constexpr int CHAR_OFFSET_X_2 = 150;
constexpr int CHAR_OFFSET_Y_2 = 430;

constexpr int MANUAL_OFFSET_Y_3 = 520;
constexpr int CHAR_OFFSET_X_3 = 150;
constexpr int CHAR_OFFSET_Y_3 = 550;

constexpr int MANUAL_OFFSET_Y_4 = 650;
constexpr int CHAR_OFFSET_X_4 = 150;
constexpr int CHAR_OFFSET_Y_4 = 680;

UIManual::UIManual():
	m_Handle_ArrowKey(0),
	m_Handle_SpaceKey(0),
	m_Handle_QKey(0),
	m_Handle_EKey(0),
	m_Handle_FKey(0),
	m_Handle_VKey(0),
	m_Handle_CKey(0),
	m_Handle_ZKey(0),
	m_Handle_XKey(0),
	m_Handle_AButton(0),
	m_Handle_BButton(0),
	m_Handle_XButton(0),
	m_Handle_YButton(0),
	m_Handle_R2Button(0),
	m_Handle_L2Button(0),
	m_Handle_StickButton_R(0),
	m_Handle_StickButton_L(0),
	m_Player(nullptr)
{
}

UIManual::~UIManual()
{
}

void UIManual::Init()
{
	// 表示フラグをオンにする
	m_Hide = true;
}

void UIManual::Load()
{
	// 各画像をロードしてハンドルを取得 ---
	SharedPtr handle = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/WASD.png", false, true);
	if (handle)
	{
		m_Handle = handle->GetHandle();
	}

	SharedPtr handle2 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/ArrowKey_new.png", false, true);
	if (handle2)
	{
		m_Handle_ArrowKey = handle2->GetHandle();
	}

	SharedPtr handle3 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_space.png", false, true);
	if (handle3)
	{
		m_Handle_SpaceKey = handle3->GetHandle();
	}

	SharedPtr handle4 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_q.png", false, true);
	if (handle4)
	{
		m_Handle_QKey = handle4->GetHandle();
	}

	SharedPtr handle5 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_e.png", false, true);
	if (handle5)
	{
		m_Handle_EKey = handle5->GetHandle();
	}

	SharedPtr handle6 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_f.png", false, true);
	if (handle6)
	{
		m_Handle_FKey = handle6->GetHandle();
	}
	
	SharedPtr handle7 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_button_color_a.png", false, true);
	if (handle7)
	{
		m_Handle_AButton = handle7->GetHandle();
	}
	SharedPtr handle8 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_button_color_y.png", false, true);
	if (handle8)
	{
		m_Handle_YButton = handle8->GetHandle();
	}
	SharedPtr handle9 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_rt.png", false, true);
	if (handle9)
	{
		m_Handle_R2Button = handle9->GetHandle();
	}

	SharedPtr handle10 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_lt.png", false, true);
	if (handle10)
	{
		m_Handle_L2Button = handle10->GetHandle();
	}

	SharedPtr handle11 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_stick_r.png", false, true);
	if (handle11)
	{
		m_Handle_StickButton_R = handle11->GetHandle();
	}
	SharedPtr handle12 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_stick_l.png", false, true);
	if (handle12)
	{
		m_Handle_StickButton_L = handle12->GetHandle();
	}
	SharedPtr handle13 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_v.png", false, true);
	if (handle13)
	{
		m_Handle_VKey = handle13->GetHandle();
	}
	SharedPtr handle14 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_x.png", false, true);
	if (handle14)
	{
		m_Handle_XKey = handle14->GetHandle();
	}
	SharedPtr handle15 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_c.png", false, true);
	if (handle15)
	{
		m_Handle_CKey = handle15->GetHandle();
	}
	SharedPtr handle16 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/keyboard_z.png", false, true);
	if (handle16)
	{
		m_Handle_ZKey = handle16->GetHandle();
	}
	SharedPtr handle17 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/xbox_button_color_x.png", false, true);
	if (handle17)
	{
		m_Handle_XButton = handle17->GetHandle();
	}
	// -------------------------------------
}

void UIManual::Start()
{
	// プレイヤーのインスタンスを取得
	m_Player = PlayerManager::GetInstance()->GetPlayer().get();
}

void UIManual::Step()
{
}

void UIManual::Update()
{
	// STARTボタンかCキーが押されたら表示フラグを切り替える
	if (Input::IsTriggerKey(KEY_M) || Input::IsTriggerButton(BUTTON_START))
	{
		if(!m_Hide)
		{
			Hide();
		}
		else
		{
			Show();
		}
	}
	
}

void UIManual::Draw()
{

	// 非表示フラグがオンのときは操作説明を非表示にする
	if (m_Hide)
	{
		DrawFormatStringToHandle(0, 850,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "操作説明　START or Mキーで表示");
		return;
	}
	else
	{
		DrawFormatStringToHandle(0, 850,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "操作説明　START or Mキーで非表示");
	}	

	// コントローラーが接続されている場合はコントローラーの操作説明を表示し、そうでない場合はキーボードの操作説明を表示する
	if (Input::GetConectingPad())
	{
		// 移動キー
		DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D, m_Handle_StickButton_L, true);
		// ジャンプキー
		DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + JUMP_KEY_MANUAL_OFFSET_Y, m_Handle_AButton, true);

		if (m_Player->GetMode() == MODE_NORMAL)
		{	
			// 視点移動キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_1, m_Handle_StickButton_R, true);
			// エイムキー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + _MANUAL_OFFSET_Y_2, m_Handle_L2Button, true);
			// 発射キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_3, m_Handle_R2Button, true);
		}
		else if (m_Player->GetMode() == MODE_CONTROLOBJECT && !m_Player->GetShotMode() == SHOT_TARGET)
		{
			// 視点移動キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_1, m_Handle_StickButton_R, true);
			// ターゲットキー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + _MANUAL_OFFSET_Y_2, m_Handle_R2Button, true);
		}	
		else if (m_Player->GetShotMode() == SHOT_TARGET)
		{
			// 敵発射キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_1, m_Handle_XButton, true);
			// ターゲット（次）キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + _MANUAL_OFFSET_Y_2, m_Handle_R2Button, true);
			// ターゲット解除キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_3, m_Handle_YButton, true);
		}
	}
	else
	{
		// WASD
		DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D, m_Handle_ArrowKey, true);
		// ジャンプキー
		DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + JUMP_KEY_MANUAL_OFFSET_Y, m_Handle_ZKey, true);

		if (m_Player->GetMode() == MODE_NORMAL)
		{
			// 矢印キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_1, m_Handle, true);
			// エイムキー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + _MANUAL_OFFSET_Y_2, m_Handle_SpaceKey, true);
			// 発射キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_3, m_Handle_CKey, true);
		}
		else if (m_Player->GetMode() == MODE_CONTROLOBJECT && !m_Player->GetShotMode() == SHOT_TARGET)
		{
			// 矢印キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_1, m_Handle, true);
			// ターゲット開始キー	
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + _MANUAL_OFFSET_Y_2, m_Handle_CKey, true);
		}
		else if (m_Player->GetShotMode() == SHOT_TARGET)
		{
			// 敵発射キー	
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_1, m_Handle_CKey, true);
			// ターゲット切り替え(次)キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + _MANUAL_OFFSET_Y_2, m_Handle_VKey, true);
			// ターゲット解除キー
			DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D + MANUAL_OFFSET_Y_3, m_Handle_FKey, true);
		}
	}

	DrawFormatStringToHandle(m_Transform.posX_2D + MOVE_MANUAL_CHAR_OFFSET_X, m_Transform.posY_2D + MOVE_MANUAL_CHAR_OFFSET_Y,
		GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…移動");
	DrawFormatStringToHandle(m_Transform.posX_2D + JUMP_KEY_CHAR_OFFSET_X, m_Transform.posY_2D + JUMP_KEY_CHAR_OFFSET_Y,
		GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…ジャンプ");

	if (m_Player->GetMode() == MODE_NORMAL)
	{		
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_1, m_Transform.posY_2D + CHAR_OFFSET_Y_1,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…視点移動");
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_2, m_Transform.posY_2D + CHAR_OFFSET_Y_2,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…照準");
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_3, m_Transform.posY_2D + CHAR_OFFSET_Y_3,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…重力球発射");
	}
	else if (m_Player->GetMode() == MODE_CONTROLOBJECT && !m_Player->GetShotMode() == SHOT_TARGET)
	{
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_1, m_Transform.posY_2D + CHAR_OFFSET_Y_1,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…視点移動");
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_2, m_Transform.posY_2D + CHAR_OFFSET_Y_2,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…ターゲット");
	}
	else if (m_Player->GetShotMode() == SHOT_TARGET)
	{
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_1, m_Transform.posY_2D + CHAR_OFFSET_Y_1,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…敵発射");
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_2, m_Transform.posY_2D + CHAR_OFFSET_Y_2,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…ターゲット(次)");
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_3, m_Transform.posY_2D + CHAR_OFFSET_Y_3,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…ターゲット(前)");
		DrawFormatStringToHandle(m_Transform.posX_2D + CHAR_OFFSET_X_4, m_Transform.posY_2D + CHAR_OFFSET_Y_4,
			GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_MANUAL), "…ターゲット解除");
	}
	
}

void UIManual::Fin()
{
}

UniquePtr<UIBase> UIManual::Clone()
{
	UniquePtr<UIManual> clone = MakeUnique<UIManual>();
	*clone = *this;

	return std::move(clone);
}
