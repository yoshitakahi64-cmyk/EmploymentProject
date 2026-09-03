#pragma once

#include "StageObject.h"

// 空いているドアクラス
class DoorOpen : public StageObject
{
public:
	DoorOpen(); // コンストラクタ
	~DoorOpen() = default; // デストラクタ

public:
	void Load()override; // ロード関数
	void OnStart()override; // 継承先スタート関数

	// クローン関数
	UniquePtr<StageObject> Clone()override;

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Door_Open"; }
};
