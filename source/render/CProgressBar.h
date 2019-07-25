#pragma once
#include "CRGBA.h"
#include "CSprite2d.h"

enum eProgressSprites {
	BAR_OUTLINE,
	BAR_INSIDE,
	PROGRESS_TOTALSPRITES
};

class CProgressBar {
public:
	static bool ms_bSpritesLoaded;
	static CSprite2d Sprites[PROGRESS_TOTALSPRITES];

public:
	static void Initialise();
	static void Shutdown();
	static bool IsProgressBarRequired();
	static void DrawProgressBar(float x, float y, float width, float height, float progress, char border, char shadow, CRGBA colorFront, CRGBA colorBack);
	static void DrawProgressBarWithProgressDelta(float x, float y, float width, float height, float progress, CRGBA const &color, float deltaProgress, CRGBA const &deltaColor);
	static void DrawProgressBarWithSprites(float x, float y, float width, float height, float progress, CRGBA color);
};