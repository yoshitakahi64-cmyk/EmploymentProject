#pragma once
#include "SceneBase.h"

// プレイシーン
class PlayScene : public SceneBase
{
public:
	PlayScene(); // コンストラクタ
	~PlayScene(); // デストラクタ

public:
	void Init() override; // 初期化関数
	void Load() override; // ロード関数
	void Start() override; // スタート関数
	void Step() override; // ステップ関数
	void Update() override; // 更新関数
	void Draw() override; // 描画関数
	void Fin() override; // 終了関数

private:
	// デバッグモードフラグ
	bool m_IsDebug;
};
