#pragma once

// スコアクラス
class Score
{
public:
	Score(); // コンストラクタ
	~Score() = default; // デストラクタ

public:
	void Init(); // 初期化関数
	void Fin(); // 終了関数

public:
	// スコアを加算する関数
	void AddScore(int score) { m_Score += score; }
	// スコアをリセットする関数
	void ResetScore() { m_Score = 0; }
	// スコアを取得する関数
	int GetScore() const { return m_Score; }

private:
	// スコアの値
	int m_Score;
};