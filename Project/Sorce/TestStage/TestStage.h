#pragma once
#include <DxLib.h>
#include "../GameObject/3DGameObject.h"

// プレイステージクラス
class TestStage : public GameObject3D
{
public:
	TestStage(); // コンストラクタ
	~TestStage(); // デストラクタ

public:	
	void Load()override; // ロード関数
	void Start()override; // スタート関数

};
