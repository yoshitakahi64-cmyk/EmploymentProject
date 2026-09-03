#include "StageObjectManager.h"
#include "StageObject.h"
#include "WoodBox.h"
#include "MoveWater.h"
#include "Goal.h"
#include "../GimmickDriven/Door.h"
#include "DoorOpen.h"

StageObjectManager::StageObjectManager()
{
	for (int i = 0; i < STAGEOBJECT_TYPE_MAX; i++)
	{
		m_OriginalStageObject[i] = nullptr;
	}
}

void StageObjectManager::Init()
{
	m_OriginalStageObject[WOODBOX] = MakeUnique<WoodBox>();
	m_OriginalStageObject[MOVEWATER] = MakeUnique<MoveWater>();
	m_OriginalStageObject[GOAL] = MakeUnique<Goal>();
	m_OriginalStageObject[DOOR_OPEN] = MakeUnique<DoorOpen>();
}

void StageObjectManager::Load()
{
	for (int i = 0; i < STAGEOBJECT_TYPE_MAX; i++)
	{
		m_OriginalStageObject[i]->Load();
	}
}

void StageObjectManager::Start()
{
	for (auto& stageObject : m_StageObjectList)
	{
		stageObject->Start();
	}
}

void StageObjectManager::Step()
{
	for (auto& stageObject : m_StageObjectList)
	{
		stageObject->Step(); 
	}
}

void StageObjectManager::Update()
{
	for (auto& stageObject : m_StageObjectList)
	{
		stageObject->Update();
	}
}

void StageObjectManager::Draw()
{
	for (auto& stageObject : m_StageObjectList)
	{
		stageObject->Draw();
	}
}


void StageObjectManager::AddObject(StageObjectType type, VECTOR pos, Quaternion rot, VECTOR scale)
{
	// タイプに合わせたオブジェクトを生成
	UniquePtr<StageObject> stageObject = m_OriginalStageObject[type]->Clone();

	// 位置、回転、スケールを設定
	stageObject->SetTransform(pos, rot, scale);

	// 生成したオブジェクトをリストに追加
	m_StageObjectList.push_back(std::move(stageObject));
}
