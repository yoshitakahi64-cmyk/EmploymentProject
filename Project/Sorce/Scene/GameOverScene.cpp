#include <DxLib.h>
#include "GameOverScene.h"
#include "../Input/Input.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Font/FontManager.h"
#include "../Font/FontParam.h"
#include "../Resource/MyResourceManager.h"

GameOverScene::GameOverScene():
	m_IsTextActive(false),
	m_TextActiveCounter(0),
	m_Handle(0)
{
	m_State = INIT;
}

GameOverScene::~GameOverScene()
{
	Fin();
}

void GameOverScene::Init()
{
	// ステートをロードに設定
	m_State = LOAD;
	// テキスト表示のカウンターを初期化
	m_TextActiveCounter = 0;
	m_IsTextActive = true;
}

void GameOverScene::Load()
{
	// 画像をロードしてハンドルを取得
	m_Handle = LoadGraph("Data/GameOver/GameOver.png");
	// ステートをスタートに設定
	m_State = START;
}

void GameOverScene::Start()
{
	// BGMを再生
	SoundManager::GetInstance()->Play_BGM(BGM_GAMEOVER, true);
	// ステートをループに設定
	m_State = LOOP;
}

void GameOverScene::Step()
{
	// Zキーでプレイシーンへ
	if (Input::IsTriggerKey(KEY_Z) || Input::IsTriggerButton(BUTTON_A))
	{
		SoundManager::GetInstance()->Stop_BGM(BGM_GAMEOVER);
		SceneManager::GetInstance()->ChangeScene(TITLE);
	}
}

void GameOverScene::Update()
{
	// テキスト点滅用のカウンターをインクリメント
	m_TextActiveCounter++;

	// カウンターが40以上になったらテキストの表示状態を切り替える
	if (m_TextActiveCounter >= 40)
	{
		// カウンターをリセット
		m_TextActiveCounter = 0;

		// テキストの表示状態を切り替える
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

void GameOverScene::Draw()
{
	// 画像を描画
	DrawGraph(0, 0, m_Handle, true);

	// テキストが表示状態のときは「A or Zキーでタイトルへ」のテキストを描画
	if (m_IsTextActive)
	{
		DrawFormatStringToHandle(300, 500, GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_TITLESELECT), "A or Zキーでタイトルへ");
	}
}

void GameOverScene::Fin()
{
	// リソースマネージャーの終了処理を呼ぶ
	MyResourceManager::GetInstance()->Fin();
	// リソースマネージャーを削除
	MyResourceManager::DeleteInstance();
}
