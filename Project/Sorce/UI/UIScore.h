#pragma once
#include "UIBase.h"
#include "../Score/Score.h"

// スコアUIクラス
class UIScore : public UIBase
{
public:
	UIScore(); // コンストラクタ 
	~UIScore();// デストラクタ

public:
	void Init()override; //	初期化関数
	void Load()override; // ロード関数
	void Start()override; // スタート関数
	void Update()override; // 更新関数
	void Draw()override; // 描画関数
	void Fin()override; // 終了関数

	// クローン関数
	UniquePtr<UIBase> Clone()override;

private:
	// スコアクラスのポインタ
	Score* m_Score;

	// スコアの値
	int m_Value;
	// スコアフレームのハンドル
	int m_ScoreFrameHandle;
};