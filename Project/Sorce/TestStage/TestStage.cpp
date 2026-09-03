#include "TestStage.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyManager.h"
#include "../Collision/Capsule.h"
#include "../Quatrenion/Quatrenion.h"
#include "../MyMath/MyMath.h"
#include "../Container.h"
#include "../Memory.h"

TestStage::TestStage()
{
}

TestStage::~TestStage()
{
	Fin();
}

void TestStage::Load()
{
	// モデルをロード
	std::shared_ptr<MyResource> testStageResorce = MyResourceManager::GetInstance()->LoadModelResource("Data/Room/PlayStage/PlayStage2.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (testStageResorce)
	{
	   m_Handle = testStageResorce->GetHandle();
	}
	else if (!testStageResorce)
	{
		int test = 0;
	}
}

void TestStage::Start()
{
	// 生存フラグをオン
	m_IsActive = true;

	// 衝突情報を構築
	MV1SetupCollInfo(m_Handle);

	MV1RefreshCollInfo(m_Handle);
}
