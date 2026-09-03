#include "DoorMechanism.h"
#include "../GimmickDriven/Door.h"
#include "../Mechanism/IDManager.h"


DoorMechanism::DoorMechanism():
	m_Door(nullptr)
{
}

void DoorMechanism::Activate()
{
	// IDManagerを使ってターゲットIDからドアオブジェクトを取得
	GameObject3D* obj = IDManager::GetInstance()->FindByID(m_TargetID);
	m_Door = dynamic_cast<Door*>(obj);

	// ドアオブジェクトが見つかった場合、ドアを開ける
	if (m_Door)
	{
		m_Door->Open();
	}
}

void DoorMechanism::Deactivate()
{
	// IDManagerを使ってターゲットIDからドアオブジェクトを取得
	GameObject3D* obj = IDManager::GetInstance()->FindByID(m_TargetID);
	m_Door = dynamic_cast<Door*>(obj);

	// ドアオブジェクトが見つかった場合、ドアを閉める
	if (m_Door)
	{
		m_Door->Close();
	}
}
