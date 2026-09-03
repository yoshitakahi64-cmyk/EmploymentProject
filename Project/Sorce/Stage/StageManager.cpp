#include "StageManager.h"
#include "StageParam.h"
#include "../Mechanism/IDManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Player/PlayerManager.h"
#include "../Gimmick/GimmickManager.h"
#include "../PullObject/PullObjectManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../TestStage/TestStageManager.h"
#include <fstream>

// usingして使いやすくする
using json = nlohmann::json;

// データがまとまっている階層のキー名
constexpr const char* KEY_ITEMS = "items";

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
	Fin();
}

void StageManager::Load(const char* fileName)
{
	// JSONファイルを開く
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		// ファイルが開けなかった場合のエラーハンドリング
		return;
	}

	// 開いたファイルをjsonクラスに取り込み
	json stageJson;
	file >> stageJson;

	// from_json関数を使ってデータをm_StageObjectsに変換して格納
	m_Objects = stageJson[KEY_ITEMS].get<Vector<GameObject_Unity>>();

	file.close();
	
}

void StageManager::Start()
{
	for (GameObject_Unity obj : m_Objects)
	{
		
		// Stage
		if (obj.id == TestRoom_Model)
		{
			TestStageManager::GetInstance()->CreateTestStage(obj.position_3D,obj.rotation,obj.scale);
		}
		// Player
		else if (obj.id == Player_Model)
		{
			PlayerManager::GetInstance()->CreatePlayer(obj.position_3D, obj.rotation, obj.scale);
		}
		
		// ShotEnemy
		else if (obj.id == ShotEnemy_Model)
		{
			EnemyManager::GetInstance()->CreateEnemy(SHOT_ENEMY, obj.position_3D, obj.rotation, obj.scale);
		}
		
		// RushEnemy
		else if (obj.id == RushEnemy_Model)
		{
			EnemyManager::GetInstance()->CreateEnemy(RUSH_ENEMY, obj.position_3D, obj.rotation, obj.scale);
		}
		
		// Button
		else if (obj.id == Button_Model)
		{
			GimmickManager::GetInstance()->AddGimmick(BUTTON ,obj.position_3D, obj.rotation, obj.scale,obj.target);
		}
		// Door
		else if (obj.id == Door_Model)
		{
			GimmickManager::GetInstance()->AddGimmick(DOOR, obj.position_3D, obj.rotation, obj.scale, obj.target);
		}
		// Pully
		else if (obj.id == Pully_Model)
		{
			GimmickManager::GetInstance()->AddGimmick(PULLEY,obj.position_3D, obj.rotation, obj.scale,obj.target);
		}
		// Ball
		else if (obj.id == Ball_Model)
		{
			PullObjectManager::GetInstance()->AddPullObject(BALL, obj.position_3D, obj.rotation, obj.scale);
		}
		// WoodBox
		else if (obj.id == WoodBox_Model)
		{
			StageObjectManager::GetInstance()->AddObject(WOODBOX,obj.position_3D, obj.rotation, obj.scale);
		}
		// Goal
		else if (obj.id == Goal_Model)
		{
			StageObjectManager::GetInstance()->AddObject(GOAL, obj.position_3D, obj.rotation, obj.scale);
		}
		else if (obj.id == DoorOpen_Model)
		{
			StageObjectManager::GetInstance()->AddObject(DOOR_OPEN, obj.position_3D, obj.rotation, obj.scale);
		}	
		
	}
}

void StageManager::Fin()
{

}
