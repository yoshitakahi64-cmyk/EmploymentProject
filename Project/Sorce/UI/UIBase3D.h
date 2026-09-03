#pragma once

#include "../Memory.h"
#include <DxLib.h>
#include <functional>

// 3D UIの基底クラス
class UIBase3D
{
public:
	UIBase3D(); // コンストラクタ
	virtual ~UIBase3D(); // デストラクタ

public:
	virtual void Init(); // 初期化関数
	virtual void Load(); // ロード関数
	virtual void Start(); // スタート関数
	virtual void Step(); // ステップ関数
	virtual void Update(); // 更新関数
	virtual void Draw(); // 描画関数
	virtual void Fin(); // 終了関数

	// クローン関数
	virtual  UniquePtr<UIBase3D> Clone() { return nullptr; }

public:
	// UIを非表示にする関数
	void Hide() { m_Hide = true; }
	// UIを表示する関数
	void Show() { m_Hide = false; }
	// UIが非表示かどうかを返す関数
	bool IsHide() const { return m_Hide; }
	// UIの位置を設定する関数
	void SetPos(VECTOR pos) { m_Pos = pos; }
	// UIの位置を参照してくる関数
	void SetPositionProvider(std::function<VECTOR()> provider) { m_PositionProvider = provider; }



protected:
	// UIの位置を提供する関数
	std::function<VECTOR()> m_PositionProvider;
	// UIの位置
	VECTOR m_Pos;
	// UIが非表示かどうか
	bool m_Hide;
};
