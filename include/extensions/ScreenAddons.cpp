#include "plugin.h"
#include "ScreenAddons.h"
#include "CDraw.h"
#include "Settings.h"

CVector2D ScreenAddons::ms_fScreenMultiplier;

float ScreenAddons::GetAspectRatio() {
#if GTA3
	return *(float*)0x5F53C0;
#elif GTAVC
	return *(float*)0x94DD38;
#elif GTASA
	return CDraw::ms_fAspectRatio;
#endif
}

CVector2D ScreenAddons::GetScreenMult() {
	return ms_fScreenMultiplier;
};

void ScreenAddons::SetScreenMult(float w, float h) {
	ms_fScreenMultiplier.x = s.m_bAdjustProportions ? w : 1.0f;
	ms_fScreenMultiplier.y = s.m_bAdjustProportions ? h : 1.0f;
};