#include "DxLib.h"
#include "FPS.h"

// 平均を計算するタイミング（大体はFPSと同じ数でOK）
#define FPS_SAMPLE_NUM (60) // 60フレームに一度平均を計算する

// ゲームのFPS
#define FPS (60)

int FPSSystem::m_PrevTime = 0;      // 測定開始時刻
int FPSSystem::m_StartTime = 0;
float FPSSystem::m_DeltaTime = 0.0f;
int FPSSystem::m_Count = 0;          // カウンタ
float FPSSystem::m_Fps = 0.0f;         // 現在のFPS

void FPSSystem::Init()
{
	// 測定開始時刻を現在のカウンタの値に設定
    m_StartTime = GetNowCount();
	m_Count = 0;
	m_Fps = 0;
}

void FPSSystem::Update()
{
	// 現在のカウンタの値を取得
    int now = GetNowCount();

	// 前回のカウンタの値が0の場合は、現在のカウンタの値を前回のカウンタの値として設定
    if (m_PrevTime == 0) m_PrevTime = now;
	// 前回のカウンタの値と現在のカウンタの値の差分を計算し、デルタタイムを更新
    m_DeltaTime = (now - m_PrevTime) / 1000.0f;
	// 現在のカウンタの値を前回のカウンタの値として保存
    m_PrevTime = now;

	// カウンタが0のときは、測定開始時刻を現在のカウンタの値に設定
    if (m_Count == 0) { m_StartTime = now; }

    // サンプル数と同じ回数フレームが回ったら平均を計算する
    if (m_Count == FPS_SAMPLE_NUM) { 
        // かかった時間（ﾐﾘ秒）をサンプル数で割り平均とする（その値が現在のFPS値）
        m_Fps = 1000.f / ((now - m_StartTime) / (float)FPS_SAMPLE_NUM);
        m_Count = 0;
        m_StartTime = now;
    }
    m_Count++;
}

void FPSSystem::Draw()
{
}

void FPSSystem::WaitFPS()
{
    // かかった時間
    int takeTime = GetNowCount() - m_StartTime;

    // 待機時間
    // FPSの値から1フレームにかかってほしい時間（g_Count * 1000 / FPS）
    // 実際に経過した時間（takeTime）
    // 実際の時間が早すぎた場合は、その差分を待機時間とする
    int waitTime = m_Count * 1000 / FPS - takeTime; 

    // 待機
    if (waitTime > 0) 
    {
        Sleep(waitTime);
    }
}
