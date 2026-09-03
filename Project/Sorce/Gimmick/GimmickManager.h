#pragma once

#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"
#include "../Quatrenion/Quatrenion.h"
#include <string>

class Gimmick;

// ギミックの種類を表す列挙型
enum GimmickType
{
	BUTTON,
	PULLEY,
	DOOR,
	GIMMICK_TYPE_MAX,
	GIMMICK_NONE
};

// ギミック管理クラス
class GimmickManager : public Singleton<GimmickManager>
{
public:
	GimmickManager(); // コンストラクタ
	~GimmickManager(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Load(); // ロード関数
	void Start(); // スタート関数
	void Step(); // ステップ関数
	void Update(); // アップデート関数
	void Draw(); // 描画関数
	void Fin(); // 終了関数

public:
	// ギミックの追加関数
	void AddGimmick(GimmickType type,VECTOR pos, Quaternion rot, VECTOR scale,std::string target);

	// ギミックのリストを取得する関数
	const List<UniquePtr<Gimmick>>& GetGimmickList() const { return m_GimmickList; }

private:
	// ギミックの原型を保存する配列
	UniquePtr<Gimmick> m_OriginalGimmick[GIMMICK_TYPE_MAX];
	// 生成中のギミックを保存するリスト
	List<UniquePtr<Gimmick>> m_GimmickList;
};