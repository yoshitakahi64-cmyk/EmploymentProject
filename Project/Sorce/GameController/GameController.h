#pragma once

// ゲームコントローラークラス
class GameController
{
public:
	static void Init(); 	// 初期化
	static void Fin();     // 終了

public:
	// ゲッター・セッター関数
	static bool GetFinishGame() { return m_IsFinishGame; }
	static void SetFinishGame(bool isFinishGame) { m_IsFinishGame = isFinishGame; }

private:
	// ゲーム終了フラグ
	static bool m_IsFinishGame;
};
