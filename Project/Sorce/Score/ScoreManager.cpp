#include "ScoreManager.h"
#include "Score.h"

ScoreManager::ScoreManager()
{
}

void ScoreManager::CreateScore()
{
	// スコアが生成されていない場合は生成する
	if (!m_Score)
	{
		m_Score = MakeUnique<Score>();
	}
}

void ScoreManager::Init()
{
	// スコアが存在する場合は初期化する
	if (m_Score)
	{
		m_Score->Init();
	}
}

void ScoreManager::Fin()
{

}
