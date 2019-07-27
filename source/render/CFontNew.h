#pragma once
#include "CFont.h"

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

	// GAMEMODE_LCS
	LCS_FONT_BANK = 0,
	LCS_FONT_SUBTITLES = 1,
	LCS_FONT_PRICEDOWN = 2,

	// GAMEMODE_VCS
	VCS_FONT_RAGE = 0,
	VCS_FONT_SUBTITLES = 1,
	VCS_FONT_PRICEDOWN = 2,
};

class CFontNew {
public:
	static bool ms_bFontsLoaded;

public:
	CFontNew();

	static void Initialise();
	static void Shutdown();
	static void SetFontStyle(int Font);
};