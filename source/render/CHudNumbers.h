#pragma once
#include "CVector2D.h"
#include "CSprite2d.h"

enum eHudNumbersSprites {
	HUDNUMBERS,
	HUDNUMBERS_TOTALSPRITES
};

class CHudNumbers {
public:
	static CSprite2d Sprite[HUDNUMBERS_TOTALSPRITES];
	static CVector2D m_vecFontScale;
	static CRGBA m_Color;
	static int m_nFontStyle;
	static bool ms_bHudNumbersLoaded;

public:
	CHudNumbers();

	static void Initialise();
	static void Shutdown();
	static bool IsHudNumbersRequired();

	static void SetFontScale(float w, float h);
	static void SetColor(CRGBA &color);
	static void SetFontStyle(int style);
	static void PrintString(float x, float y, std::string text);
};