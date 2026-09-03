#include "UIBase3D.h"

UIBase3D::UIBase3D():
	m_Hide(false)
{
}

UIBase3D::~UIBase3D()
{
}

void UIBase3D::Init()
{
}

void UIBase3D::Load()
{
}

void UIBase3D::Start()
{
}

void UIBase3D::Step()
{
}

void UIBase3D::Update()
{
	// 位置プロバイダーが設定されている場合は、位置を更新
	if (m_PositionProvider)
	{
		// 位置プロバイダーから新しい位置を取得して更新
		m_Pos = m_PositionProvider();
	}
}

void UIBase3D::Draw()
{
}

void UIBase3D::Fin()
{
}
