#pragma once

#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"
#include "Bullet.h"

// 弾の管理クラス
class BulletManager : public Singleton<BulletManager>
{
public:
	BulletManager(); // コンストラクタ
	~BulletManager(); // デストラクタ

public:
	void Init();   // 初期化関数
	void Load();   // ロード関数
	void Start();  // スタート関数
	void Step();   // ステップ関数
	void Update(); // アップデート関数
	void Draw();   // 描画関数
	void Fin();     // 終了関数

public:
	// 弾の生成関数
	Bullet* CreateBullet(BULLETOWNER_TYPE owner);
	// 弾のリストを返す関数
	const List<UniquePtr<Bullet>>& GetBulletList() const { return m_BulletList; }

private:
	// クローン元のバレット
	UniquePtr<Bullet> m_OriginalBullet[BULLET_OWNER_MAX];
	// 敵のリスト
	List<UniquePtr<Bullet>> m_BulletList;
};
