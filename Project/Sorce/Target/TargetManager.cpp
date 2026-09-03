#include "TargetManager.h"
#include "Target.h"

TargetManager::TargetManager()
{

}

void TargetManager::CreateTarget()
{
	m_Target = MakeUnique<Target>();
}

void TargetManager::Update()
{
	if (m_Target)
	{
		m_Target->Update();
	}
}
