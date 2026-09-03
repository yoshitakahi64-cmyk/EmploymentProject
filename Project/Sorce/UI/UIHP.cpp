#include <DxLib.h>
#include "UIHP.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"

static constexpr int HP_GAUGE_X = 30;
static constexpr int HP_GAUGE_Y = 60;

UIHP::UIHP():
	m_GaugeHeight(0),
	m_GaugeWidth(0),
	m_Value(0),
	m_MaxValue(0),
	m_Player(nullptr)
{
}

UIHP::~UIHP()
{
	Fin();
}

void UIHP::Init()
{
}

void UIHP::Load()
{
	// HPゲージの画像をロード
	std::shared_ptr<MyResource> hpGaugeImage = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/HPFrame.png", false,true);

	// 画像がロードできたらハンドルを取得
	if (hpGaugeImage)
	{
		m_Handle = hpGaugeImage->GetHandle();
	}
}

void UIHP::Start()
{
	// プレイヤーを参照
	m_Player = PlayerManager::GetInstance()->GetPlayer().get();

	// HPの最大値をプレイヤーのHPに設定
	m_MaxValue = m_Player->GetHP();

	// HPゲージのサイズを設定
	m_GaugeHeight = 35;
	m_GaugeWidth = 480;
}

void UIHP::Update()
{
	// プレイヤーのHPを取得して値を更新
	m_Value = m_Player->GetHP();
}

void UIHP::Draw()
{
	if(m_Hide) return;

	// HPゲージの割合を計算
	float gaugeRate = MyMath::Clamp((float)m_Value / (float)m_MaxValue, 0.0f, 1.0f);
	// ゲージの幅を割合に応じて調整
	int gaugeWidth = (int)(m_GaugeWidth * gaugeRate);

	// ゲージの中身をHPの割合に応じて描画
	if (gaugeRate >= 0.5f)
	{
		DrawBox(HP_GAUGE_X, HP_GAUGE_Y, HP_GAUGE_X + gaugeWidth, HP_GAUGE_Y + m_GaugeHeight, GetColor(0, 255, 0), true);
	}
	// HPが半分以下のときは黄色で描画
	else if(gaugeRate >= 0.25f)
	{
		DrawBox(HP_GAUGE_X, HP_GAUGE_Y, HP_GAUGE_X + gaugeWidth, HP_GAUGE_Y + m_GaugeHeight, GetColor(255, 255, 0), true);
	}
	// HPが25%以下のときは赤で描画
	else
	{
		DrawBox(HP_GAUGE_X, HP_GAUGE_Y, HP_GAUGE_X + gaugeWidth, HP_GAUGE_Y + m_GaugeHeight, GetColor(255, 0, 0), true);
	}

	// ゲージのフレームを描画
	DrawGraph((int)(m_Transform.posX_2D), (int)(m_Transform.posY_2D), m_Handle, true);
}

void UIHP::Fin()
{
}

UniquePtr<UIBase> UIHP::Clone()
{
	UniquePtr<UIHP> clone = MakeUnique<UIHP>();
	*clone = *this;

	return std::move(clone);
}

