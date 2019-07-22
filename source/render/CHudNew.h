#pragma once
#define BLANK NULL
#define III_VC_SHADOW_SIZE (2 * s.m_fHudW)
#define SA_EDGE_SIZE (2)
#define AREA_NAME_TIME 10000
#define VEHICLE_NAME_TIME 10000

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
	static void DrawHelpText(float x, float y, float w, float h);
	static void DrawSubtitles(float x, float y, float w, float h);
	static void DrawProgressBar(float x, float y, float width, float height, float progress, char border, char shadow, CRGBA colorFront, CRGBA colorBack);
};