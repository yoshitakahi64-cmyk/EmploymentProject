#include <DxLib.h>
#include "CollisionStage.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionSphere.h"
#include "../Collision/Capsule.h"
#include "../TestStage/TestStage.h"
#include "../TestStage/TestStageManager.h"
#include "../MyMath/MyMath.h"


CollisionStage::CollisionStage()
{
}

MV1_COLL_RESULT_POLY_DIM CollisionStage::StageCollision(CollisionAABB* aabb)
{
	// 当たり判定結果格納用
	MV1_COLL_RESULT_POLY_DIM hitResult = {};

	return hitResult;
}

MV1_COLL_RESULT_POLY_DIM CollisionStage::StageCollision(CollisionSphere* sphere)
{
	// 当たり判定結果格納用
	MV1_COLL_RESULT_POLY_DIM hitResult = {};

	return hitResult;
}

MV1_COLL_RESULT_POLY_DIM CollisionStage::StageCollision(Capsule* capsule)
{
	if (!capsule->m_IsActive)
	{
		MV1_COLL_RESULT_POLY_DIM noHitResult = {};
		return noHitResult;
	}

	// 当たり判定結果格納用
	MV1_COLL_RESULT_POLY_DIM hitResult = {};

	// テストステージを取得
	TestStage* testStage = TestStageManager::GetInstance()->GetTestStage();

	// カプセルの上端と下端の座標を取得
	VECTOR top = capsule->GetRotatedTopPos();
	VECTOR bottom = capsule->GetRotatedBottomPos();

	// カプセルの上端と下端の座標から、カプセルの上方向を取得
	VECTOR upDir = capsule->GetUpDirection();

	// 上端と下端の座標を上方向に投影して、どちらが上端かを判定
	float topDot = MyMath::VecDot(top, upDir);
	float bottomDot = MyMath::VecDot(bottom, upDir);

	// 上端と下端の座標を上方向に投影して、どちらが上端かを判定
	VECTOR pUpper = (topDot > bottomDot) ? top : bottom;
	VECTOR pLower = (topDot > bottomDot) ? bottom : top;

	// オブジェクトとステージの当たり判定
	hitResult = MV1CollCheck_Capsule(testStage->Gethandle(), -1,
		top, bottom, capsule->GetRadius());

	if (hitResult.HitNum != 0)
	{
		int test = 0;
	}

	return hitResult;
}
