#include "3DGameObject.h"
#include "../MyMath/MyMath.h"
#include "../Quatrenion/Quatrenion.h"
#include "../Collision/CollisionBase.h"
#include "../Collision/Capsule.h"
#include "../Collision/CollisionStage.h"
#include "../MyEffekseer/EffekseerManager.h"
#include <math.h>

#include <DxLib.h>

static constexpr float GRAVITY = 1.0f;
static constexpr int HITATTACKFRAME_INTERVAL = 4;
static constexpr float RAY_LONG = 100.0f;
static constexpr int RAY_MAX = 8;
static constexpr int RAYSTART_MAX = 3;

GameObject3D::GameObject3D():
	m_HP(0),
	m_MaxHP(0),
	m_HitAttackFrame(0),
	m_ID(""),
	m_TypeName(""),
	m_EnableFloorCheck(false),
	m_IsHitWall(false),
    m_IsActive(false),
    m_IsJump(false),
    m_IsStartChangeGravity(false),
	m_IsHitAttack(false),
	m_PrevPos(0.0f, 0.0f, 0.0f),
	m_Move(0.0f, 0.0f, 0.0f),
	m_GravityDir(0.0f, -1.0f, 0.0f),
	m_Up(0.0f, 1.0f, 0.0f),
	m_Forward(0.0f, 0.0f, 1.0f),
	m_Right(1.0f, 0.0f, 0.0f),
	m_AnimationAttachIndex(0),
	m_AnimationSpeed(1.0f),
	m_AnimationTotalTime(0.0f),
	m_AnimationNowTime(0.0f),
	m_IsLoopAnimation(false),
	m_NowAnimation(-1),
	m_Collision(nullptr),
	m_TargetCollision(nullptr),
	m_CapsuleCollision(nullptr),
	m_GravityCommand({})
{	
}

GameObject3D::~GameObject3D()
{
	Fin();
}

void GameObject3D::Update()
{
	// スケール行列
	MATRIX scaleMat = MyMath::MatScale(m_Transform.scale);
	// 回転行列
	MATRIX rotMat = MyQuaternion::QToMatrix(m_Transform.rotation);
	// 平行移動行列
	MATRIX transMat = MyMath::MatTranslation(m_Transform.position_3D);

	// 行列を合成
	MATRIX worldMat = MMult(transMat,rotMat);
	worldMat = MMult(worldMat, scaleMat);
	worldMat = MyMath::MatTransposition(worldMat);

	// モデルに行列をセット
	MV1SetMatrix(m_Handle,worldMat);

	// アニメーションを更新する
	UpdateAnimation();

	// 当たり判定と同期
	SyncCollision();

	// 無敵時間の点滅時間を減らしていく
	if (m_IsHitAttack)
	{
		m_HitAttackFrame--;
		if (m_HitAttackFrame < 0)
		{
			m_HitAttackFrame = 0;
			m_IsHitAttack = false;
		}
	}
}

void GameObject3D::Draw()
{
	// 生存フラグがオンのときだけ描画する
	if (m_IsActive)
	{
		if (m_IsHitAttack)
		{
			if (((m_HitAttackFrame / HITATTACKFRAME_INTERVAL) & 2) == 0)
			{
				MV1DrawModel(m_Handle);
			}
		}
		else
		{
			MV1DrawModel(m_Handle);
		}
		
	}
}

void GameObject3D::Fin()
{

}

