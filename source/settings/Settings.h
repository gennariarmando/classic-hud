#pragma once
#define READ_BOOL(set, a, b, c) a = set[b].asBool(c)
#define READ_INT(set, a, b, c) a = set[b].asInt(c)
#define READ_STR(set, a, b, c) a = set[b].asString(c)
#define READ_RECT(set, a, b, c) a = set[b].asRect(c)

#define MAX_BLIPS 300

#include "ClassicHud.h"

class Settings {
public:
	// Main
	bool m_bEnable;
	std::string m_nGameMode, m_nGameModeTemp;
	bool m_bAdjustProportions;

	// Hud
	bool bMinimalMoneyCounter;
	bool bCrosshairRadius;

	// Radar
	bool bRectangularRadar;
 
	// Data
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
	CRect m_fRadarBlips, m_fRadarBlipsTemp;

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