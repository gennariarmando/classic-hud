#include "plugin.h"
#include "ClassicHud.h"
#include "CFontNew.h"
#include "CHudNew.h"
#include "CHudNumbers.h"
#include "CProgressBar.h"
#include "CRadarNew.h"
#include "Settings.h"

using namespace plugin;

ClassicHud classichud;
eGameMode ClassicHud::ms_nGameMode;
char *ClassicHud::ms_nGamePrefix;
bool ClassicHud::ms_bReload;
bool UpAndRunning;

ClassicHud::ClassicHud() {
	s.readIni();

	if (!s.m_bEnable)
		return;

	// Set game mode.
	SetGameMode(GAMEMODE_NULL);
	char *ModeNames[GAMEMODE_TOTALMODES] = { "", "III", "VC", "SA", "LCS", "VCS", "ADVANCE" };
	if (GetGameMode() == GAMEMODE_NULL) {
		for (int i = 0; i < GAMEMODE_TOTALMODES; i++) {
			if (s.m_nGameMode == ModeNames[i]) {
				SetGameMode((eGameMode)i);
				ms_nGamePrefix = ModeNames[i];
			}
		}
	}

	/*
		Events:
		Init.
		Reload.
		Render.
		Shutdown.
	*/
	if (UpAndRunning)
		return;

	Events::initRwEvent += [] {
		CFontNew::Initialise();
		CHudNew::Initialise();
		CHudNumbers::Initialise();
		CProgressBar::Initialise();
		CRadarNew::Initialise();
	};

	// Reinit if menu is active.
	Events::drawMenuBackgroundEvent += ClassicHud::ReInitialise;

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
		CHudNew::Shutdown();
		CHudNumbers::Shutdown();
		CProgressBar::Shutdown();
		CRadarNew::Shutdown();
	};

	UpAndRunning = true;
}

void ClassicHud::ReInitialise() {
	if (!ClassicHud::ms_bReload) { // TODO: find an automatized check.
		s.readIni();
		ClassicHud::ClassicHud();

		CFontNew::Shutdown();
		CFontNew::Initialise();

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
