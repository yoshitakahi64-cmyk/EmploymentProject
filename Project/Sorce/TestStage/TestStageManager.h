#pragma once
#include "../Singleton.h"
#include "../Memory.h"
#include <DxLib.h>
#include "../Quatrenion/Quatrenion.h"

class TestStage;

// プレイステージ管理クラス
class TestStageManager : public Singleton<TestStageManager>
{
public:
	TestStageManager();	// コンストラクタ
	~TestStageManager() = default;	// デストラクタ

public:

	void Init();	// 初期化関数
	void Load();	// ロード関数
	void Start();	// スタート関数
	void Step();	// ステップ関数
	void Update();	// 更新関数
	void Draw();	// 描画関数

public:
	//  テストステージを生成する
	void CreateTestStage(VECTOR pos,Quaternion rot,VECTOR scale);

	// 管理中のプレイヤーを取得する
    TestStage* GetTestStage() { return m_TestStage.get(); }

private:
	// 生成されたステージの参照
	UniquePtr<TestStage> m_TestStage;
};
