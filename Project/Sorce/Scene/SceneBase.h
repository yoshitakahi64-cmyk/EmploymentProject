#pragma once
#include "DxLib.h"
#include "SceneParameter.h"

// シーンの基底クラス
class SceneBase
{
public:
	SceneBase(); // コンストラクタ			
	virtual ~SceneBase(); // デストラクタ

public:
	virtual void Init() = 0; // 初期化関数
	virtual void Load() = 0; // ロード関数
	virtual void Start() = 0; // スタート関数
	virtual void Step() = 0; // ステップ関数		
	virtual void Update() = 0; // 更新関数
	virtual void Draw() = 0; // 描画関数
	virtual void Fin() = 0; // 終了関数

	// シーンの実行関数
	void Exec();

protected:
	// シーンの状態
	SceneState m_State;
	


};
