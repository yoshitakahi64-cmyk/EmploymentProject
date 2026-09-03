#pragma once

#include "FontParam.h"
#include "../Singleton.h"
#include <string>

// フォント管理クラス
class FontManager : public Singleton<FontManager>
{
public:
	FontManager(); // コンストラクタ
	~FontManager(); // デストラクタ

public:
	// フォントのロード、取得、アンロード関数
	void Load(FontID id,int size);
	int GetFont(FontID id);
	void UnloadAll();

private:
	// フォントの配列
	Font m_Fonts[FONT_ID_MAX];
};