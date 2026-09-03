#include "SceneManager.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "GameOverScene.h"
#include "LoadingScene.h"
#include "GameClear.h"
#include "PlaySelect.h"
#include "SceneBase.h"

SceneManager::SceneManager():
	m_CurrentScene(SCENE_TYPE_NONE),
	m_ChooseScene(__nullptr),
	m_IsCreatedLodingScene(false),
	m_NextScene(SCENE_TYPE_NONE)
{
}

SceneManager::~SceneManager()
{
	Fin();
}

void SceneManager::Init()
{
	// 最初のシーンを作成して初期化から開始
	CreateScene(TITLE);
	// 最初のシーンをタイトルシーンに設定
	m_CurrentScene = TITLE;
}

void SceneManager::Update()
{
	// 現在動いているシーンを更新する
	for (auto& nowScene : m_NowScene)
	{
		nowScene->Exec();
	}

	// 遷移するシーンがあるかどうか
	if (m_NextScene != SCENE_TYPE_NONE)
	{
		// 終了したシーンを削除する	
	    m_NowScene.clear();
	    // 次のシーンを生成する
		CreateScene(m_NextScene);
		// 現在のシーンを次のシーンに変更する
		m_NextScene = SCENE_TYPE_NONE;
	}
}

void SceneManager::Fin()
{

}

void SceneManager::ChangeScene(SceneType type)
{
	m_NextScene = type;
	m_CurrentScene = type;
}

SceneBase* SceneManager::CreateScene(SceneType type)
{
	// 引数で引き渡されたシーンを生成して管理変数に保存する
	std::unique_ptr<SceneBase> scene;

	// タイプに合わせたシーンを生成
	switch (type)
	{
	case TITLE:
		scene = MakeUnique<TitleScene>();
		break;

	case PLAY: 
		scene = MakeUnique<PlayScene>();
		break;

	case CLEAR:
		scene = MakeUnique<GameClearScene>();
		break;

	case GAMEOVER:
		scene = MakeUnique<GameOverScene>();
		break;

	case PLAYSELECT: 
		scene = MakeUnique<PlaySelectScene>();
		break;

	case LOADING:
		scene = MakeUnique<LoadingScene>();
		break;
	}

	// 生成したシーンの生ポインタを保存
	SceneBase* rawPtr = scene.get(); 
	// 生成したシーンを管理変数に保存
	m_NowScene.push_back(std::move(scene));

	return rawPtr;
}

SceneBase* SceneManager::StartLoadingScene()
{
	// ロードシーンが生成されていない場合は生成する
	if (!m_IsCreatedLodingScene)
	{
		// ロードシーンを生成して管理変数に保存
		m_ChooseScene = CreateScene(LOADING);
		// ロードシーン生成フラグを立てる
		m_IsCreatedLodingScene = true;
	}
		
	return m_ChooseScene;
}

void SceneManager::StopLoadingScene(SceneBase* loadingScene)
{
	// ロードシーン生成フラグを下げる
	m_IsCreatedLodingScene = false;

	// ロードシーンを管理変数から削除する
	for (auto& nowScene : m_NowScene)
	{
		if (nowScene.get() == loadingScene)
		{
			m_NowScene.remove(nowScene);

			return;
		}
	}
}
