#include "PullObjectManager.h"  
#include "PullObjectBase.h"
#include "Ball.h"  

PullObjectManager::PullObjectManager()  
{  
   for (int i = 0; i < PULLOBJECT_TYPE_MAX; i++)  
   {  
       m_OriginalPullObject[i] = nullptr;  
   }  
}

PullObjectManager::~PullObjectManager()
{
	Fin();
}

void PullObjectManager::Init()  
{  
   m_OriginalPullObject[BALL] = MakeUnique<Ball>();  
}  

void PullObjectManager::Load()  
{  
	for (int i = 0; i < PULLOBJECT_TYPE_MAX; i++)
	{
		m_OriginalPullObject[i]->Load();
	}
}  

void PullObjectManager::Start()  
{  
	for (auto& pullObject : m_PullObjectList)
	{
		pullObject->Start();
	}
}  

void PullObjectManager::Step()  
{  
	for (auto& pullObject : m_PullObjectList)
	{
		pullObject->Step();
	}
}  

void PullObjectManager::Update()  
{  
	for (auto& pullObject : m_PullObjectList)
	{
		pullObject->Update();
	}
}  

void PullObjectManager::Draw()  
{  
	for (auto& pullObject : m_PullObjectList)
	{
		pullObject->Draw();
	}
}  

void PullObjectManager::Fin()  
{  
	m_PullObjectList.clear();
}

void PullObjectManager::AddPullObject(PullObjectType type, VECTOR pos, Quaternion rot, VECTOR scale)
{  
	// タイプに合わせたギミックを生成
	UniquePtr<PullObjectBase> gimmick = m_OriginalPullObject[type]->Clone();

	// 生成したギミックのTransformを設定
	gimmick->SetTransform(pos, rot, scale);

	// 生成したギミックをリストに追加
	m_PullObjectList.push_back(std::move(gimmick));
}
