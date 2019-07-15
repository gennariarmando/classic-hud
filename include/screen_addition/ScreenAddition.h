#pragma once
#include "plugin.h"

#define HORPLUS(a) ((a) * (4.0f / 3.0f) / GetAspectRatio())
#define SCREEN_LEFT(a) (HORPLUS((a) * SCREEN_WIDTH / 640.0f))
#define SCREEN_TOP(a) ((a) * SCREEN_HEIGHT / 448.0f)
#define SCREEN_RIGHT(a) (SCREEN_WIDTH - SCREEN_LEFT(a))
#define SCREEN_BOTTOM(a) (SCREEN_HEIGHT - SCREEN_TOP(a))

extern float GetAspectRatio();
