#include "plugin.h"
#include "CHud.h"
#include "CRadar.h"
#include "CMenuManager.h"
#include "CTxdStore.h"
#include "CHudNew.h"
#include "CRadarNew.h"
#include "Settings.h"
#include "ScreenAddition.h"
#include "CEntryExitManager.h"
#include "CTimer.h"
#include "CCamera.h"
#include "CReplay.h"

using namespace plugin;

CSprite2d CRadarNew::RadarSprites[RADAR_TOTALSPRITES];
CSprite2d CRadarNew::BlipsSprites[300];
bool CRadarNew::ms_bSpritesLoaded;

char *radarSpriteList[] = {
	"radardisc",
	"radarringfront",
	"radarringback",
	"radarringmask"
};

void CRadarNew::Initialise() {
	s.readBlipsDat();

	if (!ms_bSpritesLoaded) {
		int RadarSlot = CTxdStore::AddTxdSlot("radar_new");
		CTxdStore::LoadTxd(RadarSlot, PLUGIN_PATH(s.SetFileWithPrefix("classichud\\radar\\", "radar.txd")));
		CTxdStore::AddRef(RadarSlot);
		CTxdStore::PopCurrentTxd();
		CTxdStore::SetCurrentTxd(RadarSlot);

		for (int i = 0; i < RADAR_TOTALSPRITES; i++)
			RadarSprites[i].SetTexture(radarSpriteList[i]);

		CTxdStore::PopCurrentTxd();

		int BlipsSlot = CTxdStore::AddTxdSlot("blips_new");
		CTxdStore::LoadTxd(BlipsSlot, PLUGIN_PATH(s.SetFileWithPrefix("classichud\\radar\\", "blips.txd")));
		CTxdStore::AddRef(BlipsSlot);
		CTxdStore::PopCurrentTxd();
		CTxdStore::SetCurrentTxd(BlipsSlot);

		for (int i = 0; i < MAX_BLIPS; i++) {
			BlipsSprites[i].SetTexture(s.m_pBlipNames[i]);
		}

		CTxdStore::PopCurrentTxd();

		ms_bSpritesLoaded = true;
	}
}

void CRadarNew::Shutdown() {
	if (ms_bSpritesLoaded) {
		for (int i = 0; i < RADAR_TOTALSPRITES; i++)
			RadarSprites[i].Delete();

		int RadarSlot = CTxdStore::FindTxdSlot("radar_new");
		CTxdStore::RemoveTxdSlot(RadarSlot);

		for (int i = 0; i < MAX_BLIPS; i++)
			BlipsSprites[i].Delete();

		int BlipsSlot = CTxdStore::FindTxdSlot("blips_new");
		CTxdStore::RemoveTxdSlot(BlipsSlot);

		ms_bSpritesLoaded = false;
	}
}

void CRadarNew::DrawRadar() {
	if (CReplay::Mode != 1
		&& !CWeapon::ms_bTakePhoto
		&& !FrontEndMenuManager.m_bActivateMenuNextFrame) {
		if (!CHud::bScriptDontDisplayRadar && !TheCamera.m_bWideScreenOn) {
			if (!CPad::GetPad(0)->GetDisplayVitalStats(FindPlayerPed()) || FindPlayerVehicle(-1, 0)) {
				if (CEntryExitManager::ms_exitEnterState != 1 && CEntryExitManager::ms_exitEnterState != 2 &&
					FrontEndMenuManager.m_nRadarMode != 2 && (CHud::m_ItemToFlash != 8 || CTimer::m_FrameCounter & 8)) {

					RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
					RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);

					ScreenAddition::SetScreenMult(s.m_fRadarW, s.m_fRadarH);
					CRadar::DrawMap();

					// Radar disc
					RadarSprites[RADAR_DISC].Draw(CRect(SCREEN_LEFT(s.m_fRadarSprites.left), SCREEN_BOTTOM(s.m_fRadarSprites.top), SCREEN_LEFT(s.m_fRadarSprites.right), SCREEN_BOTTOM(s.m_fRadarSprites.bottom)), CRGBA(255, 255, 255, 255));

					// Draw blips
					CRadar::DrawBlips();
					ScreenAddition::SetScreenMult(s.m_fRadarW, s.m_fRadarH);

					// Radar centre
					CVector2D out;
					CVector2D in = CVector2D(0.0f, 0.0f);
					TransformRadarPointToScreenSpace(out, in);
					float angle = FindPlayerHeading(0) - (CRadar::m_fRadarOrientation + M_PI);
					CRadar::DrawRotatingRadarSprite(&BlipsSprites[2], out.x, out.y, angle, SCREEN_LEFT(8.0f), SCREEN_TOP(7.6f), CRGBA(255, 255, 255, 255));
				
					ScreenAddition::SetScreenMult(DEFAULT_HUD_SCALE);
				}
			}
		}
	}
}

void CRadarNew::DrawRadarSprite(unsigned char iconID, float x, float y, unsigned int alpha) {
#if GTASA
	if (FrontEndMenuManager.drawRadarOrMap) {
		x = SCREEN_WIDTH * (1.0f / 640.0f) * x;
		y = SCREEN_HEIGHT * (1.0f / 448.0f) * y;
		CRadar::LimitToMap(&x, &y);
	}

	if (CRadar::DisplayThisBlip(iconID, -99)) {
		float w = 7.6f;
		float h = 7.6f;
		if (ms_bSpritesLoaded)
			BlipsSprites[iconID].Draw(CRect(x - SCREEN_LEFT(w), y - SCREEN_TOP(h), x + SCREEN_LEFT(w), y + SCREEN_TOP(h)), CRGBA(255, 255, 255, alpha));	
		CRadar::AddBlipToLegendList(0, iconID);
	}
#endif
}

void CRadarNew::TransformRadarPointToScreenSpace(CVector2D &out, CVector2D &in) {
#if GTASA
	__asm push edx

	if (FrontEndMenuManager.drawRadarOrMap) {
		out.x = FrontEndMenuManager.m_fMapZoom * in.x + FrontEndMenuManager.m_fMapBaseX;
		out.y = FrontEndMenuManager.m_fMapBaseY - FrontEndMenuManager.m_fMapZoom * in.y;
	}
	else {
		out.x = (in.x + 1.0f) * 0.5f * SCREEN_LEFT(s.m_fRadarMap.right) + SCREEN_LEFT(s.m_fRadarMap.left);
		out.y = (1.0f - in.y) * 0.5f * SCREEN_TOP(s.m_fRadarMap.bottom) + SCREEN_BOTTOM(s.m_fRadarMap.top + s.m_fRadarMap.bottom);
	}
	__asm pop edx
#endif
}

void CRadarNew::InjectPatches() {
	Events::initRwEvent += CRadarNew::Initialise;
	Events::shutdownRwEvent += CRadarNew::Shutdown;
	Events::drawHudEvent += CRadarNew::DrawRadar;

#if GTASA
	patch::RedirectJump(0x585FF0, CRadarNew::DrawRadarSprite);
	patch::RedirectJump(0x583480, CRadarNew::TransformRadarPointToScreenSpace);
	patch::Set(0x58FC53, 5); // CRadar::DrawRadar
	patch::Set(0x588722, 5); // CRadar::DrawRotatingRadarSprite[RADAR_SPRITE_CENTRE];
#endif
}
