#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

enum eGameMode {
	GAMEMODE_NULL,
	GAMEMODE_III,
	GAMEMODE_VC,
	GAMEMODE_SA,
	GAMEMODE_LCS,
	GAMEMODE_VCS,
	GAMEMODE_IV,
	GAMEMODE_UG,
	GAMEMODE_TOTALMODES
};

class ClassicHud {
public:
	static eGameMode ms_nGameMode;
	static char *ms_nGamePrefix;
	static bool ms_bReload;

public:
	ClassicHud();

	static void InitGameMode(std::string gameMode, bool reInit = false);
	static void ReInitialise();
	static eGameMode GetGameMode() { return ms_nGameMode; }
	static void SetGameMode(eGameMode Mode) { ms_nGameMode = Mode; }
	static char *GetGamePrefix() { return ms_nGamePrefix; }
};
