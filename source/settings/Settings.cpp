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
	READ_BOOL(ini, bMinimalMoneyCounter, "bMinimalMoneyCounter", false);

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
	READ_RECT(hud, m_fWastedBustedText, "HUD_WB_MESSAGE", rect);
	READ_RECT(hud, m_fSuccessFailedMessage, "HUD_MISSION_RESULT", rect);
	READ_RECT(hud, m_fMissionTitle, "HUD_MISSION_TITLE", rect);

	READ_RECT(hud, m_fRadarMap, "HUD_RADAR_MAP", rect);
	READ_RECT(hud, m_fRadarSprites, "HUD_RADAR_SPRITES", rect);

	// Color data
	config_file hudColor(PLUGIN_PATH(SetFileWithPrefix("classichud\\data\\", "hudColor.dat")));

	CRGBA rgba = CRGBA(0, 0, 0, 255);
	READ_RGBA(hudColor, HUD_COLOUR_CLOCK, "HUD_COLOUR_CLOCK", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_CASH, "HUD_COLOUR_CASH", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_HEALTH, "HUD_COLOUR_HEALTH", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_ARMOUR, "HUD_COLOUR_ARMOUR", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_BREATH, "HUD_COLOUR_BREATH", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_WEAPON_ICON, "HUD_COLOUR_WEAPON_ICON", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_AMMO, "HUD_COLOUR_AMMO", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_WANTED_N, "HUD_COLOUR_WANTED_N", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_WANTED_A, "HUD_COLOUR_WANTED_A", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_ZONE_NAME, "HUD_COLOUR_ZONE_NAME", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_VEHICLE_NAME, "HUD_COLOUR_VEHICLE_NAME", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_RADIO_NAME_N, "HUD_COLOUR_RADIO_NAME_N", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_RADIO_NAME_A, "HUD_COLOUR_RADIO_NAME_A", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_RADIO_NAME_A, "HUD_COLOUR_RADIO_NAME_A", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_RADIO_NAME_A, "HUD_COLOUR_RADIO_NAME_A", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_HELP_TEXT, "HUD_COLOUR_HELP_TEXT", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_HELP_BOX, "HUD_COLOUR_HELP_BOX", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_SUBTITLES, "HUD_COLOUR_SUBTITLES", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_WASTED, "HUD_COLOUR_WASTED", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_BUSTED, "HUD_COLOUR_BUSTED", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_MISSION_RESULT, "HUD_COLOUR_MISSION_RESULT", rgba);
	READ_RGBA(hudColor, HUD_COLOUR_MISSION_TITLE, "HUD_COLOUR_MISSION_TITLE", rgba);

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