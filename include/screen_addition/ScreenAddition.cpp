#include "ScreenAddition.h"

float GetAspectRatio() {
#if GTA3
	return *(float*)0x5F53C0;
#elif GTAVC
	return *(float*)0x94DD38;
#elif GTASA
	return *(float*)0xC3EFA4;
#endif
}