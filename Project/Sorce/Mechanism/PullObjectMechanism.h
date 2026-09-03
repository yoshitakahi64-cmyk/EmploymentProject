#pragma once

#include "../Mechanism/MechanismBase.h"
#include "../PullObject/PullObjectBase.h"
#include <string>

class Pulley;

// 引っ張りオブジェクトを操作するメカニズムクラス
class PullObjectMechanism : public MechanismBase
{
public:
	PullObjectMechanism(); // コンストラクタ
	~PullObjectMechanism() = default; // デストラクタ

	// ゲッター関数
	PullObjectMechanism GetPullObjectMechanism() const { return *this; }
public:
	// ギミック作動関数
	void Activate()override;
	// ギミック停止関数
	void Deactivate()override;

	// 引っ張っているオブジェクトのセット関数
	void SetPulley(Pulley* pulley) { m_CurrentPulley = pulley; }
	// 引っ張られているオブジェクトのセット関数
	void SetPullObject(PullObjectBase* obj) { m_CurrentPullObject = obj; }
private:
	// 引っ張ているオブジェクト
	Pulley* m_CurrentPulley;
	// 引っ張られているオブジェクト
	PullObjectBase* m_CurrentPullObject;
};