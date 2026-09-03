#pragma once

class FPSSystem
{
public:
	// 関数のプロトタイプ宣言
	static void Init();
	static void Update();
	static void Draw();

	// 1フレームが速すぎたときの待機関数
	static void WaitFPS();
	
	// デルタタイムを取得
	static float GetDeltaTime() { return m_DeltaTime; }

private:
	static int m_StartTime;      // 測定開始時刻
	static int m_PrevTime;       // 前回の時刻
	static float m_DeltaTime;    // フレームレートに依存しない時間スケール
	static int m_Count;          // カウンタ
	static float m_Fps;          // 現在のFPS
};