void GameObject3D::CheckStageCollision()
{
	// 床との当たり判定可能かチェック
	if (!m_EnableFloorCheck)
		return;

	// ステージとの当たり判定を取得
	MV1_COLL_RESULT_POLY_DIM hitResult = CollisionStage::GetInstance()->StageCollision(m_CapsuleCollision);

	// 当たっていなかったら何もしない
	if (hitResult.HitNum == 0) return;

	// 床に当たっているかどうかのフラグと、床までの距離
	bool onFloor = false;
	float bestFloorDist = FLT_MAX;

	// 壁からの押し出し量をまとめて集計する変数
	VECTOR totalPush = VGet(0.0f, 0.0f, 0.0f);

	// 壁に当たっているかどうかのフラグをリセット
	m_IsHitWall = false;

	// ヒットしたポリゴンすべてに対して処理
	for (int i = 0; i < hitResult.HitNum; i++)
	{
		// ヒットしたポリゴンの法線を取得
		MV1_COLL_RESULT_POLY polygon = hitResult.Dim[i];
		VECTOR normal = polygon.Normal;

		// 法線と重力方向の逆のベクトルの内積を計算
		float dot = MyMath::VecDot(normal, MyMath::VecScale(m_GravityDir, -1.0f));

		// 内積が0.7以上なら床とみなす
		if (dot > 0.7f)
		{
			// 床に対するレイの始点と終点を計算
			VECTOR lineStart = m_CapsuleCollision->GetRotatedWorldPos();
			VECTOR lineEnd = VAdd(lineStart, MyMath::VecScale(m_GravityDir, RAY_LONG));

			// レイとポリゴンの当たり判定を行う
			HITRESULT_LINE lineResult = HitCheck_Line_Triangle(lineStart, lineEnd,
				polygon.Position[0], polygon.Position[1], polygon.Position[2]);

			// 当たっていたら床にいるとみなす
			if (lineResult.HitFlag)
			{
				// 床までの距離を計算
				float dist = MyMath::VecDot(MyMath::VecCreate(
					m_CapsuleCollision->GetLowestPos(), lineResult.Position), m_GravityDir);

				// 距離は正の値にする
				if (dist < 0) dist *= -1.0f;

				// 最も近い床までの距離を更新
				if (dist < bestFloorDist)
				{
					bestFloorDist = dist;
					onFloor = true;
				}
			}
		}
		// 内積が0.7未満なら壁とみなす
		else
		{
			// このポリゴンに対する最大めり込み量
			float maxOverlapThisPolygon = 0.0f;
			bool hitThisPolygon = false;

			// レイの始点を計算
			VECTOR lineStart_center = m_CapsuleCollision->GetRotatedWorldPos();
			VECTOR lineStart_Head = m_CapsuleCollision->GetRotatedTopPos();
			VECTOR lineStart_Foot = m_CapsuleCollision->GetRotatedBottomPos();

			// カプセル形状コリジョンの半径を取得
			float radius = m_CapsuleCollision->GetRadius();

			// レイの方向を8方向に設定（X軸正負、Z軸正負、斜め方向・正規化済み）
			const VECTOR directions[RAY_MAX] = {
				{ 1.0f, 0.0f, 0.0f },
				{ -1.0f, 0.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 0.0f, 0.0f, -1.0f },
				{ 0.7071068f, 0.0f, 0.7071068f },
				{ -0.7071068f, 0.0f, 0.7071068f },
				{ 0.7071068f, 0.0f, -0.7071068f },
				{ -0.7071068f, 0.0f, -0.7071068f }
			};

			VECTOR lineStarts[RAYSTART_MAX] = { lineStart_Head, lineStart_center, lineStart_Foot };

			// 8方向×3点にレイを飛ばして壁との当たり判定を行う
			for (int dirIdx = 0; dirIdx < RAY_MAX; dirIdx++)
			{
				for (int j = 0; j < RAYSTART_MAX; j++)
				{
					// レイの終点を計算
					VECTOR lineEnd = MyMath::VecAdd(lineStarts[j], MyMath::VecScale(directions[dirIdx], radius));

					// レイとポリゴンの当たり判定を行う
					HITRESULT_LINE lineResult = HitCheck_Line_Triangle(lineStarts[j], lineEnd,
						polygon.Position[0], polygon.Position[1], polygon.Position[2]);

					// 当たっていなかったら次の方向のレイを飛ばす
					if (!lineResult.HitFlag) continue;

					// 法線方向でのめり込み量を計算
					float planeDist = MyMath::VecDot(polygon.Normal,
						MyMath::VecSub(lineStarts[j], polygon.Position[0]));

					float overlap = radius - planeDist;

					// めり込みがない場合は次の方向のレイを飛ばす
					if (overlap <= 0.0f) continue;

					// このポリゴンの中で一番深いめり込み量だけを採用する
					if (overlap > maxOverlapThisPolygon)
					{
						maxOverlapThisPolygon = overlap;
					}
					hitThisPolygon = true;
				}
			}

			// このポリゴンにつき1回だけtotalPushに集計する
			if (hitThisPolygon)
			{
				VECTOR horizontalNormal = polygon.Normal;
				horizontalNormal.y = 0.0f;
				horizontalNormal = MyMath::VecNormalize(horizontalNormal);

				float alreadyResolved = MyMath::VecDot(totalPush, horizontalNormal);
				float remaining = maxOverlapThisPolygon - alreadyResolved;

				if (remaining > 0.0f)
				{
					VECTOR pushVec = MyMath::VecScale(horizontalNormal, remaining);
					totalPush = MyMath::VecAdd(totalPush, pushVec);
				}

				m_IsHitWall = true;
			}
		}
	}

	MV1CollResultPolyDimTerminate(hitResult);

	// 壁の押し出しをまとめて反映する
	if (m_IsHitWall)
	{
		m_Transform.position_3D = MyMath::VecAdd(m_Transform.position_3D, totalPush);
		m_Move = MyMath::VecAdd(m_Move, totalPush);
	}

	// 床に当たっていてジャンプしていないときは、床から押し出す
	if (onFloor && m_Move.y < 0.0f)
	{
		// 床から押し出す
		m_Transform.position_3D = VAdd(m_Transform.position_3D,
			MyMath::VecScale(m_GravityDir, bestFloorDist * -1.0f));

		// 床に押し出された分だけ移動量もリセットする
		float dot = MyMath::VecDot(m_Move, m_GravityDir);
		if (dot > 0.0f)
		{
			m_Move = MyMath::VecCreate(m_Move, MyMath::VecScale(m_GravityDir, dot));
		}
	}

	// 押し出し処理を行った後の位置をモデルに反映する
	MV1SetPosition(m_Handle, m_Transform.position_3D);
}

