#pragma once
#include "SceneBase.h"
#include "../Singleton.h"
#include "../Container.h"
#include "../Memory.h"



class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();	// コンストラクタ
	~SceneManager();	// デストラクタ

public:
	// シーンのリストを取得する関数
	const List<UniquePtr<SceneBase>>& GetNowScene() const { return m_NowScene; }
	// 現在のシーンを取得する関数
	const SceneType GetCurrentScene() const { return m_CurrentScene; }

public:
	void Init();	// 初期化
	void Update();	// 更新
	void Fin();		// 終了

public:
	void ChangeScene(SceneType type);
	SceneBase* CreateScene(SceneType type);

	SceneBase* StartLoadingScene(); // ロード画面を開始する関数
	void StopLoadingScene(SceneBase*); // ロード画面を終了する関数

private:
	// 現在動いているシーン
	List<UniquePtr<SceneBase>> m_NowScene;

	// 選択したシーンを保持する変数
	SceneBase* m_ChooseScene;

	// 次に遷移するシーン
	SceneType m_NextScene;

	// 現在のシーン
	SceneType m_CurrentScene;

	// ロードシーン生成フラグ
	bool m_IsCreatedLodingScene;
};
