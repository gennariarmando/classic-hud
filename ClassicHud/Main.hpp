#pragma once

#define g_pHelpMessage ((char *)0xBAA7A0)
#define m_pLastHelpMessage ((char *)0xBAA610)
#define g_HelpMessageState (*(__int32 *)0xBAA474)
#define g_HelpMessageTimer (*(signed __int32 *)0xBAA47C)
#define g_HelpMessageFadeTimer (*(signed __int32 *)0xBAA478)
#define g_pHelpMessageToPrint ((char *)0xBAA480)
#define g_fTextBoxNumLines (*(float *)0xBAA460)
#define PLAYONESHOT(id, volume) ((void (__thiscall *)(unsigned int, unsigned short, float, float))0x506EA0)(0xB6BC90, id, volume, 1.0)
#define Camera_WidescreenOn (*(unsigned __int8 *)0xB6F065)
#define bTextBoxPermanent2 (*(__int8 *)0xBAA464)
#define m_HelpMessageQuick (*(unsigned __int8 *)0xBAA472)
#define Cutscene_Running (*(unsigned __int8 *)0xB5F851)
#define g_HelpMessageStatId (*(unsigned __int16 *)0xBAA470)
#define g_filenameBuffer ((char *)0xB71670)
#define flt_8D0938 (*(float *)0x8D0938)
#define GetGroupMembersCount(groupId) ((unsigned int (__thiscall *)(unsigned int))0x5F6AA0)(groupId * 0x2D4 + 0xC09928)
#define GetPlayerStat(statid) ((float (__cdecl *)(unsigned short))0x558E40)(statid)
#define dword_BAA468 (*(float *)0xBAA468)
#define g_fScriptTextBoxesWidth (*(float *)0x8D0934)
#define StatMax (*(unsigned __int16 *)0xBAA46C)
#define g_BigMessage0 ((char *)0xBAACC0)
#define g_BigMessage4 ((char *)0xBAAEC0)
#define byte_96C014 ((char *)0x96C014)
#define Menu_WidescreenOn (*(unsigned __int8 *)0xBA6793)
#define ZoneToPrint ((char *)0xBAB1D0)

static float fProperWidthMultiplier = 480.0f / 448.0f;
static float fProperHeightMultiplier = 448.0f / 480.0f;

#define _x(a) (RsGlobal.maximumWidth - fProperWidthMultiplier * (a))
#define _xleft(a) (fProperWidthMultiplier * (a))
#define _xmiddle(a) ((RsGlobal.maximumWidth / 2) + fProperWidthMultiplier * (a))
#define _y(a) (fProperHeightMultiplier * (a))
#define _ydown(a) (RsGlobal.maximumHeight - fProperHeightMultiplier * (a))
#define _ymiddle(a) ((RsGlobal.maximumHeight / 2) + fProperHeightMultiplier * (a))
#define _width(a) _xleft(a)
#define _height(a) _y(a)

// Plugin-SDK
#include <plugin.h>
#include "game_sa\CClock.h"
#include "game_sa\CFont.h"
#include "game_sa\CFileMgr.h"
#include "game_sa\CHud.h"
#include "game_sa\CMenuManager.h"
#include "game_sa\CMessages.h"
#include "game_sa\CPad.h"
#include "game_sa\CPed.h"
#include "game_sa\CRadar.h"
#include "game_sa\CSprite2d.h"
#include "game_sa\CSprite.h"
#include "game_sa\CStats.h"
#include "game_sa\CText.h"
#include "game_sa\CTimer.h"
#include "game_sa\CTxdStore.h"
#include "game_sa\CWeaponInfo.h"
#include "game_sa\CWorld.h"

#include "game_sa\common.h"
#include "game_sa\RenderWare.h"

// ClassicHud
#include "IniReader.h"

// Ini
static int Enable;
static int Ultra;
static int Style;
static int SquareBar;
static int DrawBarChart;
static int MoneyCounter;
static int StatBox;

static float HUD_POS_X;
static float HUD_POS_Y;

// Health
static int HUD_COLOUR_HEALTH_R;
static int HUD_COLOUR_HEALTH_G;
static int HUD_COLOUR_HEALTH_B;
static int HUD_COLOUR_HEALTH_A;

// Armour
static int HUD_COLOUR_ARMOUR_R;
static int HUD_COLOUR_ARMOUR_G;
static int HUD_COLOUR_ARMOUR_B;
static int HUD_COLOUR_ARMOUR_A;

// Breath
static int HUD_COLOUR_BREATH_R;
static int HUD_COLOUR_BREATH_G;
static int HUD_COLOUR_BREATH_B;
static int HUD_COLOUR_BREATH_A;

// Money
static int HUD_COLOUR_MONEY_R;
static int HUD_COLOUR_MONEY_G;
static int HUD_COLOUR_MONEY_B;
static int HUD_COLOUR_MONEY_A;

// Clock
static int HUD_COLOUR_CLOCK_R;
static int HUD_COLOUR_CLOCK_G;
static int HUD_COLOUR_CLOCK_B;
static int HUD_COLOUR_CLOCK_A;

static int HUD_DROPCOLOUR_CLOCK_R;
static int HUD_DROPCOLOUR_CLOCK_G;
static int HUD_DROPCOLOUR_CLOCK_B;
static int HUD_DROPCOLOUR_CLOCK_A;

// Ammo
static int HUD_COLOUR_AMMO_R;
static int HUD_COLOUR_AMMO_G;
static int HUD_COLOUR_AMMO_B;

static int HUD_DROPCOLOUR_AMMO_R;
static int HUD_DROPCOLOUR_AMMO_G;
static int HUD_DROPCOLOUR_AMMO_B;

// Star
static int HUD_COLOUR_STAR_R;
static int HUD_COLOUR_STAR_G;
static int HUD_COLOUR_STAR_B;
static int HUD_COLOUR_SHADOWSTAR_A;
static int HUD_COLOUR_SHADOWSTAR_A3;
static int HUD_COLOUR_SHADOWSTAR_A2;

static int HUD_COLOUR_STARBACK_R;
static int HUD_COLOUR_STARBACK_G;
static int HUD_COLOUR_STARBACK_B;

// Area
static int HUD_COLOUR_AREA_R;
static int HUD_COLOUR_AREA_G;
static int HUD_COLOUR_AREA_B;

static int HUD_COLOUR_VEHICLE_R;
static int HUD_COLOUR_VEHICLE_G;
static int HUD_COLOUR_VEHICLE_B;

