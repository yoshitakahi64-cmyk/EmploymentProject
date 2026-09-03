#pragma once

#include "Gimmick.h"
#include <string>

class MechanismBase;
class GameObject3D;

// ボタンクラス
class Button : public Gimmick
{
public:
	Button(); // コンストラクタ
	~Button() = default; // デストラクタ

public:
	void Load()override; // ロード関数
	void OnStart()override; // 継承先スタート関数

	// クローン関数
	UniquePtr<Gimmick> Clone()override;

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Button"; }
	void SetTargetID(std::string id) { m_TargetID = id; }

public:
	// ボタンオン関数
	void OnPressed();
	// ボタンオフ関数
	void OnReleased();
	// 動かしたいギミックの動きを設定する関数
	void SettingMechanism();

private:
	// アニメーション管理関数
	void ControllAnimation()override;
	// 当たり判定関数
	void OnCollisionEnter()override;
	void OnCollisionExit()override;

private:
	// ボタンが押されているかのフラグ
	bool m_IsPush;
	// ボタンが押された瞬間のフラグ
	bool m_IsPressed;
	// 対象のID
	std::string m_TargetID;
	// 対象のギミックの動きを管理するクラスのポインタ
	MechanismBase* m_Mechanism;
};
