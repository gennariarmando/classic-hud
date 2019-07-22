#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "CSprite2d.h"
#include "Settings.h"

enum eRadarSpritesNew {
	RADAR_DISC,
	RADAR_TOTALSPRITES
};

class CRadarNew {
public:
	static CSprite2d RadarSprites[RADAR_TOTALSPRITES];
	static CSprite2d BlipsSprites[MAX_BLIPS];
	static bool ms_bSpritesLoaded;

public:
	CRadarNew();

	static void Initialise();
	static void Shutdown();
	static void DrawRadar();
	static void DrawRadarSprite(unsigned char iconID, float x, float y, unsigned int alpha);
	static void TransformRadarPointToScreenSpace(CVector2D &out, CVector2D &in);
};