#pragma once

#include "../Singleton.h"

class Capsule;
class CollisionAABB;
class CollisionSphere;

// ステージの当たり判定クラス
class CollisionStage : public Singleton<CollisionStage>
{
public:
	CollisionStage(); // コンストラクタ
	~CollisionStage() = default; // デストラクタ

public:
	static MV1_COLL_RESULT_POLY_DIM StageCollision(CollisionAABB*);
	static MV1_COLL_RESULT_POLY_DIM StageCollision(CollisionSphere*);
	static MV1_COLL_RESULT_POLY_DIM StageCollision(Capsule*);
};