// 他オブジェクトとの押し出し処理
void GameObject3D::HitBodyAnyObject(CollisionResult* result)
{
	// 当たっていなかったら何もしない
	if (!result->isHit) return;

	// 押し出し処理
	VECTOR pushVec = VScale(result->hitNormal, result->panetration);

	// 押し出すベクトルを反映
	m_Transform.position_3D = VSub(m_Transform.position_3D, pushVec);
}

void GameObject3D::UpdateCollision(bool isCollidingNow)
{
	//　当たった瞬間
	if (isCollidingNow && !m_WasColliding)OnCollisionEnter();
	//　離れた瞬間
	else if (!isCollidingNow && m_WasColliding)OnCollisionExit();
	// 衝突情報を更新
	m_WasColliding = isCollidingNow;
}

bool GameObject3D::IsGrounded()
{
	if (m_Move.y > 0.0f) return false;

	// ステージとの当たり判定を取得
	MV1_COLL_RESULT_POLY_DIM hitResult = CollisionStage::GetInstance()->StageCollision(m_CapsuleCollision);

	for (int i = 0; i < hitResult.HitNum; i++)
	{
		// ヒットしたポリゴンの法線を取得
		VECTOR normal = hitResult.Dim[i].Normal;
		// 法線と重力方向の逆のベクトルの内積を計算
		float dot = MyMath::VecDot(normal, MyMath::VecScale(m_GravityDir, -1.0f));
		// 内積が0.7以上なら接地中とみなす
		if (dot > 0.7f) return true;
	}
	// どのポリゴンとも床とみなせる条件を満たさなかったら接地していないとみなす
	return false;
}

void GameObject3D::ApplyGravityCommand(GravityControlCommand cmd)
{
	// コマンドを保存
	m_GravityCommand = cmd;
}

void GameObject3D::HitDamage(int damage)
{
	// ダメージをHPから引く
	m_HP -= damage;
}

void GameObject3D::SetFrame(int animIndex, bool isLoop)
{
	// 再生中のアニメーションで呼ばれた場合は何もしない
	if (animIndex == m_NowAnimation) return;

	// 再生したアニメーションはデタッチする
	MV1DetachAnim(m_Handle, m_AnimationAttachIndex);

	// アニメーションをアタッチする
	m_AnimationAttachIndex = MV1AttachAnim(m_Handle, animIndex);

	// アニメーションのトータル時間を取得
	m_AnimationTotalTime = MV1GetAttachAnimTotalTime(m_Handle, m_AnimationAttachIndex);

	// 現在の再生時間を0にする
	m_AnimationNowTime = 0.0f;

	// ループ設定
	m_IsLoopAnimation = isLoop;

	// 再生中のアニメーション設定
	m_NowAnimation = animIndex;
}

void GameObject3D::GravityModeUpdate()
{
	// 重力操作して発射されているときは、重力操作コマンドの力を移動量に加算する
	if (m_GravityCommand.IsShot)
	{
		VECTOR force = MyMath::VecScale(m_GravityCommand.ForceDir, m_GravityCommand.ForcePower);
		m_Move = MyMath::VecAdd(m_Move, force);
	}
	// 重力操作していないときは、通常の力を移動量とする
	else
	{
		VECTOR force = MyMath::VecScale(m_GravityCommand.ForceDir, m_GravityCommand.ForcePower);
		m_Move = force;
	}
}

// 他オブジェクトとの当たり判定
bool GameObject3D::CheckCollision(GameObject3D* other) const
{
	// 衝突結果を保存する構造体
	CollisionResult result;

	// どちらかの当たり判定が存在しない場合は当たっていないとみなす
	if (!m_Collision || !other || !other->GetCollision()) return false;
	// 当たり判定をチェック
	m_Collision->CheckCollision(other->GetCollision(),result);
	return result.isHit;
}

void GameObject3D::UpdateAnimation()
{
	// アニメーションの再生時間を設定
	MV1SetAttachAnimTime(m_Handle, m_AnimationAttachIndex, m_AnimationNowTime);

	// 再生時間を進める
	m_AnimationNowTime += m_AnimationSpeed;

	// 末尾まで再生したか
	if (m_AnimationNowTime > m_AnimationTotalTime)
	{
		// ループ設定なら冒頭へ戻す
		if (m_IsLoopAnimation)
		{	
			m_AnimationNowTime = 0.0f;
		}
		// ループでなければ
		else
		{
			m_AnimationNowTime = m_AnimationTotalTime;
		}
	}
}
