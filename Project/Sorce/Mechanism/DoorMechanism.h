#pragma once

#include "../Mechanism/MechanismBase.h"
#include <string>

class Door;

// ドアギミッククラス
class DoorMechanism : public MechanismBase
{
public:
	DoorMechanism(); // コンストラクタ
	~DoorMechanism() = default; // デストラクタ
	
	// ゲッター関数
	DoorMechanism GetDoorMechanism() const { return *this; }
public:
	// ギミック作動関数と停止関数のオーバーライド
	void Activate()override;
	void Deactivate()override;
private:
	// ドアのポインタ
	Door* m_Door;
};
