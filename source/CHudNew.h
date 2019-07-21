#pragma once
#define BLANK NULL
#define III_VC_SHADOW_SIZE (2 * s.m_fHudW)
#define SA_EDGE_SIZE (2)
#define AREA_NAME_TIME 5000
#define VEHICLE_NAME_TIME 5000

#include "CSprite2d.h"

enum eGameMode {
	GAMEMODE_NULL = -1,
	GAMEMODE_III,
	GAMEMODE_VC,
	GAMEMODE_SA,
	GAMEMODE_LCS,
	GAMEMODE_VCS,
	GAMEMODE_ADVANCE,
	GAMEMODE_TOTALMODES
};

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
	static eGameMode ms_nGameMode;
	static bool ms_bSpritesLoaded;
	static char *ms_nGamePrefix;
	static bool ms_bReload;

public:
	static void Initialise();
	static void Shutdown();
	static void ReInitialise();
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
	static void DrawTextBox();
	static void DrawProgressBar(float x, float y, float width, float height, float progress, char border, char shadow, CRGBA colorFront, CRGBA colorBack);

public:
	static eGameMode GetGameMode() { return ms_nGameMode; }
	static void SetGameMode(eGameMode Mode) { ms_nGameMode = Mode; }
	static char *GetGamePrefix() { return ms_nGamePrefix; }

public:
	static void InjectPatches();
};