#pragma once
#include "GameObject.h"

// 2DGameObjectクラス
class GameObject2D : public GameObject
{
public:
	GameObject2D(); // コンストラクタ
	~GameObject2D(); // デストラクタ

public:
	virtual void Load(); // ロード関数
	virtual void Start(); // スタート関数
	virtual void Step(); // ステップ関数
	virtual void Update(); // アップデート関数
	virtual void Draw() = 0; // 描画関数（純粋仮想関数）
    void Fin()override;

	// 2D座標のセッター関数
	void SetPos(int x,int y) { m_Transform.posX_2D = x; m_Transform.posY_2D = y; }

protected:
	// 表示フラグ
	bool m_Hide;
};