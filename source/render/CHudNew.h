#pragma once
#define BLANK NULL
#define HUD_SHADOW_SIZE (2)
#define SUBS_SHADOW_SIZE (2)
#define EDGE_SIZE (2)
#define AREA_NAME_TIME 3000
#define VEHICLE_NAME_TIME 3000

#include "CSprite2d.h"

enum eHudSpritesNew {
	HUD_UNARMED,
	HUD_TOTALSPRITES
};

enum eSharedSprites {
	SHARED_BREATHICON,
	SHARED_SKIPICON,
	SHARED_TOTALSPRITES
};

class CHudNew {
public:
	static CSprite2d SpritesNew[HUD_TOTALSPRITES];
	static CSprite2d SharedSprites[SHARED_TOTALSPRITES];
	static bool ms_bSpritesLoaded;

public:
	CHudNew();

	static void Initialise();
	static void Shutdown();
	static void Draw();
	static void DrawAfterFade();

private:
	static void DrawClock(float x, float y, float w, float h);
	static void DrawMoneyCounter(float x, float y, float w, float h);
	static void DrawHealth(int PlayerID, float x, float y, float w, float h);
	static void DrawArmour(int PlayerID, float x, float y, float w, float h);
	static void DrawBreath(int PlayerID, float x, float y, float w, float h);
	static void DrawWeaponIcon(int PlayerID, float x, float y, float w, float h);
	static void DrawAmmo(int PlayerID, float x, float y, float w, float h);
	static void DrawWanted(float x, float y, float w, float h);
	static void DrawStatsBox(float x, float y, float w, float h);
	static void DrawAreaName(float x, float y, float w, float h);
	static void DrawVehicleName(float x, float y, float w, float h);
	static void DrawRadioStation(float x, float y, float w, float h);
	static void PrintHelpText(float x, float y, char * str, float alpha);
	static void DrawHelpText(float x, float y, float w, float h);
	static void DrawSubtitles(float x, float y, float w, float h);
	static void DrawWastedBustedMessage(float x, float y, float w, float h);
	static void DrawMissionTitle(float x, float y, float w, float h);
	static void DrawSuccessFailedMessage(float x, float y, float w, float h);
	static void DrawOddJobMessage(float x, float y, float w, float h);

public:
	static void DrawProgressBar(float x, float y, float width, float height, float progress, char border, char shadow, CRGBA colorFront, CRGBA colorBack);
	static void DrawProgressBarWithProgressDelta(float x, float y, float width, float height, float progress, CRGBA const & color, float deltaProgress, CRGBA const & deltaColor);
};