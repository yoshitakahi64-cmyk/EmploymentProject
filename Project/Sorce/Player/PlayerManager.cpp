#include "PlayerManager.h"
#include "../Player/Player.h"

PlayerManager::PlayerManager()
{
	m_Player = nullptr;
}

PlayerManager::~PlayerManager()
{
	Fin();
}

void PlayerManager::CreatePlayer(VECTOR pos, Quaternion rot, VECTOR scale)
{	
	// プレイヤーが存在しない場合は生成
	if (!m_Player)
	{
		m_Player = MakeUnique<Player>();
	}
	// プレイヤーの位置、回転、スケールを設定
	m_Player->SetTransform(pos, rot, scale);
}

void PlayerManager::Init()
{
	if (!m_Player)
	{
		m_Player = MakeUnique<Player>();
	}

	if (m_Player)
	{
		m_Player->Init();
	}
}

void PlayerManager::Load()
{
	if (m_Player)
	{
		m_Player->Load();
	}
}

void PlayerManager::Start()
{
	if (m_Player)
	{
		m_Player->Start();
	}
}

void PlayerManager::Step()
{
	if (m_Player)
	{
		m_Player->Step();
	}
}

void PlayerManager::Update()
{
	if (m_Player)
	{
		m_Player->Update();
	}
}

void PlayerManager::Draw()
{
	if (m_Player)
	{
		m_Player->Draw();
	}
}

void PlayerManager::Fin()
{
	if (m_Player)
	{
		m_Player->Fin();
	}
}
