#pragma once
#include "../GameObject/3DGameObject.h"
#include "../Memory.h"

class CollisionAABB;
class Player;

// ステージオブジェクトのモード
enum StageObjectMode
{
	OBJ_MODE_NORMAL,
	OBJ_MODE_GRAVITY,
	OBJ_MODE_NONE = -1,
};

// ステージオブジェクトの基底クラス
class StageObject : public GameObject3D
{
public:
	StageObject(); // コンストラクタ
	~StageObject(); // デストラクタ
public:
	virtual void Load(); // ロード関数
	void Start()override; // スタート関数
	virtual void OnStart() {}; // 継承先スタート関数
	void Step()override; // ステップ関数

	// クローン関数
	virtual  UniquePtr<StageObject> Clone() { return nullptr; }

	// ステージとプレイヤーとの当たった場所を判定
	bool HitWeherFace(const Player& player);
	// モード設定関数
	void SetMode(StageObjectMode mode) { m_Mode = mode; }

	// ゴール
	void FinishStage();

public:
	// ゴール判定フラグ
	bool m_Goal;

private:
	// 重力操作モード更新関数
	void GravityModeUpdate()override;

private:
	// モード
	StageObjectMode m_Mode;	

	
};
