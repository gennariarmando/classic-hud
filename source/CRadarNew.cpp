#include "plugin.h"
#include "CHud.h"
#include "CRadar.h"
#include "CMenuManager.h"
#include "CTxdStore.h"
#include "CHudNew.h"
#include "CRadarNew.h"
#include "Settings.h"
#include "ScreenAddition.h"

using namespace plugin;

CSprite2d CRadarNew::RadarSprites[RADAR_TOTALSPRITES];
CSprite2d CRadarNew::BlipsSprites[64];
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

		for (int i = 0; i < s.m_nBlipsCounter; i++) {
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

		for (int i = 0; i < s.m_nBlipsCounter; i++)
			BlipsSprites[i].Delete();

		int BlipsSlot = CTxdStore::FindTxdSlot("blips_new");
		CTxdStore::RemoveTxdSlot(BlipsSlot);

		ms_bSpritesLoaded = false;
	}
}

void CRadarNew::DrawRadarCircle() {
	if (CHudNew::GetGameMode() == GAMEMODE_III) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_VC) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_SA) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_LCS) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CRadarNew::DrawRadarSprite(unsigned int iconID, float x, float y, unsigned int alpha) {
	if (FrontEndMenuManager.drawRadarOrMap) {
		x = SCREEN_WIDTH * (1.0f / 640.0f) * x;
		y = SCREEN_HEIGHT * (1.0f / 640.0f) * y;
		CRadar::LimitToMap(&x, &y);
	}

	if (CRadar::DisplayThisBlip(iconID, -99)) {
		float w = 8.0f;
		float h = 8.0f;
		CRGBA color = CRGBA(255, 255, 255, alpha);

		BlipsSprites[iconID].Draw(CRect(x - SCREEN_LEFT(w), y - SCREEN_TOP(h), x + SCREEN_LEFT(w), y + SCREEN_TOP(h)), color);
		CRadar::AddBlipToLegendList(0, iconID);
	}
}

void CRadarNew::InjectPatches() {
	//Events::initRwEvent += CRadarNew::Initialise;
	//Events::shutdownRwEvent += CRadarNew::Shutdown;

#if GTASA
	//patch::RedirectJump(0x585FF0, CRadarNew::DrawRadarSprite);
	//patch::Nop(0x58A818, 16);
	//patch::Nop(0x58A8C2, 16);
	//patch::Nop(0x58A96C, 16);
	//patch::Nop(0x58AA1A, 16); 
#endif
}
