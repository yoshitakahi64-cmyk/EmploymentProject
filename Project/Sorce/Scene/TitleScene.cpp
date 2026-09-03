#include <DxLib.h>
#include "TitleScene.h"
#include "../Scene/SceneManager.h"
#include "../Input/Input.h"
#include "../Sound/SoundManager.h"
#include "../Font/FontManager.h"
#include "../Font/FontParam.h"
#include "../GameController/GameController.h"
#include "../Resource/MyResourceManager.h"
#include "../Sound/SoundManager.h"


TitleScene::TitleScene():
	m_TextActiveCounter(0),
	m_IsTextActive(false),
	m_Handle(0)
{
	m_State = INIT;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	// タイトルシーンの状態をロードに設定
	m_State = LOAD;
	// タイトルテキストの点滅用のカウンターを初期化
	m_TextActiveCounter = 0;
	m_IsTextActive = true;

	// マイリソースマネージャーを作成
	MyResourceManager::CreateInstance();

	// マイリソースマネージャーを取得
	MyResourceManager* myResourceManager = MyResourceManager::GetInstance();
	myResourceManager->Init();


	// サウンドマネージャーを作成
	SoundManager::CreateInstance();
	// サウンドマネージャーを取得
	SoundManager* soundManager = SoundManager::GetInstance();
	// サウンドマネージャーの初期化とロード
	soundManager->Init();
	soundManager->Load();
}

void TitleScene::Load()
{
	// タイトル画像をロードしてハンドルを取得
	m_Handle = LoadGraph("Data/Title/TitleImage.png");
	// ステートをスタートに設定
	m_State = START;
}

void TitleScene::Start()
{
	// タイトルBGMを再生
	SoundManager::GetInstance()->Play_BGM(BGM_TITLE, true);
	// ステートをループに設定
	m_State = LOOP;
}

void TitleScene::Step()
{	
	// Zキーでプレイシーンへ
	if (Input::IsTriggerKey(KEY_Z) || Input::IsTriggerButton(BUTTON_A))
	{
		SoundManager::GetInstance()->Play_SE(SE_START_JINGLE, false);
		SoundManager::GetInstance()->Stop_BGM(BGM_TITLE);
		SceneManager::GetInstance()->ChangeScene(PLAY);
		// ステートを終了に設定
		m_State = FIN;
	}
}

void TitleScene::Update()
{
	// テキスト点滅用のカウンターをインクリメント
	m_TextActiveCounter++;

	// カウンターが40以上になったらテキストの表示非表示を切り替える
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

void TitleScene::Draw()
{
	// 画像を描画
	DrawGraph(0, 0, m_Handle, true);

	// タイトルテキストを点滅させて描画
	if (m_IsTextActive)
	{
		DrawFormatStringToHandle(100, 600, GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_TITLESELECT), "A or Zキーでスタート");
	}
}

void TitleScene::Fin()
{
}
