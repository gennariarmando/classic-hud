#include "plugin.h"
#include "CHudNew.h"
#include "Settings.h"

using namespace plugin;

Settings s;

void Settings::readIni() {
	config_file ini(PLUGIN_PATH("classichud\\settings.ini"));

	READ_BOOL(ini, m_bEnable, "m_bEnable", false);
	READ_STR(ini, m_nGameMode, "m_nGameMode", "");
}

void Settings::readDat() {
	// Data
	char FileName[128];
	strcpy_s(FileName, "classichud\\data\\"); // Set folder,
	strcat_s(FileName, CHudNew::GetGamePrefix()); // game prefix,
	strcat_s(FileName, "_");
	strcat_s(FileName, "hud.dat"); // file name.
	puts(FileName);
	config_file dat(PLUGIN_PATH(FileName));

	CRect rect = CRect(0.0f, 0.0f, 0.0f, 0.0f);
	READ_RECT(dat, m_fClock, "HUD_CLOCK", rect);
	READ_RECT(dat, m_fMoney, "HUD_CASH", rect);
	READ_RECT(dat, m_fHealth, "HUD_HEALTH", rect);
	READ_RECT(dat, m_fArmour, "HUD_ARMOUR", rect);
	READ_RECT(dat, m_fBreath, "HUD_BREATH", rect);
	READ_RECT(dat, m_fWeapon, "HUD_WEAPON_ICON", rect);
	READ_RECT(dat, m_fAmmo, "HUD_AMMO", rect);
	READ_RECT(dat, m_fWanted, "HUD_WANTED", rect);
	READ_RECT(dat, m_fZoneName, "HUD_ZONE_NAME", rect);
	READ_RECT(dat, m_fVehicleName, "HUD_VEHICLE_NAME", rect);
}