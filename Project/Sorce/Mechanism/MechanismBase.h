#pragma once

#include "../GameObject/3DGameObject.h"
#include <string>

// メカニズムの基底クラス
class MechanismBase : public GameObject3D
{
public:
	MechanismBase() = default; // コンストラクタ
	virtual ~MechanismBase() = default; // デストラクタ

	// ターゲットIDのセッター関数
	void SetTargetID(const std::string& id) { m_TargetID = id; }

public:
	// ギミック作動関数
	virtual void Activate() = 0;
	// ギミック停止関数
	virtual void Deactivate() {}

protected:
	// ターゲットID
	std::string m_TargetID;
};