#include "plugin.h"
#include "ClassicHud.h"
#include "CHudNew.h"
#include "CFontNew.h"
#include "CRadarNew.h"
#include "ScreenAddons.h"

#if GTASA
#include "CCamera.h"
#include "CHud.h"
#include "CMenuManager.h"
#include "CEntryExitManager.h"
#include "CRadar.h"
#include "CReplay.h"
#include "CTimer.h"
#include "CTxdStore.h"
#endif

using namespace plugin;

CRadarNew cradarnew;
CSprite2d CRadarNew::RadarSprites[RADAR_TOTALSPRITES];
CSprite2d CRadarNew::BlipsSprites[MAX_BLIPS];
bool CRadarNew::ms_bSpritesLoaded;

char *radarSpriteList[] = {
	"radardisc",
	"radarrect",
	"radarringfront",
	"radarringback",
	"radarringmask",
};

CRadarNew::CRadarNew() {
	s.readIni();

	if (!s.m_bEnable)
		return;

#if GTASA
	patch::RedirectJump(0x585FF0, CRadarNew::DrawRadarBlip);
	patch::RedirectJump(0x583480, CRadarNew::TransformRadarPointToScreenSpace);
	patch::Set(0x588722, 5); // CRadar::DrawRotatingRadarSprite[RADAR_SPRITE_CENTRE];
	patch::RedirectJump(0x585700, CRadarNew::DrawRadarMask);
	patch::RedirectJump(0x5832F0, CRadarNew::LimitRadarPoint);
	patch::Set(0x588188, 5); // CRadar::DrawBlips - radar_north
#endif
}

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

void CRadarNew::DrawRadarBeforeHud() {
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

	DrawRadarSpritesBack(s.m_fRadarSprites.left, s.m_fRadarSprites.top, s.m_fRadarSprites.right, s.m_fRadarSprites.bottom);

	if ((CHud::m_ItemToFlash != 8 || CTimer::m_FrameCounter & 8) && FrontEndMenuManager.m_nRadarMode != 2) {
		if (!CHud::bScriptDontDisplayRadar)
			CRadar::DrawMap();
	}
}

void CRadarNew::DrawRadarAfterHud() {
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

	DrawRadarSprites(s.m_fRadarSprites.left, s.m_fRadarSprites.top, s.m_fRadarSprites.right, s.m_fRadarSprites.bottom);

	if ((CHud::m_ItemToFlash != 8 || CTimer::m_FrameCounter & 8) && FrontEndMenuManager.m_nRadarMode != 2) {
		DrawRadarNorth();
		CRadar::DrawBlips();
		DrawRadarCentre();
	}
}

void CRadarNew::DrawRadarSpritesBack(float x, float y, float w, float h) {
	ScreenAddons::SetScreenMult(RADAR_WIDTH_SCALE, RADAR_HEIGHT_SCALE);

	if (ClassicHud::GetGameMode() == GAMEMODE_IV) {
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDONE);
		RadarSprites[RADAR_RING_MASK].Draw(CRect(SCREEN_LEFT(x), SCREEN_BOTTOM(y), SCREEN_LEFT(w), SCREEN_BOTTOM(y + h)), CRGBA(255, 255, 255, 35));
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);

		RadarSprites[RADAR_RING_BACK].Draw(CRect(SCREEN_LEFT(x), SCREEN_BOTTOM(y), SCREEN_LEFT(w), SCREEN_BOTTOM(y + h)), CRGBA(115, 115, 115, 150));
	}
}

void CRadarNew::DrawRadarSprites(float x, float y, float w, float h) {
	ScreenAddons::SetScreenMult(RADAR_WIDTH_SCALE, RADAR_HEIGHT_SCALE);

	if (ClassicHud::GetGameMode() == GAMEMODE_IV) {
		RadarSprites[RADAR_RING_FRONT].Draw(CRect(SCREEN_LEFT(x), SCREEN_BOTTOM(y), SCREEN_LEFT(w), SCREEN_BOTTOM(y + h)), CRGBA(255, 255, 255, 255));
	}
	else {
		if (s.bRectangularRadar)
			RadarSprites[RADAR_RECT].Draw(CRect(SCREEN_LEFT(x), SCREEN_BOTTOM(y - 5.5f), SCREEN_LEFT(w), SCREEN_BOTTOM(y + h + 5.5f)), CRGBA(255, 255, 255, 150));
		else
			RadarSprites[RADAR_DISC].Draw(CRect(SCREEN_LEFT(x), SCREEN_BOTTOM(y), SCREEN_LEFT(w), SCREEN_BOTTOM(y + h)), CRGBA(255, 255, 255, 255));
	}
}

void CRadarNew::DrawRadarNorth() {
	CVector2D out;
	CVector2D in = CVector2D(0.0f, 0.0f);
	CVector2D pos;
	pos.y = CRadar::m_radarRange * 1.4142135 + CRadar::vec2DRadarOrigin.y;
	pos.x = CRadar::vec2DRadarOrigin.x;
	CRadar::TransformRealWorldPointToRadarSpace(in, pos);
	CRadar::LimitRadarPoint(in);
	CRadar::TransformRadarPointToScreenSpace(out, in);
	DrawRadarBlip(RADAR_SPRITE_NORTH, out.x, out.y, 255);
}

