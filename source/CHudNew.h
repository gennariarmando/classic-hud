#pragma once
#define BLANK NULL
#define HUD_SHADOW_SIZE 2
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

enum eSpritesMod {
	HUD_UNARMED = 0,
	HUD_DILDO1 = 1, // 321
	HUD_DILDO2 = 2,
	HUD_VIBE1 = 3,
	HUD_VIBE2 = 4,
	HUD_FLOWERS = 5,
	HUD_CANE = 6,
	HUD_BOXWEE = 7,
	HUD_BOXBIG = 8,
	HUD_CELL = 10,
	HUD_BRASSKNUCKLE = 11,
	HUD_GOLFCLUB = 13,
	HUD_NIGHTSTICK = 14,
	HUD_KNIFE = 15,
	HUD_BASEBALLBAT = 16,
	HUD_SHOVEL = 17,
	HUD_POOLCUE = 18,
	HUD_KATANA = 19,
	HUD_CHAINSAW = 21,
	HUD_GRENADE = 22,
	HUD_TEARGAS = 23,
	HUD_MOLOTOV = 24,
	HUD_MISSILE = 25,
	HUD_PISTOL = 26,
	HUD_PISTOL_SILENCED = 27,
	HUD_DESERT_EAGLE = 28,
	HUD_SHOTGUN = 29,
	HUD_SAWNOFF = 30,
	HUD_SPAS12 = 31,
	HUD_MICRO_UZI = 32,
	HUD_MP5 = 33,
	HUD_FLARE = 34,
	HUD_AK47 = 35,
	HUD_M4 = 36,
	HUD_COUNTRYRIFLE = 37,
	HUD_SNIPERRIFLE = 38,
	HUD_RLAUNCHER = 39,
	HUD_HEATSEEK = 40,
	HUD_FLAMETHROWER = 41,
	HUD_MINIGUN = 42,
	HUD_SATCHEL = 43,
	HUD_DETONATOR = 44,
	HUD_SPRAYCAN = 45,
	HUD_EXTINGUISHER = 46,
	HUD_CAMERA = 47,
	HUD_NIGHTVISION = 48,
	HUD_INFRARED = 49,
	HUD_JETPACK = 50,
	HUD_PARACHUTE = 51,
	HUD_TEC9 = 52, // 372

	HUD_TOTALWEAPONS
};

class CHudNew {
public:
	static CSprite2d SpritesMod[HUD_TOTALWEAPONS];
	static eGameMode ms_nGameMode;
	static bool ms_bSpritesLoaded;
	static char *ms_nGamePrefix;
	static bool ms_bReload;

public:
	static void Initialise();
	static void Shutdown();
	static void ReInitialise();
	static void Draw();
	static void DrawAfterFade();
	static void PrintClock(float x, float y, float w, float h);
	static void PrintMoneyCounter(float x, float y, float w, float h);
	static void PrintHealth(int PlayerID, float x, float y, float w, float h);
	static void PrintArmour(int PlayerID, float x, float y, float w, float h);
	static void PrintBreath(int PlayerID, float x, float y, float w, float h);
	static void PrintWeaponIcon(int PlayerID, float x, float y, float w, float h);
	static void PrintAmmo(int PlayerID, float x, float y, float w, float h);
	static void DrawWanted(float x, float y, float w, float h);
	static void PrintStatsBox(float x, float y, float w, float h);
	static void DrawAreaName(float x, float y, float w, float h);
	static void DrawVehicleName(float x, float y, float w, float h);

public:
	static void DrawProgressBar(float x, float y, float w, float h, float percentage);
	static void DrawProgressString(CSprite2d sprite, float, float y, float w, float h);

public:
	static eGameMode GetGameMode() { return ms_nGameMode; }
	static void SetGameMode(eGameMode Mode) { ms_nGameMode = Mode; }
	static char *GetGamePrefix() { return ms_nGamePrefix; }

public:
	static void InjectPatches();
};