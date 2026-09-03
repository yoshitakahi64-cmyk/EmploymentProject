#pragma once
#include "../GameObject/3DGameObject.h"
#include "../Memory.h"

// ギミックの基底クラス
class Gimmick : public GameObject3D
{
public:
	Gimmick(); // コンストラクタ
	~Gimmick(); // デストラクタ
public:
	virtual void Load() {} // ロード関数
	void Start()override; // スタート関数
	virtual void OnStart() {} // 継承先スタート関数
	void Step()override; // ステップ関数
	virtual void OnStep() {} // 継承先ステップ関数


	// 押し出し処理使用フラグ
	bool GetIsPushing() { return m_IsPushing; }

	// クローン関数
	virtual  UniquePtr<Gimmick> Clone() { return nullptr; }

	// アニメーション管理関数
    virtual void ControllAnimation(){}

protected:
	// 重力適用フラグ
	bool m_IsGravityApplied;
	// 押し出し処理使用フラグ
	bool m_IsPushing;

};
