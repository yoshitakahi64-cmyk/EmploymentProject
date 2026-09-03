#pragma once

#include "DxLib.h"
#include "../GameObject/2DGameObject.h"
#include "../Memory.h"

// UIの基底クラス
class UIBase : public GameObject2D
{
public:
	UIBase(); // コンストラクタ
	virtual ~UIBase(); // デストラクタ

public:
	virtual void Init(); // 初期化関数
	virtual void Load(); // ロード関数
	virtual void Start(); // スタート関数
	virtual void Step(); // ステップ関数
	virtual void Update(); // 更新関数

	// クローン関数
	virtual  UniquePtr<UIBase> Clone() { return nullptr; }

public:
	// UIを非表示にする関数
	void Hide() { m_Hide = true; }
	// UIを表示する関数
	void Show() { m_Hide = false; }
	// UIが非表示かどうかを返す関数
	bool IsHide() const { return m_Hide; }

protected:
	// 非表示フラグ
	bool m_Hide;
};
