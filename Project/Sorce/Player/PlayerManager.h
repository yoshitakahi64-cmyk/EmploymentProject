#pragma once
#include"../Singleton.h"
#include "../Memory.h"
#include <DxLib.h>
#include "../Quatrenion/Quatrenion.h"


class Player;

// プレイヤー管理クラス
class PlayerManager :public Singleton<PlayerManager>
{
public:
	PlayerManager(); // コンストラクタ
	~PlayerManager(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Load(); // ロード関数
	void Start(); // スタート関数
	void Step(); // ステップ関数
	void Update(); // 更新関数
	void Draw(); // 描画関数
	void Fin(); // 終了関数

public:
	// プレイヤー生成関数
	void CreatePlayer(VECTOR pos, Quaternion rot, VECTOR scale);
	// プレイヤー取得関数
	const UniquePtr<Player>& GetPlayer() const { return  m_Player; }

private:
	// プレイヤーのインスタンス
	UniquePtr<Player> m_Player;
};
