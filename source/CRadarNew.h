#pragma once
#include "CSprite2d.h"

enum eRadarSpritesNew {
	RADAR_DISC,
	RADAR_TOTALSPRITES
};

class CRadarNew {
public:
	static CSprite2d RadarSprites[RADAR_TOTALSPRITES];
	static CSprite2d BlipsSprites[64];
	static bool ms_bSpritesLoaded;

public:
	static void Initialise();
	static void Shutdown();
	static void DrawRadarCircle();
	static void DrawRadarSprite(unsigned int iconID, float x, float y, unsigned int alpha);

public:
	static void InjectPatches();
};