#include "FontManager.h"
#include <DxLib.h>

FontManager::FontManager()
{
	// フォントの初期化
	for (int i = 0; i < FONT_ID_MAX; i++)
	{
		m_Fonts[i].fontType = static_cast<FontID>(i);
		m_Fonts[i].fontHandle = -1;
	}
}

FontManager::~FontManager()
{
}

void FontManager::Load(FontID id,int size)
{
	// 引数のIDに対応するフォントを作成して保存
	m_Fonts[id].fontType = id;
	m_Fonts[id].fontHandle = CreateFontToHandle(nullptr, size, -1, DX_FONTTYPE_ANTIALIASING_EDGE_16X16);
}

int FontManager::GetFont(FontID id)
{
	// 引数のIDに対応するフォントハンドルを返す
	return m_Fonts[id].fontHandle;
}


void FontManager::UnloadAll()
{
	// すべてのフォントハンドルを削除
	for (auto& f : m_Fonts) {
		DeleteFontToHandle(f.fontHandle);
	}
}
