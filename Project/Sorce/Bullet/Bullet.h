#pragma once
#include "../GameObject/3DGameObject.h"
#include "../Memory.h"

// 弾の生存時間
static constexpr float ACTIVE_TIME = 60.0f;

// 弾の発射主の種類
enum BULLETOWNER_TYPE
{
	PLAYER,
	ENEMY,
	BULLET_OWNER_MAX,
	NONE = -1,
};

// 弾のクラス
class Bullet : public GameObject3D
{
public:
	Bullet(); // コンストラクタ
	~Bullet(); // デストラクタ

public:
	virtual void Load() {}; // ロード関数
	void Start()override;   // スタート関数
	void Step()override;    // ステップ関数

	// クローン関数
	virtual UniquePtr<Bullet> Clone() { return nullptr; }

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Bullet"; }
	void SetPos(VECTOR pos) { m_Transform.position_3D = pos; }
	void SetMove(VECTOR move) { m_Move = move; }
	void SetRotation(Quaternion rot) { m_Transform.rotation = rot; }
	void SetOwner(BULLETOWNER_TYPE owner) { m_Owner = owner; }
	BULLETOWNER_TYPE GetOwner() const { return m_Owner; }

public:
	// 当たり判定関数
	virtual void Hit() = 0;
	// ステージオブジェクトに当たったときの処理関数
	void HitStageObject();
	// 弾の設定関数
	void BulletSetting(VECTOR front, BULLETOWNER_TYPE owner);

	// 弾の生存時間リセット関数
	void ResetLife() { m_Life = ACTIVE_TIME; }

protected:
	// 生存時間
	float m_Life;

private:
// 弾の発射主
	BULLETOWNER_TYPE m_Owner;
};
