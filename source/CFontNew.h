#pragma once
#include "CFont.h"

struct tFontData {
	char m_propValues[208];
	char m_spaceValue;
	char m_unpropValue;
};

enum {
	// GAMEMODE_III
	III_FONT_SUBTITLES = 0,
	III_FONT_PRICEDOWN = 1,

	// GAMEMODE_VC
	VC_FONT_RAGE = 0,
	VC_FONT_SUBTITLES = 1,
	VC_FONT_PRICEDOWN = 2,

	// GAMEMODE_SA
	SA_FONT_GOTHIC = 0,
	SA_FONT_SUBTITLES = 1,
	SA_FONT_MENU = 2,
	SA_FONT_PRICEDOWN = 3,
};

class CFontNew {
public:
	static bool ms_bFontsLoaded;

public:
	static void Initialise();
	static void Shutdown();
	static void SetFontStyle(int Font);
	static void InjectPatches();
};