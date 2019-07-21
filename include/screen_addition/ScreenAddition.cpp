#include "ScreenAddition.h"
#include "CDraw.h"

float ScreenAddition::ms_fScreenMultiplier[2];

float ScreenAddition::GetAspectRatio() {
#if GTA3
	return *(float*)0x5F53C0;
#elif GTAVC
	return *(float*)0x94DD38;
#elif GTASA
	return CDraw::ms_fAspectRatio;
#endif
}

float ScreenAddition::GetScreenMult(bool h) {
	return ms_fScreenMultiplier[h];
};

void ScreenAddition::SetScreenMult(float w, float h) {
	ms_fScreenMultiplier[0] = w;
	ms_fScreenMultiplier[1] = h;
};
