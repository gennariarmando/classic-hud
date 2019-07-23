#pragma once
#define READ_BOOL(set, a, b, c) a = set[b].asBool(c)
#define READ_INT(set, a, b, c) a = set[b].asInt(c)
#define READ_STR(set, a, b, c) a = set[b].asString(c)
#define READ_FLOAT(set, a, b, c) a = set[b].asFloat(c)
#define READ_RECT(set, a, b, c) a = set[b].asRect(c)
#define READ_RGBA(set, a, b, c) a = set[b].asRGBA(c)

#define MAX_BLIPS 300

#include "ClassicHud.h"

class Settings {
public:
	bool m_bEnable;
	std::string m_nGameMode, m_nGameModeTemp;
	bool bMinimalMoneyCounter;
	float m_fHudW, m_fHudH;
	float m_fRadarW, m_fRadarH;
	float m_fSubsW, m_fSubsH;
 
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
	CRect m_fSubtitles, m_fSubtitlesTemp;
	CRect m_fWastedBustedText, m_fWastedBustedTextTemp;
	CRect m_fSuccessFailedMessage, m_fSuccessFailedMessageTemp;
	CRect m_fMissionTitle, m_fMissionTitleTemp;

	CRect m_fRadarMap, m_fRadarMapTemp;
	CRect m_fRadarSprites, m_fRadarSpritesTemp;


	CRGBA HUD_COLOUR_CLOCK, HUD_COLOUR_CLOCK_TEMP;
	CRGBA HUD_COLOUR_CASH, HUD_COLOUR_CASH_TEMP;
	CRGBA HUD_COLOUR_HEALTH, HUD_COLOUR_HEALTH_TEMP;
	CRGBA HUD_COLOUR_ARMOUR, HUD_COLOUR_ARMOUR_TEMP;
	CRGBA HUD_COLOUR_BREATH, HUD_COLOUR_BREATH_TEMP;
	CRGBA HUD_COLOUR_WEAPON_ICON, HUD_COLOUR_WEAPON_ICON_TEMP;
	CRGBA HUD_COLOUR_AMMO, HUD_COLOUR_AMMO_TEMP;
	CRGBA HUD_COLOUR_WANTED_N, HUD_COLOUR_WANTED_A, HUD_COLOUR_WANTED_N_TEMP, HUD_COLOUR_WANTED_A_TEMP;
	CRGBA HUD_COLOUR_ZONE_NAME, HUD_COLOUR_ZONE_NAME_TEMP;
	CRGBA HUD_COLOUR_VEHICLE_NAME, HUD_COLOUR_VEHICLE_NAME_TEMP;
	CRGBA HUD_COLOUR_RADIO_NAME_N, HUD_COLOUR_RADIO_NAME_A, HUD_COLOUR_RADIO_NAME_N_TEMP, HUD_COLOUR_RADIO_NAME_A_TEMP;
	CRGBA HUD_COLOUR_HELP_TEXT, HUD_COLOUR_HELP_TEXT_TEMP;
	CRGBA HUD_COLOUR_HELP_BOX, HUD_COLOUR_HELP_BOX_TEMP;
	CRGBA HUD_COLOUR_SUBTITLES, HUD_COLOUR_SUBTITLES_TEMP;
	CRGBA HUD_COLOUR_WASTED, HUD_COLOUR_WASTED_TEMP;
	CRGBA HUD_COLOUR_BUSTED, HUD_COLOUR_BUSTED_TEMP;
	CRGBA HUD_COLOUR_MISSION_RESULT, HUD_COLOUR_MISSION_RESULT_TEMP;
	CRGBA HUD_COLOUR_MISSION_TITLE, HUD_COLOUR_MISSION_TITLE_TEMP;

	unsigned int m_nBlipsCounter;
	char *m_pBlipNames[MAX_BLIPS];

public:
	void readIni();
	void readDat();
	void readBlipsDat();

	static char *SetFileWithPrefix(char *folder, char *file) {
		static char FileName[128];
		strcpy_s(FileName, folder);
		strcat_s(FileName, ClassicHud::GetGamePrefix());
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