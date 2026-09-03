#include "SceneBase.h"

SceneBase::SceneBase():
	m_State(INIT)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Exec()
{
	// シーンの状態に応じた関数を呼び出す
	switch (m_State)
	{
	case INIT:
		Init();
		break;
	case LOAD:
		Load();
		break;
	case START:
		Start();
		break;
	case LOOP:
		Step();
		Update();
		Draw();
		break;
	case FIN:
		Fin();
		break;
	default:
		break;
	}
}
