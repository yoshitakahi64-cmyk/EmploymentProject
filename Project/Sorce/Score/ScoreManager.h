#pragma once

#include "../Singleton.h"
#include "../Memory.h"

class Score;

// スコア管理クラス
class ScoreManager : public Singleton<ScoreManager>
{
public:
	ScoreManager(); // コンストラクタ
	~ScoreManager() = default; // デストラクタ

public:
	void Init(); // 初期化関数
	void Fin(); // 終了関数

public:
	// スコア生成関数
	void CreateScore();
	// スコア取得関数
	const UniquePtr<Score>& GetScore() const { return  m_Score; }

private:
	// スコアのインスタンス
	UniquePtr<Score> m_Score;

};