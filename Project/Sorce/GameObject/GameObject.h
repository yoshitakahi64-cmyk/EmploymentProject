#pragma once
#include <DxLib.h>
#include "Transform.h"

// 全ゲームオブジェクトの基底クラス
class GameObject
{
public:
	GameObject(); // コンストラクタ
	~GameObject() = default; // デストラクタ

public:
	void Init(); // 初期化関数
	virtual void Load() {} // ロード関数
	virtual void Start() {} // スタート関数
	virtual void Step() {} // ステップ関数
	virtual void Update() {} // アップデート関数
	virtual void Draw() {} // 描画関数
	virtual void Fin() {} // 終了関数
	
public:
	// ハンドルを取得する関数
	int Gethandle() { return m_Handle; }

protected:
	// position,rotation,scaleの要素を持った構造体Transfomの変数
	Transform m_Transform;
	// 空中フラグ
	bool m_IsFall;
	// ハンドル
	int m_Handle;
};