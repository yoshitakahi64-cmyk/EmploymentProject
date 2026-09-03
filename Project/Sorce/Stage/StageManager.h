#pragma once

#include "../Singleton.h"
#include "../Container.h"

struct GameObject_Unity;

// ステージ管理クラス
class StageManager : public Singleton<StageManager>
{
public:
	StageManager(); // コンストラクタ
	~StageManager(); // デストラクタ

	void Load(const char* fileName); // ロード関数
	void Start(); // スタート関数
	void Fin(); // 終了関数

private:
	// ステージオブジェクトのリスト
	Vector<GameObject_Unity> m_Objects;
};
