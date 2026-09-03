#pragma once

#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"
#include <DxLib.h>
#include "../Quatrenion/Quatrenion.h"

class PullObjectBase;

// 引っ張られるオブジェクトのタイプ
enum PullObjectType
{
	BALL,
	PULLOBJECT_TYPE_MAX,
	PULLOBJECT_NONE
};

// 引っ張られるオブジェクトの管理クラス
class PullObjectManager : public Singleton<PullObjectManager>
{
public:
	PullObjectManager(); // コンストラクタ
	~PullObjectManager(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Load(); // ロード関数
	void Start(); // スタート関数
	void Step(); // ステップ関数
	void Update(); // 更新関数
	void Draw(); // 描画関数
	void Fin(); // 終了関数

public:
	// 引っ張られるオブジェクト追加関数
	void AddPullObject(PullObjectType type, VECTOR pos, Quaternion rot, VECTOR scale);
	// 引っ張られるオブジェクトリスト取得関数
	const List<UniquePtr<PullObjectBase>>& GetPullObjectList() const { return m_PullObjectList; }

private:
	// 各タイプのオブジェクトの原型
	UniquePtr<PullObjectBase> m_OriginalPullObject[PULLOBJECT_TYPE_MAX];
	// 引っ張られるオブジェクトのリスト
	List<UniquePtr<PullObjectBase>> m_PullObjectList;
};