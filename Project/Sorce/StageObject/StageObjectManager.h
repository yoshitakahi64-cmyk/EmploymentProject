#pragma once

#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"
#include <DxLib.h>
#include "../Quatrenion/Quatrenion.h"

class StageObject;

// ステージオブジェクトのタイプ
enum StageObjectType
{
	WOODBOX,
	MOVEWATER,
	GOAL,
	DOOR_OPEN,
	STAGEOBJECT_TYPE_MAX,
	STAGEOBJECT_NONE
};

// ステージオブジェクトの管理クラス
class StageObjectManager : public Singleton<StageObjectManager>
{
public:
	StageObjectManager(); // コンストラクタ
	~StageObjectManager() = default; // デストラクタ

public:
	void Init(); // 初期化関数
	void Load(); // ロード関数
	void Start(); // スタート関数
	void Step(); // ステップ関数
	void Update(); // 更新関数
	void Draw(); // 描画関数

public:
	// ステージオブジェクト追加関数
	void AddObject(StageObjectType type, VECTOR pos ,Quaternion rot ,VECTOR scale);
	// ステージオブジェクトリスト取得関数
	const List<UniquePtr<StageObject>>& GetStageObjectList() const { return m_StageObjectList; }

private:
	// 各タイプのオブジェクトの原型
	UniquePtr<StageObject> m_OriginalStageObject[STAGEOBJECT_TYPE_MAX];
	// ステージオブジェクトのリスト
	List<UniquePtr<StageObject>> m_StageObjectList;
};
