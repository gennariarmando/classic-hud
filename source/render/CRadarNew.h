#pragma once
#define RADAR_WIDTH_SCALE 0.82f
#define RADAR_HEIGHT_SCALE 1.0f

#include "CSprite2d.h"
#include "Settings.h"

enum eRadarSpritesNew {
	RADAR_DISC,
	RADAR_RECT,
	RADAR_RING_FRONT,
	RADAR_RING_BACK,
	RADAR_RING_MASK,
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
	static void DrawRadarBeforeHud();
	static void DrawRadarAfterHud();
	static void DrawRadarSpritesBack(float x, float y, float w, float h);
	static void DrawRadarSprites(float x, float y, float w, float h);
	static void DrawRadarNorth();
	static void DrawRadarCentre();
	static void DrawRadarBlip(unsigned char iconID, float x, float y, unsigned int alpha);
	static void TransformRadarPointToScreenSpace(CVector2D &out, CVector2D &in);
	static void DrawRadarMask();
	static float LimitRadarPoint(CVector2D *pos);
};