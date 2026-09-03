#include "DxLib.h"
#include "Camera/CameraManager.h"
#include "Collision/CollisionManager.h"
#include "MyEffekseer/EffekseerManager.h"
#include "Resource/MyResourceManager.h"
#include "Scene/SceneManager.h"
#include "Font/FontManager.h"
#include "Font/FontParam.h"
#include "Mechanism/IDManager.h"
#include "Input/Input.h"
#include "FPS/FPS.h"
#include "MyMath/MyMath.h"
#include "GameController/GameController.h"
#include "UI/UIManager.h"


#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define FONT_SIZE_SCORE 60
#define FONT_SIZE_MANUAL 40
#define FONT_SIZE_TITLESELECT 50

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// ウィンドウモードON
	ChangeWindowMode(TRUE);

	// 画面解像度の設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	// エフェクシアマネージャーを生成
	EffekseerManager::CreateInstence();

	// エフェクシアマネージャーを取得
	EffekseerManager* effekseerManager = EffekseerManager::GetInstance();

	// エフェクシアを使用するためのセットアップ
	effekseerManager->Setup();

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Effekseer 
	EffekseerManager::GetInstance()->Init();

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// シーンマネージャー生成
	SceneManager::CreateInstance();
	// シーン初期化
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Init();


	FontManager::CreateInstance();

	FontManager* fontManager = FontManager::GetInstance();
	fontManager->Load(FONT_ID_SCORE, FONT_SIZE_SCORE);
	fontManager->Load(FONT_ID_MANUAL, FONT_SIZE_MANUAL);
	fontManager->Load(FONT_ID_TITLESELECT, FONT_SIZE_TITLESELECT);

	// Mechanismを作成
	IDManager::CreateInstance();

	// 入力初期化
	Input::Init();

	// ゲームコントローラー初期化
	GameController::Init();

	// FPS初期化
	FPSSystem::Init();

	// ゲームのメインループ
	while (ProcessMessage() >= 0)
	{
		// 1ミリ秒待機すると処理が安定する
		Sleep(1);

		// FPS更新
		FPSSystem::Update();

		// 入力更新
		Input::Update();

		// 画面をクリア
		ClearDrawScreen();

		// シーンを更新
		sceneManager->Update();

		// エスケープキーで終了
		if (Input::IsTriggerKey(KEY_ESCAPE))
		{
			break;
		}
		
		// 1フレームが速すぎた場合の待機
		FPSSystem::WaitFPS();

		// 画面フリップ
		ScreenFlip();

	}

	// 入力終了
	Input::Fin();

	// シーンマネージャー削除
	SceneManager::DeleteInstance();

	// エフェクシアマネージャー削除
	EffekseerManager::DeleteInstance();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
