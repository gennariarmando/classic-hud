#pragma once
#define READ_BOOL(set, a, b, c) a = set[b].asBool(c)
#define READ_INT(set, a, b, c) a = set[b].asInt(c)
#define READ_STR(set, a, b, c) a = set[b].asString(c)
#define READ_RECT(set, a, b, c) a = set[b].asRect(c)
#define READ_RGBA(set, a, b, c) a = set[b].asRGBA(c)

class Settings {
public:
	bool m_bEnable;
	std::string m_nGameMode;
 
	CRect m_fClock;
	CRect m_fMoney;
	CRect m_fHealth;
	CRect m_fArmour; 
	CRect m_fBreath; 
	CRect m_fWeapon;
	CRect m_fAmmo;
	CRect m_fWanted;
	CRect m_fZoneName; 
	CRect m_fVehicleName;
	CRect m_fRadioName;

	CRGBA HUD_COLOR_CLOCK;
	CRGBA HUD_COLOR_CASH;
	CRGBA HUD_COLOR_HEALTH;
	CRGBA HUD_COLOR_ARMOUR;
	CRGBA HUD_COLOR_BREATH;
	CRGBA HUD_COLOR_WEAPON_ICON;
	CRGBA HUD_COLOR_AMMO;
	CRGBA HUD_COLOR_WANTED_N, HUD_COLOR_WANTED_A;
	CRGBA HUD_COLOR_ZONE_NAME;
	CRGBA HUD_COLOR_VEHICLE_NAME;
	CRGBA HUD_COLOR_RADIO_NAME_N, HUD_COLOR_RADIO_NAME_A;


public:
	void readIni();
	void readDat();

	static char *SetFileWithPrefix(char *folder, char *file) {
		static char FileName[128];
		strcpy_s(FileName, folder);
		strcat_s(FileName, CHudNew::GetGamePrefix());
		strcat_s(FileName, "_");
		strcat_s(FileName, file);
		puts(FileName);
		return FileName;
	}
};

extern Settings s;