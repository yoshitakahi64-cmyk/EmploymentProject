#pragma once

#include "DxLib.h"

// Effekseerエフェクト１つ分のクラス
class EffekseerEffect
{
public:
	EffekseerEffect();	// コンストラクタ
	~EffekseerEffect();	// デストラクタ

public:
	void Step();	// ステップ
	void Update();	// 更新
	void Fin();		// 終了

	// 再生関数
	// efkefcデータのリソースハンドルを渡す
	void Play(int handle);
	// エフェクト停止
	void Stop();

	// ゲッター・セッター関数
	bool IsActive() const { return m_Active; }
	void SetActive(bool active) { m_Active = active; }
	void SetPos(VECTOR pos) { m_Pos = pos; }

	// 追跡用関数
	void SetTracking(VECTOR* target,bool* activeFlag);
	void StopTracking();

private:
	bool m_Active;	// アクティブフラグ
	int m_Handle;	// プレイハンドル
	VECTOR m_Pos;	// 座標

	// 位置追跡フラグ
	bool m_IsTracking;
	// 追跡対象の生存フラグ
	bool* m_IsTrackTargetActive;
	// 追跡対象の座標ポインタ
	VECTOR* m_TrackTarget;
};
