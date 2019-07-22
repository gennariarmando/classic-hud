#include "ScreenAddons.h"
#include "CDraw.h"

float ScreenAddons::ms_fScreenMultiplier[2];

float ScreenAddons::GetAspectRatio() {
#if GTA3
	return *(float*)0x5F53C0;
#elif GTAVC
	return *(float*)0x94DD38;
#elif GTASA
	return CDraw::ms_fAspectRatio;
#endif
}

float ScreenAddons::GetScreenMult(bool h) {
	return ms_fScreenMultiplier[h];
};

void ScreenAddons::SetScreenMult(float w, float h) {
	ms_fScreenMultiplier[0] = w;
	ms_fScreenMultiplier[1] = h;
};
