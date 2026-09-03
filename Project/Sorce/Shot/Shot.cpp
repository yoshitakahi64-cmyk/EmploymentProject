#include "Shot.h"
#include "../Bullet/BulletManager.h"
#include "../MyMath/MyMath.h"
#include "../Quatrenion/Quatrenion.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"

Shot::Shot()
{

}

void Shot::Fire(VECTOR pos, VECTOR front, BULLETOWNER_TYPE owner,float speed)
{
	// 移動量を計算
	VECTOR move = MyMath::VecScale(front, speed);

	// 弾の生成
	Bullet* bullet = BulletManager::GetInstance()->CreateBullet(owner);
	bullet->SetActive(true);
	bullet->SetPos(pos);
	bullet->SetMove(move);
	bullet->SetOwner(owner);
	bullet->BulletSetting(front, owner);	

	// 弾の寿命をリセット
	bullet->ResetLife();
}
