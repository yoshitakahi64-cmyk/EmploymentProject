#include "PullObjectMechanism.h"
#include "../PullObject/Ball.h"
#include "../Mechanism/IDManager.h"
#include "../Gimmick/Pulley.h"

PullObjectMechanism::PullObjectMechanism():
		m_CurrentPulley(nullptr),
	m_CurrentPullObject(nullptr)
{
}

void PullObjectMechanism::Activate()
{
	GameObject3D* obj = IDManager::GetInstance()->FindByID(m_TargetID);
	m_CurrentPullObject = dynamic_cast<Ball*>(obj);

	if (m_CurrentPullObject)
	{
		// 引っ張る量と滑車の位置を渡す
		m_CurrentPullObject->OnPulling(0.3f, m_CurrentPulley->GetPos3D());
	}
}

void PullObjectMechanism::Deactivate()
{
	if (m_CurrentPullObject)
	{
		// 引っ張り停止を伝える
		m_CurrentPullObject->OnStopPulling();
	}
}