// StatBox
static int HUD_COLOUR_STATBAR_R;
static int HUD_COLOUR_STATBAR_G;
static int HUD_COLOUR_STATBAR_B;

// Map Arrot
static int MAP_COLOUR_ARROW_R;
static int MAP_COLOUR_ARROW_G;
static int MAP_COLOUR_ARROW_B;

// Weapon
static int HUD_COLOUR_WEAPON_A;

// Wasted & Busted
static int HUD_COLOUR_WB_R;
static int HUD_COLOUR_WB_G;
static int HUD_COLOUR_WB_B;

// Weapon
static float HUD_WEAPON_POS_X;
static float HUD_WEAPON_POS_Y;
static float HUD_WEAPON_SIZE_X;
static float HUD_WEAPON_SIZE_Y;

// Ammo
static float HUD_AMMO_POS_X;
static float HUD_AMMO_POS_Y;
static float HUD_AMMO_SIZE_X;
static float HUD_AMMO_SIZE_Y;

// Health
static float HUD_HEALTH_POS_X;
static float HUD_HEALTH_POS_Y;
static float HUD_HEALTH_SIZE_X;
static float HUD_HEALTH_SIZE_Y;

static float HUD_HEALTHBAR_POS_X;
static float HUD_HEALTHBAR_POS_Y;
static float HUD_HEALTHBAR_SIZE_X;
static float HUD_HEALTHBAR_SIZE_Y;

// Armour
static float HUD_ARMOUR_POS_X;
static float HUD_ARMOUR_POS_Y;
static float HUD_ARMOUR_SIZE_X;
static float HUD_ARMOUR_SIZE_Y;

static float HUD_ARMOURBAR_POS_X;
static float HUD_ARMOURBAR_POS_Y;
static float HUD_ARMOURBAR_SIZE_X;
static float HUD_ARMOURBAR_SIZE_Y;

// Breath
static float HUD_BREATH_POS_X;
static float HUD_BREATH_POS_X_2;
static float HUD_BREATH_POS_Y;
static float HUD_BREATH_SIZE_X;
static float HUD_BREATH_SIZE_Y;

static float HUD_BREATHBAR_POS_X;
static float HUD_BREATHBAR_POS_Y;
static float HUD_BREATHBAR_SIZE_X;
static float HUD_BREATHBAR_SIZE_Y;

// Health icon
static float HUD_HEALTHICON_POS_X;
static float HUD_HEALTHICON_POS_Y;
static float HUD_HEALTHICON_SIZE_X;
static float HUD_HEALTHICON_SIZE_Y;

// Armour icon
static float HUD_ARMOURICON_POS_X;
static float HUD_ARMOURICON_POS_Y;
static float HUD_ARMOURICON_SIZE_X;
static float HUD_ARMOURICON_SIZE_Y;

// Breath icon
static float HUD_BREATHICON_POS_X;
static float HUD_BREATHICON_POS_Y;
static float HUD_BREATHICON_SIZE_X;
static float HUD_BREATHICON_SIZE_Y;

// Money
static float HUD_MONEY_POS_X;
static float HUD_MONEY_POS_Y;
static float HUD_MONEY_SIZE_X;
static float HUD_MONEY_SIZE_Y;

// Clock
static float HUD_CLOCK_POS_X;
static float HUD_CLOCK_POS_Y;
static float HUD_CLOCK_SIZE_X;
static float HUD_CLOCK_SIZE_Y;

// Star
static float HUD_STAR_POS_X = 0.0f;
static float HUD_STAR_POS_Y = 200.8f;
static float HUD_STAR_SIZE_X = 37.5f;
static float HUD_STAR_SIZE_Y = 31.5f;

// Area
static float HUD_AREA_POS_X;
static float HUD_AREA_POS_Y;
static float HUD_AREA_SIZE_X;
static float HUD_AREA_SIZE_Y;
static float HUD_AREA_SLANT;

// Vehicle
static float HUD_VEHICLE_POS_X;
static float HUD_VEHICLE_POS_Y;
static float HUD_VEHICLE_SIZE_X;
static float HUD_VEHICLE_SIZE_Y;
static float HUD_VEHICLE_SLANT;

static float HUD_STAR_SPACE;

static float RADAR_RANGE;
static int RADAR_COLOUR_A;

static char* SetTextBasedOnStyle;

static int HUD_AMMO_OUTLINE;
static int HUD_AMMO_SHADOW;

static int HUD_GLOBAL_SHADOW;
static int HUD_GLOBAL_OUTLINE;

static char* MoneyString;
static char* MinusMoneyString;

static int STAT_WEP_ALPHA;
static float stat3_2;

static short HUD_AREA_FONT;

static short HUD_VEHICLE_FONT;

static const float fTextBoxPosnX = 50.0f;
static const float fTextBoxPosnXWithRadar = 50.0f;
static const float fTextBoxPosnXWithRadarAndPlane = 50.0f;
static const float fTextBoxPosnY = 50.0;
static const float fTextBoxFontScaleX = 0.7;
static const float fTextBoxFontScaleY = 1.9;
static const float fTextBoxWidth = 421.0;
static const float fTextBoxBorderSize = 6.0;

static const char *hudfilename;
static const char *fontfilename;
static const char *fontdatafilename;

