#pragma once
#include "CFont.h"

struct tFontData {
	char m_propValues[208];
	char m_spaceValue;
	char m_unpropValue;
};

class CFontNew {
public:
	static bool ms_bFontsLoaded;

public:
	static void Initialise();
	static void Shutdown();
	static void SetFontStyle(eFontStyle Font);
	static void InjectPatches();
};