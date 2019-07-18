#include "plugin.h"
#include "CHudNew.h"
#include "Settings.h"

using namespace plugin;

Settings s;

void Settings::readIni() {
	config_file ini(PLUGIN_PATH("classichud\\classichud.ini"));

	READ_BOOL(ini, m_bEnable, "m_bEnable", false);
	READ_STR(ini, m_nGameMode, "m_nGameMode", "");
}

void Settings::readDat() {
	// Data
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