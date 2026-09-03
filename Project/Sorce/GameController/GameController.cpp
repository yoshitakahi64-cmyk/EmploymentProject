#include "GameController.h"

bool GameController::m_IsFinishGame = false;

void GameController::Init()
{
	// ゲーム終了フラグを初期化
	m_IsFinishGame = false;
}

void GameController::Fin()
{
}