static void Ini()
{
	CIniReader iniReader(".\\CLASSICHUD\\CLASSICHUD.INI");
	CIniReader hudcolors(".\\CLASSICHUD\\HUDCOLOR.DAT");

	Enable = iniReader.ReadInteger("MAIN", "Enable", 0);
	Ultra = iniReader.ReadInteger("MAIN", "UltraWide", 0);
	Style = iniReader.ReadInteger("MAIN", "Style", 0);

	MoneyCounter = iniReader.ReadInteger("HUD", "iMinimalMoneyCounter", 0);
	StatBox = iniReader.ReadInteger("HUD", "iMinimalStatsBox", 0);
	SquareBar = iniReader.ReadInteger("HUD", "iSquareBar", 0);
	
	RADAR_RANGE = iniReader.ReadFloat("RADAR", "fRadarRange", 0.0f);
	RADAR_COLOUR_A = iniReader.ReadRGBA("RADAR", "iRadarAlpha", 255);

	DrawBarChart = iniReader.ReadInteger("CUSTOM", "iDrawBarChart", 0);

	if (MoneyCounter == 1) {
		MoneyString = "$%d";
		MinusMoneyString = "-$%d";
	}
	else {
		MoneyString = "$%08d";
		MinusMoneyString = "-$%07d";
	}

	if (Style == 0) {
		// hudColor III
		HUD_COLOUR_HEALTH_R = 180;
		HUD_COLOUR_HEALTH_G = 100;
		HUD_COLOUR_HEALTH_B = 50;
		HUD_COLOUR_HEALTH_A = 255;

		HUD_COLOUR_ARMOUR_R = 121;
		HUD_COLOUR_ARMOUR_G = 136;
		HUD_COLOUR_ARMOUR_B = 93;
		HUD_COLOUR_ARMOUR_A = 255;

		HUD_COLOUR_BREATH_R = 220;
		HUD_COLOUR_BREATH_G = 235;
		HUD_COLOUR_BREATH_B = 255;
		HUD_COLOUR_BREATH_A = 255;

		HUD_COLOUR_MONEY_R = 90;
		HUD_COLOUR_MONEY_G = 115;
		HUD_COLOUR_MONEY_B = 150;
		HUD_COLOUR_MONEY_A = 255;

		HUD_COLOUR_CLOCK_R = 193;
		HUD_COLOUR_CLOCK_G = 166;
		HUD_COLOUR_CLOCK_B = 121;
		HUD_COLOUR_CLOCK_A = 255;

		HUD_DROPCOLOUR_CLOCK_R = 0;
		HUD_DROPCOLOUR_CLOCK_G = 0;
		HUD_DROPCOLOUR_CLOCK_B = 0;
		HUD_DROPCOLOUR_CLOCK_A = 255;

		HUD_COLOUR_AMMO_R = 0;
		HUD_COLOUR_AMMO_G = 0;
		HUD_COLOUR_AMMO_B = 0;

		HUD_DROPCOLOUR_AMMO_R = 252;
		HUD_DROPCOLOUR_AMMO_G = 175;
		HUD_DROPCOLOUR_AMMO_B = 1;

		HUD_COLOUR_STAR_R = 193;
		HUD_COLOUR_STAR_G = 166;
		HUD_COLOUR_STAR_B = 121;
		HUD_COLOUR_SHADOWSTAR_A = 255;
		HUD_COLOUR_SHADOWSTAR_A2 = 255;
		HUD_COLOUR_SHADOWSTAR_A3 = 255;

		HUD_COLOUR_STARBACK_R = 0;
		HUD_COLOUR_STARBACK_G = 0;
		HUD_COLOUR_STARBACK_B = 0;

		HUD_COLOUR_AREA_R = 150;
		HUD_COLOUR_AREA_G = 150;
		HUD_COLOUR_AREA_B = 80;

		HUD_COLOUR_VEHICLE_R = 189;
		HUD_COLOUR_VEHICLE_G = 160;
		HUD_COLOUR_VEHICLE_B = 120;

		HUD_COLOUR_STATBAR_R = 219;
		HUD_COLOUR_STATBAR_G = 155;
		HUD_COLOUR_STATBAR_B = 0;

		MAP_COLOUR_ARROW_R = 255;
		MAP_COLOUR_ARROW_G = 255;
		MAP_COLOUR_ARROW_B = 255;

		HUD_COLOUR_WEAPON_A = 255;

		HUD_COLOUR_WB_R = 180;
		HUD_COLOUR_WB_G = 100;
		HUD_COLOUR_WB_B = 50;

		// hudPosition III
		HUD_WEAPON_POS_X = 180.0f;
		HUD_WEAPON_POS_Y = 48.0f;
		HUD_WEAPON_SIZE_X = 117.0f;
		HUD_WEAPON_SIZE_Y = 115.5f;

		HUD_AMMO_POS_X = 122.5f;
		HUD_AMMO_POS_Y = 132.0f;
		HUD_AMMO_SIZE_X = 0.72f;
		HUD_AMMO_SIZE_Y = 1.38f;

		HUD_HEALTH_POS_X = 196.8f;
		HUD_HEALTH_POS_Y = 118.8f;
		HUD_HEALTH_SIZE_X = 1.41f;
		HUD_HEALTH_SIZE_Y = 2.46f;

		HUD_ARMOUR_POS_X = 326.4f;
		HUD_ARMOUR_POS_Y = 118.8f;
		HUD_ARMOUR_SIZE_X = 1.38f;
		HUD_ARMOUR_SIZE_Y = 2.45f;

		HUD_BREATH_POS_X = 326.4f;
		HUD_BREATH_POS_X_2 = 127.2f;
		HUD_BREATH_POS_Y = 118.8f;
		HUD_BREATH_SIZE_X = 1.38f;
		HUD_BREATH_SIZE_Y = 2.46f;
		HUD_BREATHICON_POS_X = 448.8f;
		HUD_BREATHICON_POS_Y = 126.0f;
		HUD_BREATHICON_SIZE_X = 36.0f;
		HUD_BREATHICON_SIZE_Y = 31.5f;

		HUD_HEALTHICON_POS_X = 322.8f;
		HUD_HEALTHICON_POS_Y = 126.0f;
		HUD_HEALTHICON_SIZE_X = 37.5f;
		HUD_HEALTHICON_SIZE_Y = 31.5f;

		HUD_ARMOURICON_POS_X = 448.8f;
		HUD_ARMOURICON_POS_Y = 126.0f;
		HUD_ARMOURICON_SIZE_X = 36.0f;
		HUD_ARMOURICON_SIZE_Y = 31.5f;

		HUD_MONEY_POS_X = 196.8f;
		HUD_MONEY_POS_Y = 78.0f;
		HUD_MONEY_SIZE_X = 1.38f;
		HUD_MONEY_SIZE_Y = 2.46f;

		HUD_GLOBAL_SHADOW = 1;

		HUD_CLOCK_POS_X = 196.8f;
		HUD_CLOCK_POS_Y = 40.8f;
		HUD_CLOCK_SIZE_X = 1.404f;
		HUD_CLOCK_SIZE_Y = 2.46f;

		HUD_STAR_POS_X = 102.0f;
		HUD_STAR_POS_Y = 166.0f;
		HUD_STAR_SIZE_X = 42.0f;
		HUD_STAR_SIZE_Y = 39.0f;

		HUD_STAR_SPACE = 42.0f;

		HUD_AREA_POS_X = 50.0f;
		HUD_AREA_POS_Y = 80.0f;
		HUD_AREA_SIZE_X = 1.5f;
		HUD_AREA_SIZE_Y = 3.0f;

		HUD_AREA_FONT = FONT_GOTHIC;

		HUD_VEHICLE_POS_X = 50.0f;
		HUD_VEHICLE_POS_Y = 130.0f;
		HUD_VEHICLE_SIZE_X = 1.5f;
		HUD_VEHICLE_SIZE_Y = 3.0f;

		HUD_VEHICLE_FONT = FONT_GOTHIC;

		hudfilename = ".\\CLASSICHUD\\TXD\\HUD\\HUD_III.TXD";
		fontfilename = ".\\CLASSICHUD\\TXD\\FONTS\\FONTS_III.TXD";
		fontdatafilename = ".\\CLASSICHUD\\DATA\\FONTS\\FONTS_III.DAT";

		SetTextBasedOnStyle = "ClassicHud - III";
	}
	if (Style == 1) {
		// hudColour VC
		HUD_COLOUR_HEALTH_R = 255;
		HUD_COLOUR_HEALTH_G = 150;
		HUD_COLOUR_HEALTH_B = 225;
		HUD_COLOUR_HEALTH_A = 255;

		HUD_COLOUR_ARMOUR_R = 185;
		HUD_COLOUR_ARMOUR_G = 185;
		HUD_COLOUR_ARMOUR_B = 185;
		HUD_COLOUR_ARMOUR_A = 225;

		HUD_COLOUR_BREATH_R = 220;
		HUD_COLOUR_BREATH_G = 235;
		HUD_COLOUR_BREATH_B = 255;
		HUD_COLOUR_BREATH_A = 255;

		HUD_COLOUR_MONEY_R = 0;
		HUD_COLOUR_MONEY_G = 210;
		HUD_COLOUR_MONEY_B = 133;
		HUD_COLOUR_MONEY_A = 255;

		HUD_COLOUR_CLOCK_R = 90;
		HUD_COLOUR_CLOCK_G = 190;
		HUD_COLOUR_CLOCK_B = 232;
		HUD_COLOUR_CLOCK_A = 255;

		HUD_DROPCOLOUR_CLOCK_R = 0;
		HUD_DROPCOLOUR_CLOCK_G = 0;
		HUD_DROPCOLOUR_CLOCK_B = 0;
		HUD_DROPCOLOUR_CLOCK_A = 255;

		HUD_COLOUR_AMMO_R = 90;
		HUD_COLOUR_AMMO_G = 190;
		HUD_COLOUR_AMMO_B = 232;

		HUD_DROPCOLOUR_AMMO_R = 0;
		HUD_DROPCOLOUR_AMMO_G = 0;
		HUD_DROPCOLOUR_AMMO_B = 0;

		HUD_COLOUR_AREA_R = 45;
		HUD_COLOUR_AREA_G = 155;
		HUD_COLOUR_AREA_B = 90;

		HUD_COLOUR_VEHICLE_R = 92;
		HUD_COLOUR_VEHICLE_G = 175;
		HUD_COLOUR_VEHICLE_B = 219;

		HUD_COLOUR_STAR_R = 89;
		HUD_COLOUR_STAR_G = 196;
		HUD_COLOUR_STAR_B = 242;
		HUD_COLOUR_SHADOWSTAR_A = 255;
		HUD_COLOUR_SHADOWSTAR_A2 = 255;
		HUD_COLOUR_SHADOWSTAR_A3 = 255;

		HUD_COLOUR_STARBACK_R = 27;
		HUD_COLOUR_STARBACK_G = 89;
		HUD_COLOUR_STARBACK_B = 128;

		HUD_COLOUR_STATBAR_R = 220;
		HUD_COLOUR_STATBAR_G = 116;
		HUD_COLOUR_STATBAR_B = 203;

		MAP_COLOUR_ARROW_R = 255;
		MAP_COLOUR_ARROW_G = 150;
		MAP_COLOUR_ARROW_B = 225;

		HUD_COLOUR_WEAPON_A = 255;

		HUD_COLOUR_WB_R = 0;
		HUD_COLOUR_WB_G = 210;
		HUD_COLOUR_WB_B = 133;

		// hudPosition VC
		HUD_WEAPON_POS_X = 180.0f;
		HUD_WEAPON_POS_Y = 48.0f;
		HUD_WEAPON_SIZE_X = 117.0f;
		HUD_WEAPON_SIZE_Y = 115.5f;

		HUD_AMMO_POS_X = 122.5f;
		HUD_AMMO_POS_Y = 132.0f;
		HUD_AMMO_SIZE_X = 0.52f;
		HUD_AMMO_SIZE_Y = 1.35f;

		HUD_AMMO_SHADOW = 1;
		
		HUD_HEALTH_POS_X = 196.8f;
		HUD_HEALTH_POS_Y = 118.8f;
		HUD_HEALTH_SIZE_X = 1.35f;
		HUD_HEALTH_SIZE_Y = 2.46f;

		HUD_ARMOUR_POS_X = 326.4f;
		HUD_ARMOUR_POS_Y = 118.8f;
		HUD_ARMOUR_SIZE_X = 1.35f;
		HUD_ARMOUR_SIZE_Y = 2.45f;

		HUD_BREATH_POS_X = 326.4f;
		HUD_BREATH_POS_X_2 = 127.2f;
		HUD_BREATH_POS_Y = 118.8f;
		HUD_BREATH_SIZE_X = 1.35f;
		HUD_BREATH_SIZE_Y = 2.46f;
		HUD_BREATHICON_POS_X = 448.8f;
		HUD_BREATHICON_POS_Y = 126.0f;
		HUD_BREATHICON_SIZE_X = 33.0f;
		HUD_BREATHICON_SIZE_Y = 30.5f;

		HUD_HEALTHICON_POS_X = 320.8f;
		HUD_HEALTHICON_POS_Y = 126.0f;
		HUD_HEALTHICON_SIZE_X = 35.0f;
		HUD_HEALTHICON_SIZE_Y = 30.5f;

		HUD_ARMOURICON_POS_X = 448.8f;
		HUD_ARMOURICON_POS_Y = 126.0f;
		HUD_ARMOURICON_SIZE_X = 33.0f;
		HUD_ARMOURICON_SIZE_Y = 30.0f;

		HUD_MONEY_POS_X = 196.8f;
		HUD_MONEY_POS_Y = 78.0f;
		HUD_MONEY_SIZE_X = 1.35f;
		HUD_MONEY_SIZE_Y = 2.46f;

		HUD_GLOBAL_SHADOW = 1;

		HUD_CLOCK_POS_X = 196.8f;
		HUD_CLOCK_POS_Y = 40.8f;
		HUD_CLOCK_SIZE_X = 1.35f;
		HUD_CLOCK_SIZE_Y = 2.46f;

		HUD_STAR_POS_X = 228.5f;
		HUD_STAR_POS_Y = 166.0f;
		HUD_STAR_SIZE_X = 38.0f;
		HUD_STAR_SIZE_Y = 35.5f;

		HUD_STAR_SPACE = 42.0f;

		HUD_AREA_POS_X = 50.0f;
		HUD_AREA_POS_Y = 250.0f;
		HUD_AREA_SIZE_X = 2.0f;
		HUD_AREA_SIZE_Y = 3.5f;

		HUD_AREA_SLANT = 0.14;

		HUD_AREA_FONT = FONT_GOTHIC;

		HUD_VEHICLE_POS_X = 50.0f;
		HUD_VEHICLE_POS_Y = 180.0f;
		HUD_VEHICLE_SIZE_X = 2.0f;
		HUD_VEHICLE_SIZE_Y = 3.5f;

		HUD_VEHICLE_SLANT = 0.14;

		HUD_VEHICLE_FONT = FONT_GOTHIC;

		hudfilename = ".\\CLASSICHUD\\TXD\\HUD\\HUD_VC.TXD";
		fontfilename = ".\\CLASSICHUD\\TXD\\FONTS\\FONTS_VC.TXD";
		fontdatafilename = ".\\CLASSICHUD\\DATA\\FONTS\\FONTS_VC.DAT";

		SetTextBasedOnStyle = "ClassicHud - VC";
	}
	if (Style == 2) {
		// hudColour LCS
		HUD_COLOUR_HEALTH_R = 142;
		HUD_COLOUR_HEALTH_G = 24;
		HUD_COLOUR_HEALTH_B = 17;
		HUD_COLOUR_HEALTH_A = 220;

		HUD_COLOUR_ARMOUR_R = 60;
		HUD_COLOUR_ARMOUR_G = 123;
		HUD_COLOUR_ARMOUR_B = 175;
		HUD_COLOUR_ARMOUR_A = 220;

		HUD_COLOUR_BREATH_R = 220;
		HUD_COLOUR_BREATH_G = 235;
		HUD_COLOUR_BREATH_B = 255;
		HUD_COLOUR_BREATH_A = 220;

		HUD_COLOUR_MONEY_R = 66;
		HUD_COLOUR_MONEY_G = 104;
		HUD_COLOUR_MONEY_B = 45;
		HUD_COLOUR_MONEY_A = 220;

		HUD_COLOUR_CLOCK_R = 159;
		HUD_COLOUR_CLOCK_G = 120;
		HUD_COLOUR_CLOCK_B = 35;
		HUD_COLOUR_CLOCK_A = 220;

		HUD_DROPCOLOUR_CLOCK_R = 0;
		HUD_DROPCOLOUR_CLOCK_G = 0;
		HUD_DROPCOLOUR_CLOCK_B = 0;
		HUD_DROPCOLOUR_CLOCK_A = 220;

		HUD_COLOUR_AMMO_R = 255;
		HUD_COLOUR_AMMO_G = 255;
		HUD_COLOUR_AMMO_B = 255;

		HUD_DROPCOLOUR_AMMO_R = 0;
		HUD_DROPCOLOUR_AMMO_G = 0;
		HUD_DROPCOLOUR_AMMO_B = 0;

		HUD_COLOUR_AREA_R = 255;
		HUD_COLOUR_AREA_G = 255;
		HUD_COLOUR_AREA_B = 255;

		HUD_COLOUR_VEHICLE_R = 255;
		HUD_COLOUR_VEHICLE_G = 255;
		HUD_COLOUR_VEHICLE_B = 255;

		HUD_COLOUR_STAR_R = 185;
		HUD_COLOUR_STAR_G = 153;
		HUD_COLOUR_STAR_B = 62;
		HUD_COLOUR_SHADOWSTAR_A = 0;
		HUD_COLOUR_SHADOWSTAR_A2 = 0;
		HUD_COLOUR_SHADOWSTAR_A3 = 0;

		HUD_COLOUR_STARBACK_R = 27;
		HUD_COLOUR_STARBACK_G = 89;
		HUD_COLOUR_STARBACK_B = 128;

		HUD_COLOUR_STATBAR_R = 255;
		HUD_COLOUR_STATBAR_G = 255;
		HUD_COLOUR_STATBAR_B = 255;

		MAP_COLOUR_ARROW_R = 142;
		MAP_COLOUR_ARROW_G = 24;
		MAP_COLOUR_ARROW_B = 17;

		HUD_COLOUR_WEAPON_A = 220;

		HUD_COLOUR_WB_R = 160;
		HUD_COLOUR_WB_G = 0;
		HUD_COLOUR_WB_B = 3;

		// hudPosition LCS
		HUD_WEAPON_POS_X = 175.0f;
		HUD_WEAPON_POS_Y = 49.0f;
		HUD_WEAPON_SIZE_X = 110.0f;
		HUD_WEAPON_SIZE_Y = 128.0f;

		HUD_AMMO_POS_X = 120.5f;
		HUD_AMMO_POS_Y = 144.0f;
		HUD_AMMO_SIZE_X = 0.53f;
		HUD_AMMO_SIZE_Y = 1.35f;

		HUD_AMMO_SHADOW = 1;

		HUD_HEALTHBAR_POS_X = 297.0f;
		HUD_HEALTHBAR_POS_Y = 136.0f;
		HUD_HEALTHBAR_SIZE_X = 114.1f;
		HUD_HEALTHBAR_SIZE_Y = 40.1f;

		HUD_ARMOURBAR_POS_X = 297.0f;
		HUD_ARMOURBAR_POS_Y = 91.0f;
		HUD_ARMOURBAR_SIZE_X = 114.1f;
		HUD_ARMOURBAR_SIZE_Y = 40.1f;

		HUD_BREATH_POS_X_2 = 120.0f;

		HUD_MONEY_POS_X = 61.5f;
		HUD_MONEY_POS_Y = 179.5f;
		HUD_MONEY_SIZE_X = 1.332f;
		HUD_MONEY_SIZE_Y = 2.50f;

		HUD_GLOBAL_SHADOW = 0;

		HUD_CLOCK_POS_X = 182.0f;
		HUD_CLOCK_POS_Y = 40.5f;
		HUD_CLOCK_SIZE_X = 1.16f;
		HUD_CLOCK_SIZE_Y = 2.32f;

		HUD_STAR_POS_X = 100.0f;
		HUD_STAR_POS_Y = 222.0f;
		HUD_STAR_SIZE_X = 38.0f;
		HUD_STAR_SIZE_Y = 35.0f;

		HUD_STAR_SPACE = 40.0f;

		HUD_AREA_POS_X = 50.0f;
		HUD_AREA_POS_Y = 130.0f;
		HUD_AREA_SIZE_X = 1.3f;
		HUD_AREA_SIZE_Y = 3.0f;

		HUD_VEHICLE_POS_X = 50.0f;
		HUD_VEHICLE_POS_Y = 180.0f;
		HUD_VEHICLE_SIZE_X = 1.3f;
		HUD_VEHICLE_SIZE_Y = 3.0f;

		HUD_AREA_FONT = FONT_MENU;

		HUD_VEHICLE_FONT = FONT_MENU;

		hudfilename = ".\\CLASSICHUD\\TXD\\HUD\\HUD_LCS.TXD";
		fontfilename = ".\\CLASSICHUD\\TXD\\FONTS\\FONTS_STORIES.TXD";
		fontdatafilename = ".\\CLASSICHUD\\DATA\\FONTS\\FONTS_STORIES.DAT";

		SetTextBasedOnStyle = "ClassicHud - LCS";
	}
	if (Style == 3) {
		// hudColour VCS
		HUD_COLOUR_HEALTH_R = 208;
		HUD_COLOUR_HEALTH_G = 88;
		HUD_COLOUR_HEALTH_B = 133;
		HUD_COLOUR_HEALTH_A = 220;

		HUD_COLOUR_ARMOUR_R = 17;
		HUD_COLOUR_ARMOUR_G = 228;
		HUD_COLOUR_ARMOUR_B = 221;
		HUD_COLOUR_ARMOUR_A = 220;

		HUD_COLOUR_BREATH_R = 220;
		HUD_COLOUR_BREATH_G = 235;
		HUD_COLOUR_BREATH_B = 255;
		HUD_COLOUR_BREATH_A = 220;

		HUD_COLOUR_MONEY_R = 66;
		HUD_COLOUR_MONEY_G = 104;
		HUD_COLOUR_MONEY_B = 45;
		HUD_COLOUR_MONEY_A = 220;

		HUD_COLOUR_CLOCK_R = 159;
		HUD_COLOUR_CLOCK_G = 120;
		HUD_COLOUR_CLOCK_B = 35;
		HUD_COLOUR_CLOCK_A = 220;

		HUD_DROPCOLOUR_CLOCK_R = 0;
		HUD_DROPCOLOUR_CLOCK_G = 0;
		HUD_DROPCOLOUR_CLOCK_B = 0;
		HUD_DROPCOLOUR_CLOCK_A = 220;

		HUD_COLOUR_AMMO_R = 255;
		HUD_COLOUR_AMMO_G = 255;
		HUD_COLOUR_AMMO_B = 255;

		HUD_DROPCOLOUR_AMMO_R = 0;
		HUD_DROPCOLOUR_AMMO_G = 0;
		HUD_DROPCOLOUR_AMMO_B = 0;

		HUD_COLOUR_AREA_R = 255;
		HUD_COLOUR_AREA_G = 255;
		HUD_COLOUR_AREA_B = 255;

		HUD_COLOUR_VEHICLE_R = 255;
		HUD_COLOUR_VEHICLE_G = 255;
		HUD_COLOUR_VEHICLE_B = 255;

		HUD_COLOUR_STAR_R = 185;
		HUD_COLOUR_STAR_G = 153;
		HUD_COLOUR_STAR_B = 62;
		HUD_COLOUR_SHADOWSTAR_A = 0;
		HUD_COLOUR_SHADOWSTAR_A2 = 0;
		HUD_COLOUR_SHADOWSTAR_A3 = 0;

		HUD_COLOUR_STARBACK_R = 27;
		HUD_COLOUR_STARBACK_G = 89;
		HUD_COLOUR_STARBACK_B = 128;

		HUD_COLOUR_STATBAR_R = 200;
		HUD_COLOUR_STATBAR_G = 88;
		HUD_COLOUR_STATBAR_B = 133;

		MAP_COLOUR_ARROW_R = 200;
		MAP_COLOUR_ARROW_G = 88;
		MAP_COLOUR_ARROW_B = 133;

		HUD_COLOUR_WEAPON_A = 220;

		HUD_COLOUR_WB_R = 0;
		HUD_COLOUR_WB_G = 210;
		HUD_COLOUR_WB_B = 133;

		// hudPosition VCS
	/*	HUD_WEAPON_POS_X = 180.0f;
		HUD_WEAPON_POS_Y = 45.0f;
		HUD_WEAPON_SIZE_X = 160.0f;
		HUD_WEAPON_SIZE_Y = 165.0f;*/

		HUD_WEAPON_POS_X = 175.0f;
		HUD_WEAPON_POS_Y = 49.0f;
		HUD_WEAPON_SIZE_X = 110.0f;
		HUD_WEAPON_SIZE_Y = 128.0f;

		HUD_AMMO_POS_X = 120.5f;
		HUD_AMMO_POS_Y = 144.0f;
		HUD_AMMO_SIZE_X = 0.53f;
		HUD_AMMO_SIZE_Y = 1.35f;

		HUD_AMMO_SHADOW = 1;

		HUD_HEALTHBAR_POS_X = 297.0f;
		HUD_HEALTHBAR_POS_Y = 136.0f;
		HUD_HEALTHBAR_SIZE_X = 114.1f;
		HUD_HEALTHBAR_SIZE_Y = 40.1f;

		HUD_ARMOURBAR_POS_X = 297.0f;
		HUD_ARMOURBAR_POS_Y = 91.0f;
		HUD_ARMOURBAR_SIZE_X = 114.1f;
		HUD_ARMOURBAR_SIZE_Y = 40.1f;

		HUD_BREATH_POS_X_2 = 120.0f;

		HUD_MONEY_POS_X = 61.5f;
		HUD_MONEY_POS_Y = 179.5f;
		HUD_MONEY_SIZE_X = 1.332f;
		HUD_MONEY_SIZE_Y = 2.50f;

		HUD_GLOBAL_SHADOW = 0;

		HUD_CLOCK_POS_X = 182.0f;
		HUD_CLOCK_POS_Y = 40.5f;
		HUD_CLOCK_SIZE_X = 1.16f;
		HUD_CLOCK_SIZE_Y = 2.32f;

		HUD_STAR_POS_X = 100.0f;
		HUD_STAR_POS_Y = 222.0f;
		HUD_STAR_SIZE_X = 38.0f;
		HUD_STAR_SIZE_Y = 35.0f;

		HUD_STAR_SPACE = 40.0f;

		HUD_AREA_POS_X = 50.0f;
		HUD_AREA_POS_Y = 130.0f;
		HUD_AREA_SIZE_X = 1.3f;
		HUD_AREA_SIZE_Y = 3.0f;

		HUD_VEHICLE_POS_X = 50.0f;
		HUD_VEHICLE_POS_Y = 180.0f;
		HUD_VEHICLE_SIZE_X = 1.3f;
		HUD_VEHICLE_SIZE_Y = 3.0f;

		HUD_AREA_FONT = FONT_GOTHIC;

		HUD_VEHICLE_FONT = FONT_GOTHIC;

		hudfilename = ".\\CLASSICHUD\\TXD\\HUD\\HUD_VCS.TXD";
		fontfilename = ".\\CLASSICHUD\\TXD\\FONTS\\FONTS_STORIES.TXD";
		fontdatafilename = ".\\CLASSICHUD\\DATA\\FONTS\\FONTS_STORIES.DAT";

		SetTextBasedOnStyle = "ClassicHud - VCS";
	}
	if (Style == 4) {
		// hudColor CUSTOM
		HUD_COLOUR_HEALTH_R = hudcolors.ReadRGBA("HEALTH", "HUD_COLOUR_HEALTH_R", 255);
		HUD_COLOUR_HEALTH_G = hudcolors.ReadRGBA("HEALTH", "HUD_COLOUR_HEALTH_G", 255);
		HUD_COLOUR_HEALTH_B = hudcolors.ReadRGBA("HEALTH", "HUD_COLOUR_HEALTH_B", 255);
		HUD_COLOUR_HEALTH_A = 255;

		HUD_COLOUR_ARMOUR_R = hudcolors.ReadRGBA("ARMOUR", "HUD_COLOUR_ARMOUR_R", 255);
		HUD_COLOUR_ARMOUR_G = hudcolors.ReadRGBA("ARMOUR", "HUD_COLOUR_ARMOUR_G", 255);
		HUD_COLOUR_ARMOUR_B = hudcolors.ReadRGBA("ARMOUR", "HUD_COLOUR_ARMOUR_B", 255);
		HUD_COLOUR_ARMOUR_A = 255;

		HUD_COLOUR_BREATH_R = hudcolors.ReadRGBA("BREATH", "HUD_COLOUR_BREATH_R", 255);
		HUD_COLOUR_BREATH_G = hudcolors.ReadRGBA("BREATH", "HUD_COLOUR_BREATH_G", 255);
		HUD_COLOUR_BREATH_B = hudcolors.ReadRGBA("BREATH", "HUD_COLOUR_BREATH_B", 255);
		HUD_COLOUR_BREATH_A = 255;

		HUD_COLOUR_MONEY_R = hudcolors.ReadRGBA("MONEY", "HUD_COLOUR_MONEY_R", 255);
		HUD_COLOUR_MONEY_G = hudcolors.ReadRGBA("MONEY", "HUD_COLOUR_MONEY_G", 255);
		HUD_COLOUR_MONEY_B = hudcolors.ReadRGBA("MONEY", "HUD_COLOUR_MONEY_B", 255);
		HUD_COLOUR_MONEY_A = 255;

		HUD_COLOUR_CLOCK_R = hudcolors.ReadRGBA("CLOCK", "HUD_COLOUR_CLOCK_R", 255);
		HUD_COLOUR_CLOCK_G = hudcolors.ReadRGBA("CLOCK", "HUD_COLOUR_CLOCK_G", 255);
		HUD_COLOUR_CLOCK_B = hudcolors.ReadRGBA("CLOCK", "HUD_COLOUR_CLOCK_B", 255);
		HUD_COLOUR_CLOCK_A = 255;

		HUD_DROPCOLOUR_CLOCK_R = 0;
		HUD_DROPCOLOUR_CLOCK_G = 0;
		HUD_DROPCOLOUR_CLOCK_B = 0;
		HUD_DROPCOLOUR_CLOCK_A = 255;

		HUD_COLOUR_AMMO_R = hudcolors.ReadRGBA("AMMO", "HUD_COLOUR_AMMO_R", 255);
		HUD_COLOUR_AMMO_G = hudcolors.ReadRGBA("AMMO", "HUD_COLOUR_AMMO_G", 255);
		HUD_COLOUR_AMMO_B = hudcolors.ReadRGBA("AMMO", "HUD_COLOUR_AMMO_B", 255);

		HUD_DROPCOLOUR_AMMO_R = hudcolors.ReadRGBA("AMMO", "HUD_DROPCOLOUR_AMMO_R", 255);
		HUD_DROPCOLOUR_AMMO_G = hudcolors.ReadRGBA("AMMO", "HUD_DROPCOLOUR_AMMO_G", 255);
		HUD_DROPCOLOUR_AMMO_B = hudcolors.ReadRGBA("AMMO", "HUD_DROPCOLOUR_AMMO_B", 255);

		HUD_COLOUR_STAR_R = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STAR_R", 255);
		HUD_COLOUR_STAR_G = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STAR_G", 255);
		HUD_COLOUR_STAR_B = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STAR_B", 255);

		if (DrawBarChart == 1) {
			HUD_COLOUR_SHADOWSTAR_A = 0;
			HUD_COLOUR_SHADOWSTAR_A2 = 200;
			HUD_COLOUR_SHADOWSTAR_A3 = 0;
		}
		else {
			HUD_COLOUR_SHADOWSTAR_A = 200;
			HUD_COLOUR_SHADOWSTAR_A2 = 0;
			HUD_COLOUR_SHADOWSTAR_A3 = 0;
		}

		HUD_COLOUR_STARBACK_R = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STARBACK_R", 255);
		HUD_COLOUR_STARBACK_G = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STARBACK_G", 255);
		HUD_COLOUR_STARBACK_B = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STARBACK_B", 255);

		HUD_COLOUR_AREA_R = hudcolors.ReadRGBA("ZONE", "HUD_COLOUR_AREA_R", 255);
		HUD_COLOUR_AREA_G = hudcolors.ReadRGBA("ZONE", "HUD_COLOUR_AREA_G", 255);
		HUD_COLOUR_AREA_B = hudcolors.ReadRGBA("ZONE", "HUD_COLOUR_AREA_B", 255);

		HUD_COLOUR_VEHICLE_R = hudcolors.ReadRGBA("VEHICLE", "HUD_COLOUR_VEHICLE_R", 255);
		HUD_COLOUR_VEHICLE_G = hudcolors.ReadRGBA("VEHICLE", "HUD_COLOUR_VEHICLE_G", 255);
		HUD_COLOUR_VEHICLE_B = hudcolors.ReadRGBA("VEHICLE", "HUD_COLOUR_VEHICLE_B", 255);

		HUD_COLOUR_STATBAR_R = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STATBAR_R", 255);
		HUD_COLOUR_STATBAR_G = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STATBAR_G", 255);
		HUD_COLOUR_STATBAR_B = hudcolors.ReadRGBA("STAR", "HUD_COLOUR_STATBAR_B", 255);

		MAP_COLOUR_ARROW_R = 255;
		MAP_COLOUR_ARROW_G = 255;
		MAP_COLOUR_ARROW_B = 255;

		HUD_COLOUR_WEAPON_A = 255;

		HUD_COLOUR_WB_R = 255;
		HUD_COLOUR_WB_G = 255;
		HUD_COLOUR_WB_B = 255;

		// hudPosition CUSTOM
		
		HUD_WEAPON_POS_X = 180.0f;
		HUD_WEAPON_POS_Y = 48.0f;
		HUD_WEAPON_SIZE_X = 117.0f;
		HUD_WEAPON_SIZE_Y = 115.5f;

		HUD_AMMO_POS_X = 122.5f;
		HUD_AMMO_POS_Y = 132.0f;
		HUD_AMMO_SIZE_X = 0.52f;
		HUD_AMMO_SIZE_Y = 1.35f;

		HUD_AMMO_OUTLINE = 1;

		HUD_HEALTH_POS_X = 196.8f;
		HUD_HEALTH_POS_Y = 118.8f;
		HUD_HEALTH_SIZE_X = 1.35f;
		HUD_HEALTH_SIZE_Y = 2.46f;

		HUD_HEALTHBAR_POS_X = 317.0f;
		HUD_HEALTHBAR_POS_Y = 130.0f;
		HUD_HEALTHBAR_SIZE_X = 120.0f;
		HUD_HEALTHBAR_SIZE_Y = 20.0f;

		HUD_ARMOUR_POS_X = 326.4f;
		HUD_ARMOUR_POS_Y = 118.8f;
		HUD_ARMOUR_SIZE_X = 1.35f;
		HUD_ARMOUR_SIZE_Y = 2.45f;

		HUD_ARMOURBAR_POS_X = 317.0f;
		HUD_ARMOURBAR_POS_Y = 157.0f;
		HUD_ARMOURBAR_SIZE_X = 120.0f;
		HUD_ARMOURBAR_SIZE_Y = 20.0f;

		HUD_BREATH_POS_X = 326.4f;
		HUD_BREATH_POS_X_2 = 127.2f;
		HUD_BREATH_POS_Y = 118.8f;
		HUD_BREATH_SIZE_X = 1.35f;
		HUD_BREATH_SIZE_Y = 2.46f;
		HUD_BREATHICON_POS_X = 448.8f;
		HUD_BREATHICON_POS_Y = 126.0f;
		HUD_BREATHICON_SIZE_X = 33.0f;
		HUD_BREATHICON_SIZE_Y = 30.5f;

		HUD_BREATHBAR_POS_X = 425.0f;
		HUD_BREATHBAR_POS_Y = 130.0f;
		HUD_BREATHBAR_SIZE_X = 100.0f;
		HUD_BREATHBAR_SIZE_Y = 20.0f;

		HUD_HEALTHICON_POS_X = 320.8f;
		HUD_HEALTHICON_POS_Y = 126.0f;
		HUD_HEALTHICON_SIZE_X = 35.0f;
		HUD_HEALTHICON_SIZE_Y = 30.5f;

		HUD_ARMOURICON_POS_X = 448.8f;
		HUD_ARMOURICON_POS_Y = 126.0f;
		HUD_ARMOURICON_SIZE_X = 33.0f;
		HUD_ARMOURICON_SIZE_Y = 30.0f;

		HUD_MONEY_POS_X = 196.8f;
		HUD_MONEY_POS_Y = 78.0f;
		HUD_MONEY_SIZE_X = 1.35f;
		HUD_MONEY_SIZE_Y = 2.46f;

		HUD_CLOCK_POS_X = 196.8f;
		HUD_CLOCK_POS_Y = 40.8f;
		HUD_CLOCK_SIZE_X = 1.35f;
		HUD_CLOCK_SIZE_Y = 2.46f;

		if (DrawBarChart == 1) {
			HUD_STAR_POS_X = 102.0f;
			HUD_STAR_POS_Y = 190.0f;
			HUD_STAR_SIZE_X = 42.0f;
			HUD_STAR_SIZE_Y = 39.0f;

			HUD_STAR_SPACE = 42.0f;

			HUD_GLOBAL_SHADOW = 1;
		}
		else {
			HUD_STAR_POS_X = 228.5f;
			HUD_STAR_POS_Y = 166.0f;
			HUD_STAR_SIZE_X = 38.0f;
			HUD_STAR_SIZE_Y = 35.5f;

			HUD_STAR_SPACE = 42.0f;

			HUD_GLOBAL_SHADOW = 1;
		}

		HUD_AREA_POS_X = 50.0f;
		HUD_AREA_POS_Y = 130.0f;
		HUD_AREA_SIZE_X = 2.0f;
		HUD_AREA_SIZE_Y = 3.3f;

		HUD_VEHICLE_POS_X = 50.0f;
		HUD_VEHICLE_POS_Y = 180.0f;
		HUD_VEHICLE_SIZE_X = 1.3f;
		HUD_VEHICLE_SIZE_Y = 3.0f;
		
		HUD_AREA_FONT = FONT_GOTHIC;
		HUD_VEHICLE_FONT = FONT_MENU;

		hudfilename = ".\\CLASSICHUD\\TXD\\HUD\\HUD_CUSTOM.TXD";
		fontfilename = ".\\CLASSICHUD\\TXD\\FONTS\\FONTS_CUSTOM.TXD";
		fontdatafilename = ".\\CLASSICHUD\\DATA\\FONTS\\FONTS_CUSTOM.DAT";

		SetTextBasedOnStyle = "ClassicHud - CUSTOM";
	}
}