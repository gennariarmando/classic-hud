#include "plugin.h"
#include "ScreenAddons.h"
#include "ClassicHud.h"
#include "CFontNew.h"
#include "CHudColoursNew.h"
#include "CHudNew.h"
#include "CHudNumbers.h"
#include "CProgressBar.h"
#include "CRadarNew.h"
#include "Settings.h"
#include "debugmenu_public.h"

using namespace plugin;

ClassicHud classichud;
eGameMode ClassicHud::ms_nGameMode;
char *ClassicHud::ms_nGamePrefix;
bool ClassicHud::ms_bReload;
bool UpAndRunning;

DebugMenuAPI gDebugMenuAPI;
void(*DebugMenuProcess)(void);
void(*DebugMenuRender)(void);
static void stub(void) { }
void DebugMenuInit();
void ProcessDebugOptions();

ClassicHud::ClassicHud() {	
	s.readIni();

	if (!s.m_bEnable)
		return;

	// Set game mode.
	if (!UpAndRunning) {
		ClassicHud::ms_bReload = true;
		InitGameMode(s.m_nGameMode);

		/*
			Events:
			Init.
			Reload.
			Render.
			Shutdown.
		*/

		Events::initRwEvent += [] {
			DebugMenuInit();
			CFontNew::Initialise();
			HudColourNew.Initialise();
			CHudNew::Initialise();
			CHudNumbers::Initialise();
			CProgressBar::Initialise();
			CRadarNew::Initialise();
		};

		ProcessDebugOptions();

		Events::gameProcessEvent += [] {
			s.readDat();
			s.readBlipsDat();
		};

		// Draw.
		Events::drawHudEvent += [] {
			CHudNew::Draw();
		};

		Events::drawAfterFadeEvent += [] {
			CHudNew::DrawAfterFade();
		};

		// Shutdown.
		Events::shutdownRwEvent += [] {
			CFontNew::Shutdown();
			HudColourNew.Shutdown();
			CHudNew::Shutdown();
			CHudNumbers::Shutdown();
			CProgressBar::Shutdown();
			CRadarNew::Shutdown();
		};

		UpAndRunning = true;
	}
}

void ClassicHud::InitGameMode(std::string gameMode, bool reInit) {
	char *ModeNames[GAMEMODE_TOTALMODES] = { "", "III", "VC", "SA", "LCS", "VCS", "IV", "UG" };

	SetGameMode(GAMEMODE_NULL);
	if (GetGameMode() == GAMEMODE_NULL) {
		for (int i = 0; i < GAMEMODE_TOTALMODES; i++) {
			if (gameMode == ModeNames[i]) {
				SetGameMode((eGameMode)i);
				ms_nGamePrefix = ModeNames[i];

				if (reInit)
					ReInitialise();
			}
		}
	}
}

void ClassicHud::ReInitialise() {
	if (!ClassicHud::ms_bReload) { // TODO: find an automatized check.
		CFontNew::Shutdown();
		CFontNew::Initialise();

		HudColourNew.Shutdown();
		HudColourNew.Initialise();

		CHudNew::Shutdown();
		CHudNew::Initialise();

		CHudNumbers::Shutdown();
		CHudNumbers::Initialise();

		CProgressBar::Shutdown();
		CProgressBar::Initialise();

		CRadarNew::Shutdown();
		CRadarNew::Initialise();

		ClassicHud::ms_bReload = true;
	}
}

void DebugMenuInit() {
	if (DebugMenuLoad()) {
		DebugMenuProcess = (void(*)(void))GetProcAddress(gDebugMenuAPI.module, "DebugMenuProcess");
		DebugMenuRender = (void(*)(void))GetProcAddress(gDebugMenuAPI.module, "DebugMenuRender");
	}
	if (DebugMenuProcess == NULL || DebugMenuRender == NULL) {
		DebugMenuProcess = stub;
		DebugMenuRender = stub;
	}
}

void ProcessDebugOptions() {
	if (DebugMenuLoad()) {
		// Main
		DebugMenuAddVarBool8("ClassicHud|Main", "m_bEnable", (int8_t*)&s.m_bEnable, NULL);
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "III", []() { ClassicHud::InitGameMode("III", true); });
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "VC", []() { ClassicHud::InitGameMode("VC", true); });
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "SA", []() { ClassicHud::InitGameMode("SA", true); });
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "LCS", []() { ClassicHud::InitGameMode("LCS", true); });
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "VCS", []() { ClassicHud::InitGameMode("VCS", true); });
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "IV", []() { ClassicHud::InitGameMode("IV", true); });
		DebugMenuAddCmd("ClassicHud|Main|m_nGameMode", "UG", []() { ClassicHud::InitGameMode("UG", true); });

		// Hud
		DebugMenuAddVarBool8("ClassicHud|Hud", "bMinimalMoneyCounter", (int8_t*)&s.bMinimalMoneyCounter, NULL);
		DebugMenuAddVarBool8("ClassicHud|Hud", "bCrosshairRadius", (int8_t*)&s.bCrosshairRadius, NULL);

		// Radar
		DebugMenuAddVarBool8("ClassicHud|Radar", "bRectangularRadar", (int8_t*)&s.bRectangularRadar, NULL);

		//DebugMenuAddCmd("ClassicHud|", "Reload", []() { ClassicHud::InitGameMode(ClassicHud::ms_nGamePrefix, true); });
	}
}