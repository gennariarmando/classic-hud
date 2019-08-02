#include "plugin.h"
#include "CProgressBar.h"
#include "CSprite2d.h"
#include "CTxdStore.h"
#include "Settings.h"
#include "ScreenAddons.h"
#include "CFont.h"

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
		ClassicHud::GetGameMode() == GAMEMODE_VCS ||
		ClassicHud::GetGameMode() == GAMEMODE_IV;
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

void CProgressBar::DrawIVProgressBar(float x, float y, float w, float h, float progress, CRGBA color, bool armour) {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)(rwFILTERMIPLINEAR));
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)(rwBLENDSRCALPHA));
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)(rwBLENDINVSRCALPHA));

	int Slot = CTxdStore::FindTxdSlot("progressbars");
	CTxdStore::AddRef(Slot);
	CTxdStore::PopCurrentTxd();
	CTxdStore::SetCurrentTxd(Slot);
	char Text[16];
	CSprite2d sprite[20];

	if (progress <= 0.00f)
		color.a = 0;

	int nPercentage = 0;
	if (progress > 0.00f && progress <= 0.05f) 
		nPercentage = 10;	
	else if (progress > 0.05f && progress <= 0.10f)
		nPercentage = 20;
	else if (progress > 0.10f && progress <= 0.15f)
		nPercentage = 30;
	else if (progress > 0.15f && progress <= 0.20f)
		nPercentage = 40;
	else if (progress > 0.20f && progress <= 0.25f)
		nPercentage = 50;
	else if (progress > 0.25f && progress <= 0.30f)
		nPercentage = 60;
	else if (progress > 0.30f && progress <= 0.35f)
		nPercentage = 70;
	else if (progress > 0.35f && progress <= 0.40f)
		nPercentage = 80;
	else if (progress > 0.40f && progress <= 0.45f)
		nPercentage = 90;
	else if (progress > 0.45f && progress <= 0.50f)
		nPercentage = 100;
	else if (progress > 0.50f && progress <= 0.55f)
		nPercentage = 110;
	else if (progress > 0.55f && progress <= 0.60f)
		nPercentage = 120;
	else if (progress > 0.60f && progress <= 0.65f)
		nPercentage = 130;
	else if (progress > 0.65f && progress <= 0.70f)
		nPercentage = 140;
	else if (progress > 0.70f && progress <= 0.75f)
		nPercentage = 150;
	else if (progress > 0.75f && progress <= 0.80f)
		nPercentage = 160;
	else if (progress > 0.80f && progress <= 0.85f)
		nPercentage = 170;
	else if (progress > 0.85f && progress <= 0.90f)
		nPercentage = 180;
	else if (progress > 0.90f && progress <= 0.99f)
		nPercentage = 190;
	else if (progress > 0.99f)
		nPercentage = 200;

	sprintf(Text, "bars_%d", nPercentage);
	sprite->SetTexture(Text);

	if (sprite) {
		if (armour) {
			sprite->Draw(CRect((x + w / 2), y, (x + w / 2) + (w / 2), y + h), CRGBA(color), 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f);
		}
		else {
			sprite->Draw(CRect(x, y, x + (w / 2), y + h), CRGBA(color), 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f);
		}
	}
}