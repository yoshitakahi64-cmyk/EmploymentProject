#pragma once

#include "../GameObject/3DGameObject.h"
#include "../Memory.h"

// 引っ張られるオブジェクトの基底クラス
class PullObjectBase : public GameObject3D
{
public:
	PullObjectBase() = default; // コンストラクタ
	~PullObjectBase() = default; // デストラクタ

public:
	virtual void Load() {}; // ロード関数
	void Start()override; // スタート関数
	virtual void OnStart() {}; // 継承先スタート関数
	void Step()override; // ステップ関数

	// クローン関数
	virtual  UniquePtr<PullObjectBase> Clone() { return nullptr; }

public:
	// 引っ張られたときの処理
	virtual void OnPulling(float pullAmount, const VECTOR& pulleyPos) = 0;

	// 引っ張りが止まったときの処理
	virtual void OnStopPulling() = 0;
};
