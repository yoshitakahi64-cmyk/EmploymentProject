#include "GimmickManager.h"
#include "Gimmick.h"
#include "Button.h"
#include "Pulley.h"
#include "../GimmickDriven/Door.h"

GimmickManager::GimmickManager()
{
	// ギミックの原型配列を nullptr で初期化
	for (int i = 0; i < GIMMICK_TYPE_MAX; i++)
	{
		m_OriginalGimmick[i] = nullptr;
	}
}

GimmickManager::~GimmickManager()
{
	Fin();
}

void GimmickManager::Init()
{
	// ギミックの原型を生成して保存
	m_OriginalGimmick[BUTTON] = MakeUnique<Button>();
	m_OriginalGimmick[PULLEY] = MakeUnique<Pulley>();
	m_OriginalGimmick[DOOR] = MakeUnique<Door>();
}

void GimmickManager::Load()
{
	for (int i = 0; i < GIMMICK_TYPE_MAX; i++)
	{
		m_OriginalGimmick[i]->Load();
	}
}

void GimmickManager::Start()
{
	for (auto& gimmick : m_GimmickList)
	{
		gimmick->Start();
	}
}

void GimmickManager::Step()
{
	for (auto& gimmick : m_GimmickList)
	{
		gimmick->Step();
	}
}

void GimmickManager::Update()
{
	for (auto& gimmick : m_GimmickList)
	{
		gimmick->Update();
	}
}

void GimmickManager::Draw()
{
	for (auto& gimmick : m_GimmickList)
	{
		gimmick->Draw();
	}
}

void GimmickManager::Fin()
{
	// ギミックリストをクリア
	m_GimmickList.clear();
}

void GimmickManager::AddGimmick(GimmickType type, VECTOR pos, Quaternion rot, VECTOR scale,std::string target)
{
	// タイプに合わせたギミックを生成
	UniquePtr<Gimmick> gimmick = m_OriginalGimmick[type]->Clone();

	// 位置、回転、スケールを設定
	gimmick->SetTransform(pos, rot, scale);

	// ターゲットIDを設定
	if (type == BUTTON)
	{
		Button* button = static_cast<Button*>(gimmick.get());
		button->SetTargetID(target);
	}
	else if (type == PULLEY)
	{
		Pulley* pulley = static_cast<Pulley*>(gimmick.get());
		pulley->SetTargetID(target);
	}
	else if (type == DOOR)
	{
		Door* door = static_cast<Door*>(gimmick.get());
		door->SetTargetID(target);
	}

	// 生成したギミックをリストに追加
	m_GimmickList.push_back(std::move(gimmick));
}
