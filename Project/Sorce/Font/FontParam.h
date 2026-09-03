#pragma once

// フォントのID
enum FontID
{
	FONT_ID_TITLESELECT,
	FONT_ID_SCORE,
	FONT_ID_MANUAL,
	FONT_ID_MAX
};

// フォント構造体
struct Font
{
	FontID fontType;
	int fontHandle;
};