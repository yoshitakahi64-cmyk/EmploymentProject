#pragma once

#include "PullObjectBase.h"

// ボールクラス
class Ball : public PullObjectBase
{
public:
	Ball(); // コンストラクタ
	~Ball() = default; // デストラクタ

public:
	void Load()override; // ロード関数
	void OnStart()override; // 継承先スタート関数

	// クローン関数
	UniquePtr<PullObjectBase> Clone()override;

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Ball"; }

	// 引っ張られたときの処理
	void OnPulling(float pullAmount, const VECTOR& pulleyPos)override;

	// 引っ張りが止まったときの処理
	void OnStopPulling()override;
};