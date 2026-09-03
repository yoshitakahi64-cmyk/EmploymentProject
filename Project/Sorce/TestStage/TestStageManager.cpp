#include "TestStageManager.h"
#include "TestStage.h"

TestStageManager::TestStageManager()
{
	m_TestStage = nullptr;
}

void TestStageManager::CreateTestStage(VECTOR pos, Quaternion rot, VECTOR scale)
{
	if (!m_TestStage)
	{
		m_TestStage = MakeUnique<TestStage>();
	}

	m_TestStage->SetTransform(pos, rot, scale);

}

void TestStageManager::Init()
{
	if (!m_TestStage)
	{
		m_TestStage = MakeUnique<TestStage>();
	}

	if (m_TestStage)
	{
		m_TestStage->Init();
	}
}

void TestStageManager::Load()
{
	if (m_TestStage)
	{
		m_TestStage->Load();
	}
}

void TestStageManager::Start()
{
	if (m_TestStage)
	{
		m_TestStage->Start();
	}
}

void TestStageManager::Step()
{
	if (m_TestStage)
	{
		m_TestStage->Step();
	}
}

void TestStageManager::Update()
{
	if (m_TestStage)
	{
		m_TestStage->Update();
	}
}

void TestStageManager::Draw()
{
	if (m_TestStage)
	{
		m_TestStage->Draw();
	}
}

