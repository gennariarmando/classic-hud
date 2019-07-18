#pragma once
#include "CSprite2d.h"

class CRadarNew {
public:
	static bool ms_bSpritesLoaded;

public:
	static void Initialise();
	static void Shutdown();
	static void DrawRadarCircle();

public:
	static void InjectPatches();
};