#include <DxLib.h>
#include "UIScore.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"
#include "../Score/ScoreManager.h"
#include "../Score/Score.h"
#include "../Font/FontManager.h"
#include "../Font/FontParam.h"

#define SCORE_COUNT_OFFSET_X 270
#define SCORE_COUNT_OFFSET_Y -5

#define SCORE_FRAME_OFFSET_X -25
#define SCORE_FRAME_OFFSET_Y -25

UIScore::UIScore():
	m_Score(nullptr),
	m_ScoreFrameHandle(0),
	m_Value(0)
{
}

UIScore::~UIScore()
{
}

void UIScore::Init()
{
}

void UIScore::Load()
{
	// 各画像をロード
	std::shared_ptr<MyResource> scoreImage = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/Score_new.png", false, true);

	std::shared_ptr<MyResource> scoreFrameImage = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/ScoreFrame_new.png", false, true);

	// ロードできたらハンドルを取得
	if (scoreImage)
	{
		m_Handle = scoreImage->GetHandle();
	}

	if (scoreFrameImage)
	{
		m_ScoreFrameHandle = scoreFrameImage->GetHandle();
	}
}

void UIScore::Start()
{
	// スコアクラスのポインタを取得
	m_Score = ScoreManager::GetInstance()->GetScore().get();
}

void UIScore::Update()
{
	// スコアの値を更新
	m_Value = m_Score->GetScore();
}

void UIScore::Draw()
{
	if (m_Hide) return;

	// スコアフレームを描画
	DrawGraph(m_Transform.posX_2D + SCORE_FRAME_OFFSET_X , m_Transform.posY_2D + SCORE_FRAME_OFFSET_Y, m_ScoreFrameHandle, true);
	// スコアの値を描画
	DrawGraph(m_Transform.posX_2D, m_Transform.posY_2D, m_Handle, true);
	// スコアの数値を描画
	DrawFormatStringToHandle(m_Transform.posX_2D + SCORE_COUNT_OFFSET_X, m_Transform.posY_2D + SCORE_COUNT_OFFSET_Y, 
		                     GetColor(255, 255, 255), FontManager::GetInstance()->GetFont(FONT_ID_SCORE), "%d", (int)m_Value);
}

void UIScore::Fin()
{
	DeleteGraph(m_Handle);
}

UniquePtr<UIBase> UIScore::Clone()
{
	UniquePtr<UIScore> clone = MakeUnique<UIScore>();
	*clone = *this;

	return std::move(clone);
}