void CRadarNew::DrawRadarCentre() {
	ScreenAddons::SetScreenMult(1.0f, 1.0f);

	CVector2D out = CVector2D(0.0f, 0.0f);
	CVector2D in = CVector2D(0.0f, 0.0f);
	TransformRadarPointToScreenSpace(out, in);
	float angle = FindPlayerHeading(0) - (CRadar::m_fRadarOrientation + M_PI);
	ScreenAddons::SetScreenMult(1.0f, 1.0f);
	CRadar::DrawRotatingRadarSprite(&BlipsSprites[2], out.x, out.y, angle, SCREEN_LEFT(s.m_fRadarBlips.right * 0.95f), SCREEN_TOP(s.m_fRadarBlips.bottom * 0.95f), CRGBA(255, 255, 255, 245));
}

void CRadarNew::DrawRadarBlip(unsigned char iconID, float x, float y, unsigned int alpha) {
	ScreenAddons::SetScreenMult(HUD_WIDTH_SCALE, HUD_HEIGHT_SCALE);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);

#if GTASA
	if (FrontEndMenuManager.drawRadarOrMap) {
		x = SCREEN_WIDTH * (1.0f / 640.0f) * x;
		y = SCREEN_HEIGHT * (1.0f / 448.0f) * y;
		CRadar::LimitToMap(&x, &y);
	}

	if (CRadar::DisplayThisBlip(iconID, -99)) {
		float w = s.m_fRadarBlips.right;
		float h = s.m_fRadarBlips.bottom;
		if (ms_bSpritesLoaded)
			BlipsSprites[iconID].Draw(CRect(x - SCREEN_LEFT(w), y - SCREEN_TOP(h), x + SCREEN_LEFT(w), y + SCREEN_TOP(h)), CRGBA(255, 255, 255, alpha));	

		CRadar::AddBlipToLegendList(0, iconID);
	}
#endif

	ScreenAddons::SetScreenMult(RADAR_WIDTH_SCALE, RADAR_HEIGHT_SCALE);
}

void CRadarNew::TransformRadarPointToScreenSpace(CVector2D &out, CVector2D &in) {
#if GTASA
	__asm push edx

	ScreenAddons::SetScreenMult(RADAR_WIDTH_SCALE, RADAR_HEIGHT_SCALE);
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

void CRadarNew::DrawRadarMask() {
	CVector2D circle[4] = {
		CVector2D(1.0f, -1.0f),
		CVector2D(1.0f, 1.0f),
		CVector2D(-1.0f, 1.0f),
		CVector2D(-1.0, -1.0f)
	};

	CVector2D rect[4] = {
		CVector2D(0.0f, 0.0f),
		CVector2D(0.0f, 0.0f),
		CVector2D(0.0f, 0.0f),
		CVector2D(0.0f, 0.0f)
	};

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void*)FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	RwD3D9SetRenderState(rwRENDERSTATESTENCILFUNCTION, rwSTENCILFUNCTIONALWAYS);

	CVector2D out[8];
	CVector2D in;

	for (int i = 0; i < 4; i++) {
		if (s.bRectangularRadar && ClassicHud::GetGameMode() != GAMEMODE_IV) {
			in.x = rect[i].x;
			in.y = rect[i].y;
		}
		else {
			in.x = circle[i].x;
			in.y = circle[i].y;
		}
		TransformRadarPointToScreenSpace(out[0], in);

		for (int j = 0; j < 7; j++) {
			if (s.bRectangularRadar && ClassicHud::GetGameMode() != GAMEMODE_IV) {
				in.x = rect[i].x * cosf(j * (3.14159265358979323846 / 2.0f / 6.0f));
				in.y = rect[i].y * sinf(j * (3.14159265358979323846 / 2.0f / 6.0f));
			}
			else {
				in.x = circle[i].x * cosf(j * (3.14159265358979323846 / 2.0f / 6.0f));
				in.y = circle[i].y * sinf(j * (3.14159265358979323846 / 2.0f / 6.0f));
			}
			TransformRadarPointToScreenSpace(out[j + 1], in);
		};

		CSprite2d::SetMaskVertices(8, (float *)out, CSprite2d::NearScreenZ);
		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 8);
	};

	RwD3D9SetRenderState(rwRENDERSTATESTENCILFUNCTION, rwSTENCILFUNCTIONGREATER);
}

float CRadarNew::LimitRadarPoint(CVector2D *pos) {
	float m = pos->Magnitude();

	if (s.bRectangularRadar && ClassicHud::GetGameMode() != GAMEMODE_IV) {
		if (m > 1.41421356237309504880) {
			pos->x /= m * 0.707106781186547524401;
			pos->y /= m * 0.707106781186547524401;
		}
		if (pos->x > 1.0f)
			pos->x = 1.0f;
		else if (pos->x < -1.0f)
			pos->x = -1.0;
		if (pos->y > 1.0)
			pos->y = 1.0;
		else if (pos->y < -1.0f)
			pos->y = -1.0;
	}
	else {
		if (m > 1.0f) {
			pos->x /= m * 1.0f;
			pos->y /= m * 1.0f;
		}
	}
	return m;
}