#pragma once

#include "Gimmick.h"
#include "../Quatrenion/Quatrenion.h"
#include <string>

class MechanismBase;
class GameObject3D;

// 滑車クラス
class Pulley : public Gimmick
{
public:
	Pulley(); // コンストラクタ
	~Pulley() = default; // デストラクタ

public:
	// 読み込み
	void Load()override;
	// Start
	void OnStart()override;
	// Step
	void OnStep()override;

	// クローン関数
	UniquePtr<Gimmick> Clone()override;

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Pulley"; }
	void SetTargetID(std::string id) { m_TargetID = id; }
	VECTOR GetPullDirection()const;

public:
	// 滑車起動関数
	void OnPulling();
	// 滑車引っ張り関数
	void OnPull();
	// 滑車停止関数
	void OnStopPulling();
	// メカニズム設定
	void SettingMechanism();

private:
	// アニメーション管理関数
	void ControllAnimation()override;

	// 当たり判定関係関数
	void OnCollisionEnter()override;
	void OnCollisionExit()override;

private:
	// 滑車起動中フラグ
	bool m_IsPulling;
	// ターゲットID
	std::string m_TargetID;
	// メカニズム
	MechanismBase* m_Mechanism;
	// ターゲットギミック
	GameObject3D* m_TargetGimmick;

	// 滑車の位置(基点)
	VECTOR m_PulleyPos;
	// 対象(ロープ先)の位置
	VECTOR m_TargetPos;
	// ロープの色
	unsigned int m_RopeColor;
	// ロープの太さ
	float m_RopeWidth;

	// 滑車のモデル半径
	float m_PulleyRadius;
	// 回転速度
	float m_RotSpeed;
	// 前フレームの距離
	float m_PrevDistance;
};