#pragma once
#include "CFont.h"

struct tFontData {
	char m_propValues[208];
	char m_spaceValue;
	char m_unpropValue;
};

class CFontNew {
public:

public:
	static void Initialise();
	static void Shutdown();
	static void InjectPatches();
};