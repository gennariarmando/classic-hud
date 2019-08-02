#include "plugin.h"
#include "Settings.h"

using namespace plugin;

Settings s;
int m_nBlipsCounter;
char *m_pBlipNames[MAX_BLIPS];

void Settings::readIni() {
	config_file ini(PLUGIN_PATH("classichud.ini"));

	// Main
	READ_BOOL(ini, m_bEnable, "m_bEnable", true);
	READ_STR(ini, m_nGameMode, "m_nGameMode", "SA");
	READ_BOOL(ini, m_bAdjustProportions, "m_bAdjustProportions", true);

	// Hud
	READ_BOOL(ini, bMinimalMoneyCounter, "bMinimalMoneyCounter", false);
	READ_BOOL(ini, bCrosshairRadius, "bCrosshairRadius", false);

	// Radar
	READ_BOOL(ini, bRectangularRadar, "bRectangularRadar", false);
}

void Settings::readDat() {
	// Hud data
	config_file hud(PLUGIN_PATH(SetFileWithPrefix("classichud\\data\\", "hud.dat")));

	CRect rect = CRect(0.0f, 0.0f, 0.0f, 0.0f);
	READ_RECT(hud, m_fClock, "HUD_CLOCK", rect);
	READ_RECT(hud, m_fMoney, "HUD_CASH", rect);
	READ_RECT(hud, m_fHealth, "HUD_HEALTH", rect);
	READ_RECT(hud, m_fArmour, "HUD_ARMOUR", rect);
	READ_RECT(hud, m_fBreath, "HUD_BREATH", rect);
	READ_RECT(hud, m_fWeapon, "HUD_WEAPON_ICON", rect);
	READ_RECT(hud, m_fAmmo, "HUD_AMMO", rect);
	READ_RECT(hud, m_fWanted, "HUD_WANTED", rect);
	READ_RECT(hud, m_fZoneName, "HUD_ZONE_NAME", rect);
	READ_RECT(hud, m_fVehicleName, "HUD_VEHICLE_NAME", rect);
	READ_RECT(hud, m_fRadioName, "HUD_RADIO_NAME", rect);
	READ_RECT(hud, m_fTextBox, "HUD_HELP_TEXT", rect);
	READ_RECT(hud, m_fSubtitles, "HUD_SUBTITLES", rect);
	READ_RECT(hud, m_fWastedBustedText, "HUD_WB_MESSAGE", rect);
	READ_RECT(hud, m_fSuccessFailedMessage, "HUD_MISSION_RESULT", rect);
	READ_RECT(hud, m_fMissionTitle, "HUD_MISSION_TITLE", rect);

	READ_RECT(hud, m_fRadarMap, !bRectangularRadar ? "HUD_RADAR_MAP" : "HUD_RADAR_RECT_MAP", rect);
	READ_RECT(hud, m_fRadarSprites, !bRectangularRadar ? "HUD_RADAR_SPRITES" : "HUD_RADAR_RECT_SPRITES", rect);
	READ_RECT(hud, m_fRadarBlips, "HUD_RADAR_BLIPS");
}

void Settings::readBlipsDat() {
	std::ifstream file(PLUGIN_PATH("classichud\\data\\shared\\blips.dat"));
	if (file.is_open()) {
		char str[64];
		unsigned int iconID;

		for (std::string line; getline(file, line);) {
			if (!line[0] || line[0] == '#')
				continue;

			str[0] = NULL;
			sscanf(line.c_str(), "%d %s", &iconID, &str);

			if (iconID >= MAX_BLIPS) {
				Error("blips.dat contains more than %d lines.", iconID);
				exit(0);
			}

			int l = strlen(str);
			char* strAlloc = new char[l + 1];
			strcpy(strAlloc, str);

			m_pBlipNames[iconID] = strAlloc;
		}
	}
	else {
		Error("blips.dat in: 'classichud\\data\\shared' not found.");
		exit(0);
	}
}