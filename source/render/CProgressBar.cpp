#include "plugin.h"
#include "CProgressBar.h"
#include "CSprite2d.h"
#include "CTxdStore.h"
#include "Settings.h"
#include "ScreenAddons.h"

bool CProgressBar::ms_bSpritesLoaded;
CSprite2d CProgressBar::Sprites[PROGRESS_TOTALSPRITES];

char *progressBarsNames[] = {
	"bar_outline",
	"bar_inside"
};

void CProgressBar::Initialise() {
	if (IsProgressBarRequired()) {
		if (!ms_bSpritesLoaded) {
			int Slot = CTxdStore::AddTxdSlot("progressbars");
			CTxdStore::LoadTxd(Slot, PLUGIN_PATH(s.SetFileWithPrefix("classichud\\progressbars\\", "bars.txd")));
			CTxdStore::AddRef(Slot);
			CTxdStore::PopCurrentTxd();
			CTxdStore::SetCurrentTxd(Slot);

			for (int i = 0; i < PROGRESS_TOTALSPRITES; i++)
				Sprites[i].SetTexture(progressBarsNames[i]);

			CTxdStore::PopCurrentTxd();
		}
	}
}

void CProgressBar::Shutdown() {
	if (ms_bSpritesLoaded) {
		for (int i = 0; i < PROGRESS_TOTALSPRITES; ++i)
			Sprites[i].Delete();

		int Slot = CTxdStore::FindTxdSlot("progressbars");
		CTxdStore::RemoveTxdSlot(Slot);

		ms_bSpritesLoaded = false;
	}
}

bool CProgressBar::IsProgressBarRequired() {
	return
		ClassicHud::GetGameMode() == GAMEMODE_LCS ||
		ClassicHud::GetGameMode() == GAMEMODE_VCS;
}

void CProgressBar::DrawProgressBar(float x, float y, float width, float height, float progress, char border, char shadow, CRGBA colorFront, CRGBA colorBack) {
	// Shadow
	if (shadow)
		CSprite2d::DrawRect(CRect((x + shadow), (y + shadow), (x + shadow + width), (y + shadow + height)), CRGBA(0, 0, 0, 205));
	// Border
	if (border)
		CSprite2d::DrawRect(CRect((x - border), (y - border), (x + border + width), (y + border + height)), CRGBA(0, 0, 0, 255));

	if (progress >= 1.0f)
		progress = 1.0f;
	else {
		// Back
		CSprite2d::DrawRect(CRect((x), (y), (x + width), (y + height)), CRGBA(colorBack));
	}
	if (progress > 0.0f) {
		CSprite2d::DrawRect(CRect((x), (y), (x + width * (progress)), (y + height)), CRGBA(colorFront));
	}
}

void CProgressBar::DrawProgressBarWithProgressDelta(float x, float y, float width, float height, float progress, CRGBA const &color, float deltaProgress, CRGBA const &deltaColor) {
	// Progress value is 0.0 - 100.0
	if (progress >= 100.0f)
		progress = 100.0f;
	else {
		CSprite2d::DrawRect(CRect((int)(x)-0.5f, (int)(y)-0.5f, (int)(x + width) - 0.5f, (int)(y + height) - 0.5f),
			CRGBA(color.r, color.g, color.b, color.a / 2));
	}
	if (progress > 0.0f) {
		CSprite2d::DrawRect(CRect((int)(x)-0.5f, (int)(y)-0.5f, (int)(x + width * (progress / 100.f)) - 0.5f,
			(int)(y + height) - 0.5f), color);
	}
	if (deltaProgress > 0.0f) {
		if (deltaProgress > 100.0f)
			deltaProgress = 100.0f;
		CSprite2d::DrawRect(CRect((int)(x + width * (progress / 100.f) - deltaProgress / 100.0f * width) - 0.5f, (int)(y)-0.5f,
			(int)(x + width * (progress / 100.f)) - 0.5f, (int)(y + height) - 0.5f), deltaColor);
	}
}

void CProgressBar::DrawProgressBarWithSprites(float x, float y, float width, float height, float progress, CRGBA color) {
	float _x(x), _w(width);

	_x += SCREEN_LEFT(1.0f);
	_w += SCREEN_LEFT(-2.0f);
	if (progress >= 1.0f)
		progress = 1.0f;
	else
		Sprites[BAR_INSIDE].Draw(CRect((_x + _w * (progress)), (y), (_x + _w), (y + height)), CRGBA(color.r * 0.50, color.g * 0.50, color.b * 0.50, color.a), progress, 1.0f, 1.0f, 1.0f, progress, 0.0f, 1.0f, 0.0f);

	if (progress > 0.0f)
		Sprites[BAR_INSIDE].Draw(CRect((_x), (y), (_x + _w * (progress)), (y + height)), CRGBA(color.r, color.g, color.b, color.a), 0.0f, 1.0f, progress, 1.0f, 0.0f, 0.0f, progress, 0.0f);

	Sprites[BAR_OUTLINE].Draw(CRect((x), (y), (x + width), (y + height)), CRGBA(0, 0, 0, color.a));
}