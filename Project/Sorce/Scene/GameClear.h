#pragma once
#include "SceneBase.h"

// ゲームクリアシーン
class GameClearScene : public SceneBase
{
public:
	GameClearScene(); // コンストラクタ
	~GameClearScene(); // デストラクタ

public:
	void Init() override; // 初期化関数
	void Load() override; // ロード関数
	void Start() override; // スタート関数
	void Step() override; // ステップ関数
	void Update() override; // 更新関数
	void Draw() override; // 描画関数
	void Fin() override; // 終了関数

private:
	// 画像ハンドル
	int m_Handle;

	// テキスト表示のカウンター
	int m_TextActiveCounter;
	// テキストが表示されているかどうかのフラグ
	bool m_IsTextActive;

};
