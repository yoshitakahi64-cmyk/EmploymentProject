#pragma once

#include "../Singleton.h"
#include "../Container.h"

class Enemy;
class Player;

// ターゲット管理クラス
class Target
{
public:
	Target(); // コンストラクタ
	~Target() = default; // デストラクタ

public:
	void Update(); // 更新関数

public:
	// 現在のターゲット敵取得関数
	Enemy* GetCurrentTargetEnemy() { return m_CurrentTargetEnemy; }
	// ターゲット可能判定関数
	bool IsEnableTarget() { return m_EnableTargetList.size() > 0; }
	// ターゲット開始関数
	void StartTargetEnemy();
	// ターゲット変更関数
	void ChangeTargetEnemy(int delta);
	// ターゲットクリア関数
	void ClearTarget(bool isCompulsion);

	// ターゲット強制解除が行われたかフラグ
	bool IsCompulsionTargeting(){ return m_IsCompulsionTargeting; }

	// ターゲット中判定フラグ
	bool IsTargeting() { return m_IsTargeting; }

private:
	// ターゲット可能な敵を検索する関数
	void SearchEnableTarget();
	// ターゲット可能な敵を距離順にソートする関数
	void SortNearlyTarget();

private:
	// ターゲット可能な敵のリスト
	List<Enemy*> m_EnableTargetList;
	// 現在のターゲット敵
	Enemy* m_CurrentTargetEnemy;
	// 現在の順番
	int m_CurrentIndex;
	// ターゲット強制解除フラグ
	bool m_IsCompulsionTargeting;
	// ターゲット中フラグ
	bool m_IsTargeting;
};
