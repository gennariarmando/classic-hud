#pragma once
#define READ_BOOL(set, a, b, c) a = set[b].asBool(c)
#define READ_INT(set, a, b, c) a = set[b].asInt(c)
#define READ_STR(set, a, b, c) a = set[b].asString(c)
#define READ_RECT(set, a, b, c) a = set[b].asRect(c)
#define READ_RGBA(set, a, b, c) a = set[b].asRGBA(c)

class Settings {
public:
	bool m_bEnable;
	std::string m_nGameMode, m_nGameModeTemp;
 
	CRect m_fClock, m_fClockTemp;
	CRect m_fMoney, m_fMoneyTemp;
	CRect m_fHealth, m_fHealthTemp;
	CRect m_fArmour, m_fArmourTemp;
	CRect m_fBreath, m_fBreathTemp;
	CRect m_fWeapon, m_fWeaponTemp;
	CRect m_fAmmo, m_fAmmoTemp;
	CRect m_fWanted, m_fWantedTemp;
	CRect m_fZoneName, m_fZoneNameTemp;
	CRect m_fVehicleName, m_fVehicleNameTemp;
	CRect m_fRadioName, m_fRadioNameTemp;
	CRect m_fTextBox, m_fTextBoxTemp;

	CRGBA HUD_COLOR_CLOCK, HUD_COLOR_CLOCK_TEMP;
	CRGBA HUD_COLOR_CASH, HUD_COLOR_CASH_TEMP;
	CRGBA HUD_COLOR_HEALTH, HUD_COLOR_HEALTH_TEMP;
	CRGBA HUD_COLOR_ARMOUR, HUD_COLOR_ARMOUR_TEMP;
	CRGBA HUD_COLOR_BREATH, HUD_COLOR_BREATH_TEMP;
	CRGBA HUD_COLOR_WEAPON_ICON, HUD_COLOR_WEAPON_ICON_TEMP;
	CRGBA HUD_COLOR_AMMO, HUD_COLOR_AMMO_TEMP;
	CRGBA HUD_COLOR_WANTED_N, HUD_COLOR_WANTED_A, HUD_COLOR_WANTED_N_TEMP, HUD_COLOR_WANTED_A_TEMP;
	CRGBA HUD_COLOR_ZONE_NAME, HUD_COLOR_ZONE_NAME_TEMP;
	CRGBA HUD_COLOR_VEHICLE_NAME, HUD_COLOR_VEHICLE_NAME_TEMP;
	CRGBA HUD_COLOR_RADIO_NAME_N, HUD_COLOR_RADIO_NAME_A, HUD_COLOR_RADIO_NAME_N_TEMP, HUD_COLOR_RADIO_NAME_A_TEMP;

	int m_nBlipsCounter;
	char *m_pBlipNames[64];

public:
	void readIni();
	void readDat();
	void readBlipsDat();

	static char *SetFileWithPrefix(char *folder, char *file) {
		static char FileName[128];
		strcpy_s(FileName, folder);
		strcat_s(FileName, CHudNew::GetGamePrefix());
		strcat_s(FileName, "_");
		strcat_s(FileName, file);
		puts(FileName);
		return FileName;
	}

	static char *SetSharedFile(char *folder, char *file) {
		static char FileName[128];
		strcpy_s(FileName, folder);
		strcat_s(FileName, file);
		puts(FileName);
		return FileName;
	}
};

extern Settings s;