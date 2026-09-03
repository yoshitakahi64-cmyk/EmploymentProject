#include "UIManager.h"
#include "UIBase.h"
#include "UIBase3D.h"
#include "UIHP.h"
#include "UIScore.h"
#include "UIManual.h"
#include "UIEnemyHP.h"

UIManager::UIManager()
{
	for (int i = 0; i < UI_ID_MAX; i++)
	{
		m_OriginalUI[i] = nullptr;
	}

	for (int i = 0; i < UI_ID_3D_MAX; i++)
	{
		m_OriginalUI3D[i] = nullptr;
	}
}

UIManager::~UIManager()
{
	Fin();
}

void UIManager::Init()
{
	m_OriginalUI[UI_ID_HP] = MakeUnique<UIHP>();
	m_OriginalUI[UI_ID_SCORE] = MakeUnique<UIScore>();
	m_OriginalUI[UI_ID_MANUAL] = MakeUnique<UIManual>();
	m_OriginalUI3D[UI_ID_ENEMY_HP] = MakeUnique<UIEnemyHP>();
}

void UIManager::Load()
{
	for (int i = 0; i < UI_ID_MAX; i++)
	{
		m_OriginalUI[i]->Load();
	}

	for (int i = 0; i < UI_ID_3D_MAX; i++)
	{
		m_OriginalUI3D[i]->Load();
	}
}

void UIManager::Start()
{
	for (auto& ui : m_UIs)
	{
		ui->Start();
	}

	for (auto& ui : m_UIs3D)
	{
		ui->Start();
	}
}

void UIManager::Step()
{
	for (auto& ui : m_UIs)
	{
		ui->Step();
    }

	for (auto& ui : m_UIs3D)
	{
		ui->Step();
	}
}

void UIManager::Update()
{
	for (auto& ui : m_UIs)
	{
		ui->Update();
	}

	for (auto& ui : m_UIs3D)
	{
		ui->Update();
	}
}

void UIManager::Draw()
{
	for (auto& ui : m_UIs)
	{
		ui->Draw();
	}

	for (auto& ui : m_UIs3D)
	{
		ui->Draw();
	}
}

void UIManager::Fin()
{
}

void UIManager::CreateUI(UIID ID, int x, int y)
{
	UniquePtr<UIBase> ui = m_OriginalUI[ID]->Clone();
	ui->SetPos(x, y);
	m_UIs.push_back(std::move(ui));
}

UIBase3D* UIManager::CreateUI3D(UIID3D ID, VECTOR pos)
{
	UniquePtr<UIBase3D> ui = m_OriginalUI3D[ID]->Clone();
	ui->SetPos(pos);

	UIBase3D* ptr = ui.get(); 
	m_UIs3D.push_back(std::move(ui));

	return ptr; 
}


