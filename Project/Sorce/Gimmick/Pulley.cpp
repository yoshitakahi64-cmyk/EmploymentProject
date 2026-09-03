#include "Pulley.h"
#include "../Resource/MyResource.h"
#include "../Resource/MyResourceManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/Capsule.h"
#include "../Mechanism/IDManager.h"
#include "../Mechanism/MechanismBase.h"
#include "../Mechanism/PullObjectMechanism.h"
#include "../MyMath/MyMath.h"
#include "../PullObject/Ball.h"

static constexpr float PULLEY_HEIGHT = 1.0f;

Pulley::Pulley():
	m_IsPulling(false),
	m_TargetID(""),
	m_PulleyPos({}),
	m_TargetPos({}),
	m_RopeColor(0),
	m_RopeWidth(0.0f),
	m_Mechanism(nullptr),
	m_TargetGimmick(nullptr)
{
}

void Pulley::Load()
{
	// 木箱のモデルをロード
	SharedPtr pulleyModel = MyResourceManager::GetInstance()->LoadModelResource("Data/Gimmick/Pully/Pully.x", false, true);

	// モデルがロードできたらハンドルを取得
	if (pulleyModel)
	{
		m_Handle = pulleyModel->GetHandle();
	}

	// 茶色
	m_RopeColor = GetColor(120, 80, 40);
	// ロープの太さ
	m_RopeWidth = 6.0f;
}

void Pulley::OnStart()
{
	// ロープの位置を設定
	//m_Transform.position_3D = VGet(-40.0f, 10.0f, 20.0f);

	// 回転速度を設定
	m_RotSpeed = 180.0f;

	// 自分の位置を保持
	m_PulleyPos = m_Transform.position_3D;
	// モデルの半径を取得
	m_PulleyRadius = 0.5f;

	// カプセル形の当たり判定を設定
	m_CapsuleCollision = CollisionManager::GetInstance()->CreateCapsule();
	m_CapsuleCollision->SetTargetPos(&m_Transform.position_3D);
	m_CapsuleCollision->SetOffsetPos(VGet(0.0f, 0.0f, 0.0f));
	m_CapsuleCollision->SetHeight(PULLEY_HEIGHT);
	m_CapsuleCollision->SetRadius(1.0f);

	// ボックス形の当たり判定
	m_Collision = CollisionManager::GetInstance()->CreateAABB();
	m_Collision->SetTargetPos(&m_Transform.position_3D);
	m_Collision->SetOffsetPos(VGet(0.0f, 0.0f, 0.0f));
	m_Collision->SetSize(VGet(2.4f, 2.2f, 1.4f));

	// 重力無効
	m_IsGravityApplied = false;

	SetTypeName("Pulley");

	IDManager::GetInstance()->Register(this);

	m_TargetID = "Ball_001";
	
	m_Mechanism = new PullObjectMechanism();

	// メカニズム設定
	SettingMechanism();
	// ターゲットID設定
	m_Mechanism->SetTargetID(m_TargetID);
}

void Pulley::OnStep()
{
	OnPull();
}

UniquePtr<Gimmick> Pulley::Clone()
{
	// クローンを生成
	UniquePtr<Pulley> clone = MakeUnique<Pulley>();

	// 変数をコピー
	*clone = *this;

	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return std::move(clone);
}

VECTOR Pulley::GetPullDirection() const
{
	if (m_TargetGimmick == nullptr)
		return VGet(0.0f, 0.0f, 0.0f);

	// PullObject(=m_TargetGimmick) → Pulley のベクトルを求める
	VECTOR pulleyPos = m_Transform.position_3D;
	VECTOR objectPos = m_TargetGimmick->GetPos3D();

	// 方向（PullObjectからPulleyへのベクトル）
	VECTOR dir = VSub(pulleyPos, objectPos);

	// 正規化
	dir = VNorm(dir);

	return dir;
}

void Pulley::OnPulling()
{
	// 滑車起動中フラグオン
	m_IsPulling = true;

	PullObjectMechanism* pullMech = dynamic_cast<PullObjectMechanism*>(m_Mechanism);

	if (pullMech)
	{
		pullMech->SetPulley(this);
	}
}

void Pulley::OnPull()
{
	if (m_IsPulling)
	{
		m_Mechanism->Activate();
	}
}

void Pulley::OnStopPulling()
{
	// 滑車起動中フラグオフ
	m_IsPulling = false;
}

void Pulley::SettingMechanism()
{
	GameObject3D* obj = IDManager::GetInstance()->FindByID(m_TargetID);

	if (dynamic_cast<Ball*>(obj))
	{
		m_Mechanism = new PullObjectMechanism();
		m_TargetGimmick = obj;
	}
}

void Pulley::ControllAnimation()
{
	if (!m_TargetGimmick)return;

	// 端点A = 滑車の位置
	m_PulleyPos = m_Transform.position_3D;
	// 端点B = ターゲット位置
	m_TargetPos = m_TargetGimmick->GetPos3D();

	// 距離を測定
	float currentDist = MyMath::VecLong(MyMath::VecCreate(m_PulleyPos, m_TargetPos));

	if (m_PrevDistance != 0.0f)
	{
		// 縄の伸縮量
		float delta = currentDist - m_PrevDistance;
		// 回転角に変換
		float angleRad = delta / m_PulleyRadius;

		// 現在のモデルの向きに応じたワールド軸を取得
		VECTOR localAxis = VGet(0.0f, 0.0f, 1.0f);
		VECTOR worldAxis = MyQuaternion::QTransformVector(m_Transform.rotation, localAxis);

		// 回転クォータニオンを作成
		Quaternion deltaRot = MyQuaternion::QfromAxisAngle(worldAxis, angleRad);

		// モデルの回転に加算
		m_Transform.rotation = MyQuaternion::QMultiply(deltaRot, m_Transform.rotation);

	}

	// 前回の距離を更新
	m_PrevDistance = currentDist;

	// モデルの回転を適用


	// ロープを描画
	DrawLine3D(m_PulleyPos, m_TargetPos, m_RopeColor);
}

void Pulley::OnCollisionEnter()
{
	OnPulling();
}

void Pulley::OnCollisionExit()
{
	OnStopPulling();
}
