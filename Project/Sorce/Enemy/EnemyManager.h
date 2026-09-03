#pragma once
#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"
#include <DxLib.h>
#include "../Quatrenion/Quatrenion.h"

class Enemy;

// 敵の種類
enum EnemyType
{
	RUSH_ENEMY,
	SHOT_ENEMY,
	ENEMY_TYPE_MAX,
	ENEMY_TYPE_NONE = -1
};

// 敵管理クラス
class EnemyManager :public Singleton<EnemyManager>
{
public:
	EnemyManager(); // コンストラクタ
	~EnemyManager(); // デストラクタ

public:
	void Init();   // 初期化関数
	void Load();   // ロード関数
	void Start();  // スタート関数
	void Step();   // ステップ関数
	void Update(); // アップデート関数
	void Draw();   // ドロー関数
	void Fin();     // 終了関数

public:
	// 敵の生成関数
	void CreateEnemy(EnemyType type,VECTOR pos,Quaternion rot,VECTOR scale);
	// 敵のリストの取得関数
	const List<UniquePtr<Enemy>>& GetEnemyList() const {return m_Enemy; }

private:
	// クローン元の敵を管理する配列
	UniquePtr<Enemy> m_OriginalEnemy[ENEMY_TYPE_MAX];
	// 敵のリスト
	List<UniquePtr<Enemy>> m_Enemy;
};