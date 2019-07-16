#pragma once
#define READ_BOOL(set, a, b, c) a = set[b].asBool(c)
#define READ_INT(set, a, b, c) a = set[b].asInt(c)
#define READ_STR(set, a, b, c) a = set[b].asString(c)
#define READ_RECT(set, a, b, c) a = set[b].asRect(c)

class Settings {
public:
	bool m_bEnable;
	std::string m_nGameMode;

	CRect 
	m_fClock, m_fMoney, m_fHealth, m_fArmour, m_fBreath, m_fWeapon, m_fAmmo, m_fWanted, m_fZoneName, m_fVehicleName;

public:
	void readIni();
	void readDat();
};

extern Settings s;