#include "plugin.h"
#include "Settings.h"

using namespace plugin;

Settings s;
int m_nBlipsCounter;
char *m_pBlipNames[MAX_BLIPS];

void Settings::readIni() {
	config_file ini(PLUGIN_PATH("classichud.ini"));

	READ_BOOL(ini, m_bEnable, "m_bEnable", true);
	READ_STR(ini, m_nGameMode, "m_nGameMode", "SA");
	READ_FLOAT(ini, m_fHudW, "m_fHudWidthScale", 1.0f);
	READ_FLOAT(ini, m_fHudH, "m_fHudHeightScale", 1.0f);
	READ_FLOAT(ini, m_fRadarW, "m_fRadarWidthScale", 1.0f);
	READ_FLOAT(ini, m_fRadarH, "m_fRadarHeightScale", 1.0f);
	READ_FLOAT(ini, m_fSubsW, "m_fSubtitlesWidthScale", 1.0f);
	READ_FLOAT(ini, m_fSubsH, "m_fSubtitlesHeightScale", 1.0f);
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

	READ_RECT(hud, m_fRadarMap, "HUD_RADAR_MAP", rect);
	READ_RECT(hud, m_fRadarSprites, "HUD_RADAR_SPRITES", rect);

	// Color data
	config_file hudColor(PLUGIN_PATH(SetFileWithPrefix("classichud\\data\\", "hudColor.dat")));

	CRGBA rgba = CRGBA(0, 0, 0, 255);
	READ_RGBA(hudColor, HUD_COLOR_CLOCK, "HUD_COLOR_CLOCK", rgba);
	READ_RGBA(hudColor, HUD_COLOR_CASH, "HUD_COLOR_CASH", rgba);
	READ_RGBA(hudColor, HUD_COLOR_HEALTH, "HUD_COLOR_HEALTH", rgba);
	READ_RGBA(hudColor, HUD_COLOR_ARMOUR, "HUD_COLOR_ARMOUR", rgba);
	READ_RGBA(hudColor, HUD_COLOR_BREATH, "HUD_COLOR_BREATH", rgba);
	READ_RGBA(hudColor, HUD_COLOR_WEAPON_ICON, "HUD_COLOR_WEAPON_ICON", rgba);
	READ_RGBA(hudColor, HUD_COLOR_AMMO, "HUD_COLOR_AMMO", rgba);
	READ_RGBA(hudColor, HUD_COLOR_WANTED_N, "HUD_COLOR_WANTED_N", rgba);
	READ_RGBA(hudColor, HUD_COLOR_WANTED_A, "HUD_COLOR_WANTED_A", rgba);
	READ_RGBA(hudColor, HUD_COLOR_ZONE_NAME, "HUD_COLOR_ZONE_NAME", rgba);
	READ_RGBA(hudColor, HUD_COLOR_VEHICLE_NAME, "HUD_COLOR_VEHICLE_NAME", rgba);
	READ_RGBA(hudColor, HUD_COLOR_RADIO_NAME_N, "HUD_COLOR_RADIO_NAME_N", rgba);
	READ_RGBA(hudColor, HUD_COLOR_RADIO_NAME_A, "HUD_COLOR_RADIO_NAME_A", rgba);
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