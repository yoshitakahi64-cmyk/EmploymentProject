#pragma once

#include "../Singleton.h"
#include "UIBase.h"
#include "UIParameter.h"
#include "../Memory.h"
#include "../Container.h"

class UIBase;
class UIBase3D;

// UI管理クラス
class UIManager : public Singleton<UIManager>
{
public:
	UIManager(); // コンストラクタ
	~UIManager(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Load(); // ロード関数 
	void Start();// スタート関数
	void Step(); // ステップ関数
	void Update(); // 更新関数
	void Draw(); // 描画関数
	void Fin(); // 終了関数

	// UI生成関数
	void CreateUI(UIID ID, int x, int y);
	// 3DUI生成関数
	UIBase3D* CreateUI3D(UIID3D ID, VECTOR pos);
	

private:
	// 各UIの原型
	UniquePtr<UIBase> m_OriginalUI[UI_ID_MAX];
	// UIのリスト
	List<UniquePtr<UIBase>> m_UIs;

	// 各3DUIの原型
	UniquePtr<UIBase3D> m_OriginalUI3D[UI_ID_MAX];
	// 3DUIのリスト
	List<UniquePtr<UIBase3D>> m_UIs3D;
};
