#include <DxLib.h>
#include "GameClear.h"
#include "../Input/Input.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Font/FontManager.h"
#include "../Font/FontParam.h"
#include "../Resource/MyResourceManager.h"


GameClearScene::GameClearScene():
	m_Handle(0),
	m_TextActiveCounter(0),
	m_IsTextActive(false)
{
	m_State = INIT;
}

GameClearScene::~GameClearScene()
{
	Fin();
}

void GameClearScene::Init()
{
	// ステートをロードに設定
	m_State = LOAD;
	// テキスト点滅用の変数を初期化
	m_TextActiveCounter = 0;
	m_IsTextActive = true;
}

void GameClearScene::Load()
{
	// 画像をロードしてハンドルを取得
	m_Handle = LoadGraph("Data/Clear/GameClear.png");
	// ステートをスタートに設定
	m_State = START;
}

void GameClearScene::Start()
{
	// BGMを再生
	SoundManager::GetInstance()->Play_BGM(BGM_CLEAR, true);
	// ステートをループに設定
	m_State = LOOP;
}

void GameClearScene::Step()
{
	// Zキーでプレイシーンへ
	if (Input::IsTriggerKey(KEY_Z) || Input::IsTriggerButton(BUTTON_A))
	{
		SoundManager::GetInstance()->Stop_BGM(BGM_CLEAR);
		SceneManager::GetInstance()->ChangeScene(TITLE);
	}
}

void GameClearScene::Update()
{
	// テキスト点滅用のカウンターをインクリメント
	m_TextActiveCounter++;
	// カウンターが40以上になったらテキストの表示状態を切り替える
	if (m_TextActiveCounter >= 40)
	{
		m_TextActiveCounter = 0;

		if (!m_IsTextActive)
		{
			m_IsTextActive = true;
		}
		else
		{
			m_IsTextActive = false;
		}
	}
}

void GameClearScene::Draw()
{
	// 画像を描画
	DrawGraph(0, 0, m_Handle, true);

	// テキストが表示状態のときは、A or Zキーでタイトルへというテキストを描画
	if (m_IsTextActive)
	{
		DrawFormatStringToHandle(250, 500, GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_TITLESELECT), "A or Zキーでタイトルへ");
	}
}

void GameClearScene::Fin()
{
	// リソースマネージャーの終了処理を呼ぶ
	MyResourceManager::GetInstance()->Fin();
	// リソースマネージャーを削除
	MyResourceManager::DeleteInstance();
}
