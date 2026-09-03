#include "UIEnemyHP.h"
#include "../MyMath/MyMath.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"
#include "../Input/Input.h"

UIEnemyHP::UIEnemyHP():
	m_EnemyControlStateProvider(nullptr),
	m_EnemyHPProvider(nullptr),
	m_HPCursor_GHandle(0),
	m_HPCursor_RHandle(0),
	m_HPCursor_YHandle(0),
	m_YButtonHandle(0),
	m_QKeyHandle(0),
	m_Value(0),
	m_MaxValue(0)
{
}

UIEnemyHP::~UIEnemyHP()
{
}

void UIEnemyHP::Init()
{
}

void UIEnemyHP::Load()
{
	// 各画像をロードしてハンドルを取得 ---
	SharedPtr handle = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/EnemyHPUI_G.png", false, true);
	if (handle)
	{
		m_HPCursor_GHandle = handle->GetHandle();
	}

	SharedPtr handle2 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/EnemyHPUI_R.png", false, true);
	if (handle2)
	{
		m_HPCursor_RHandle = handle2->GetHandle();
	}

	SharedPtr handle4 = MyResourceManager::GetInstance()->LoadImageResource("Data/UI/EnemyHPUI_Y.png", false, true);
	if (handle4)
	{
		m_HPCursor_YHandle = handle4->GetHandle();
	}
	//--------------------------------------
}

void UIEnemyHP::Start()
{
}

void UIEnemyHP::Update()
{
	// 座標の更新など、UIBase3Dの基本的な更新処理を呼び出す
	UIBase3D::Update();

	// 敵のHPデータを提供する関数が設定されている場合は、HPデータを取得して更新
	if (m_EnemyHPProvider)
	{
		EnemyHPData hpData = m_EnemyHPProvider();
		m_Value = hpData.value;
		m_MaxValue = hpData.maxValue;
	}

	// 敵の操作状態を提供する関数が設定されている場合は、操作状態を取得して表示の切り替えなどに利用
	if (m_EnemyControlStateProvider)
	{
		bool isControlling = m_EnemyControlStateProvider();
		
		// 敵が操作されている状態ならUIを非表示にし、そうでない場合は表示する
		if(isControlling)
		{
			Hide();
		}
	}
}

void UIEnemyHP::Draw()
{
	if (m_Hide)return;

	// HPの割合を計算
	float hpRate = MyMath::Clamp(m_Value / m_MaxValue, 0.0f, 1.0f);

	// HPの割合に応じて、表示する画像を切り替える
	int handleToDraw = m_HPCursor_GHandle; // デフォルトは緑

	if (hpRate > 0.75f)
	{
		handleToDraw = m_HPCursor_GHandle; // 緑
	}
	else if (hpRate > 0.25f)
	{
		handleToDraw = m_HPCursor_YHandle; // 黄色
	}
	else
	{
		handleToDraw = m_HPCursor_RHandle; // 赤色
	}

	// HPが0以下のときは、操作状態に応じてYボタンかQキーの画像を表示する
	if (hpRate >= 0.0f)
	{
		// 画像を描画
		DrawBillboard3D(m_Pos, 0.5f, 0.5f, 0.3f, 0.0f, handleToDraw, true);
	}
}

void UIEnemyHP::Fin()
{
}

UniquePtr<UIBase3D> UIEnemyHP::Clone()
{
	UniquePtr<UIEnemyHP> clone = MakeUnique<UIEnemyHP>();
	*clone = *this;

	return std::move(clone);
}
