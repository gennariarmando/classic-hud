#include "plugin.h"
#include "CClock.h"
#include "CHud.h"
#include "CFont.h"
#include "CTxdStore.h"
#include "CWorld.h"
#include "ScreenAddition.h"
#include "eWeaponType.h"
#include "CMenuManager.h"
#include "CTheScripts.h"
#include "CCamera.h"
#include "CTimer.h"
#include "CCutsceneMgr.h"
#include "CHudNew.h"
#include "CFontNew.h"
#include "CRadarNew.h"
#include "Settings.h"
#include "CModelInfo.h"
#include "CStats.h"
#include "CMessages.h"
#include "CGarages.h"
#include "CReplay.h"
#if GTASA
#include "eModelID.h"
#include "CDarkel.h"
#include "CEntryExitManager.h"
#include "CAERadioTrackManager.h"
#include "CText.h"
#include "CRadar.h"
#endif

using namespace plugin;

eGameMode CHudNew::ms_nGameMode;
CSprite2d CHudNew::SpritesNew[HUD_TOTALSPRITES];
CSprite2d CHudNew::SharedSprites[SHARED_TOTALSPRITES];

bool CHudNew::ms_bSpritesLoaded;
char *CHudNew::ms_nGamePrefix;
bool CHudNew::ms_bReload;

char *SpritesModNames[HUD_TOTALSPRITES] = {
	"unarmed"
};

char *SharedNames[SHARED_TOTALSPRITES] = {
	"breathicon",
	"skipicon"
};

void CHudNew::Initialise() {
	char *ModeNames[GAMEMODE_TOTALMODES] = { "III", "VC", "SA", "LCS", "VCS", "ADVANCE" };

	SetGameMode(GAMEMODE_NULL);
	if (GetGameMode() == GAMEMODE_NULL) {
		for (int i = 0; i < GAMEMODE_TOTALMODES; i++) {
			if (s.m_nGameMode == ModeNames[i]) {
				SetGameMode((eGameMode)i);
				ms_nGamePrefix = ModeNames[i];
			}
		}
	}

	if (!ms_bSpritesLoaded) {
		int WeaponSlot = CTxdStore::AddTxdSlot("weapons");
		CTxdStore::LoadTxd(WeaponSlot, PLUGIN_PATH(s.SetFileWithPrefix("classichud\\weapons\\", "weapons.txd")));
		CTxdStore::AddRef(WeaponSlot);
		CTxdStore::PopCurrentTxd();
		CTxdStore::SetCurrentTxd(WeaponSlot);

		for (int i = 0; i < HUD_TOTALSPRITES; i++)
			SpritesNew[i].SetTexture(SpritesModNames[i]);

		CTxdStore::PopCurrentTxd();

		int SharedSlot  = CTxdStore::AddTxdSlot("shared");
		CTxdStore::LoadTxd(SharedSlot, PLUGIN_PATH(s.SetSharedFile("classichud\\shared\\", "shared.txd")));
		CTxdStore::AddRef(SharedSlot);
		CTxdStore::PopCurrentTxd();
		CTxdStore::SetCurrentTxd(SharedSlot);

		for (int i = 0; i < SHARED_TOTALSPRITES; i++)
			SharedSprites[i].SetTexture(SharedNames[i]);

		CTxdStore::PopCurrentTxd();

		ms_bSpritesLoaded = true;

		s.readDat();
	}
}

void CHudNew::Shutdown() {
	if (ms_bSpritesLoaded) {
		for (int i = 0; i < HUD_TOTALSPRITES; ++i)
			SpritesNew[i].Delete();

		int WeaponSlot = CTxdStore::FindTxdSlot("weapons");
		CTxdStore::RemoveTxdSlot(WeaponSlot);

		for (int i = 0; i < SHARED_TOTALSPRITES; ++i)
			SharedSprites[i].Delete();

		int SharedSlot = CTxdStore::FindTxdSlot("shared");
		CTxdStore::RemoveTxdSlot(SharedSlot);

		ms_bSpritesLoaded = false;
	}
}

void CHudNew::ReInitialise() {
	if (!ms_bReload) { // TODO: find an automatized check.
		s.readIni();
		CHudNew::Shutdown();
		CHudNew::Initialise();
		CFontNew::Shutdown();
		CFontNew::Initialise();
		CRadarNew::Shutdown();
		CRadarNew::Initialise();
		ms_bReload = true;
	}
}

void CHudNew::Draw() {
	ScreenAddition::SetScreenMult(s.m_fHudW, s.m_fHudH);

	if (!TheCamera.m_bWideScreenOn) {
		if (FrontEndMenuManager.m_bHudOn && CTheScripts::bDisplayHud) {
			RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
			RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
			RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSMIRROR);
			RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)FALSE);
			RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
			RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);

			if (CWorld::Players[0].m_pPed) {
				if (CHud::bDrawClock)
					DrawClock(s.m_fClock.left, s.m_fClock.top, s.m_fClock.right, s.m_fClock.bottom);

				DrawMoneyCounter(s.m_fMoney.left, s.m_fMoney.top, s.m_fMoney.right, s.m_fMoney.bottom);
				DrawHealth(0, s.m_fHealth.left, s.m_fHealth.top, s.m_fHealth.right, s.m_fHealth.bottom);
				DrawArmour(0, s.m_fArmour.left, s.m_fArmour.top, s.m_fArmour.right, s.m_fArmour.bottom);
				DrawBreath(0, s.m_fBreath.left, s.m_fBreath.top, s.m_fBreath.right, s.m_fBreath.bottom);
				DrawWeaponIcon(0, s.m_fWeapon.left, s.m_fWeapon.top, s.m_fWeapon.right, s.m_fWeapon.bottom);
				DrawAmmo(0, s.m_fAmmo.left, s.m_fAmmo.top, s.m_fAmmo.right, s.m_fAmmo.bottom);
				DrawWanted(s.m_fWanted.left, s.m_fWanted.top, s.m_fWanted.right, s.m_fWanted.bottom);

				if (!CHud::bScriptDontDisplayAreaName)
					DrawAreaName(s.m_fZoneName.left, s.m_fZoneName.top, s.m_fZoneName.right, s.m_fZoneName.bottom);

				if (!CHud::bScriptDontDisplayVehicleName)
					DrawVehicleName(s.m_fVehicleName.left, s.m_fVehicleName.top, s.m_fVehicleName.right, s.m_fVehicleName.bottom);

				DrawRadioStation(s.m_fRadioName.left, s.m_fRadioName.top, s.m_fRadioName.right, s.m_fRadioName.bottom);
			}

			ms_bReload = false;
		}
	}

	ScreenAddition::SetScreenMult(DEFAULT_HUD_SCALE);
}

void CHudNew::DrawClock(float x, float y, float w, float h) {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "%02d:%02d", CClock::ms_nGameClockHours, CClock::ms_nGameClockMinutes);

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_CLOCK));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (GetGameMode() == GAMEMODE_VC) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(VC_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_CLOCK));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (GetGameMode() == GAMEMODE_SA) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(SA_FONT_PRICEDOWN);
		CFont::SetDropShadowPosition(0);
		CFont::SetEdge(SA_EDGE_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_CLOCK));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (GetGameMode() == GAMEMODE_LCS) {

	}
	else if (GetGameMode() == GAMEMODE_VCS) {

	}
	else if (GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawMoneyCounter(float x, float y, float w, float h) {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "$%08d", CWorld::Players[CWorld::PlayerInFocus].m_nDisplayMoney);

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_CASH));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (GetGameMode() == GAMEMODE_VC) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(VC_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_CASH));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (GetGameMode() == GAMEMODE_SA) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(SA_FONT_PRICEDOWN);
		CFont::SetDropShadowPosition(0);
		CFont::SetEdge(SA_EDGE_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_CASH));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		float fOffset = 0.0f;
		if (static_cast<float>(CWorld::Players[CWorld::PlayerInFocus].m_nMaxHealth) > 101.0f)
			fOffset = 12.0f;

		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y + fOffset), TextToPrint);
	}
	else if (GetGameMode() == GAMEMODE_LCS) {

	}
	else if (GetGameMode() == GAMEMODE_VCS) {

	}
	else if (GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawHealth(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];
	float fProgress = CWorld::Players[PlayerID].m_pPed->m_fHealth / static_cast<float>(CWorld::Players[PlayerID].m_nMaxHealth);
	int nProgress = static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_fHealth);

	if (CHud::m_ItemToFlash != 4 || CTimer::m_FrameCounter & 8) {
		if (GetGameMode() == GAMEMODE_III) {
			strcpy_s(IconToPrint, "{");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(III_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_HEALTH));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == CWorld::Players[PlayerID].m_nLastTimeEnergyLost || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeEnergyLost + 1500 || CTimer::m_FrameCounter & 8)
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
		else if (GetGameMode() == GAMEMODE_VC) {
			strcpy_s(IconToPrint, "{");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(VC_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_HEALTH));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
			
			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == CWorld::Players[PlayerID].m_nLastTimeEnergyLost || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeEnergyLost + 1500 || CTimer::m_FrameCounter & 8)
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
		else if (GetGameMode() == GAMEMODE_SA) {
			float fOffset = 0.0f;

			if (static_cast<float>(CWorld::Players[PlayerID].m_nMaxHealth) > 101.0f)
				fOffset = 12.0f;

			float fBarSize = static_cast<float>(CWorld::Players[PlayerID].m_nMaxHealth) / CStats::GetFatAndMuscleModifier(STAT_MOD_10);

			if (fBarSize > 1.0f)
				fBarSize = 1.0f;

			if (CWorld::Players[PlayerID].m_pPed->m_fHealth >= 10 || CTimer::m_FrameCounter & 8)
				DrawProgressBar(SCREEN_RIGHT(x + (w * fBarSize)), SCREEN_TOP(y + fOffset), SCREEN_LEFT(w * fBarSize), SCREEN_TOP(h), fProgress, SCREEN_TOP(2.5f), 0, CRGBA(s.HUD_COLOR_HEALTH), CRGBA(s.HUD_COLOR_HEALTH.r, s.HUD_COLOR_HEALTH.g, s.HUD_COLOR_HEALTH.b, 150));
		}
		else if (GetGameMode() == GAMEMODE_LCS) {

		}
		else if (GetGameMode() == GAMEMODE_VCS) {

		}
		else if (GetGameMode() == GAMEMODE_ADVANCE) {

		}
	}
}

void CHudNew::DrawArmour(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];
	float fProgress = CWorld::Players[PlayerID].m_pPed->m_fArmour / static_cast<float>(CWorld::Players[PlayerID].m_nMaxArmour);
	int nProgress = static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_fArmour);

	if ((CHud::m_ItemToFlash != 3 || CTimer::m_FrameCounter & 8) && CWorld::Players[PlayerID].m_pPed->m_fArmour > 1.0) {
		if (GetGameMode() == GAMEMODE_III) {
			strcpy_s(IconToPrint, "[");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(III_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_ARMOUR));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == 0 || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeArmourLost + 1500 || CTimer::m_FrameCounter & 8)
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
		else if (GetGameMode() == GAMEMODE_VC) {
			strcpy_s(IconToPrint, "<");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(VC_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_ARMOUR));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == 0 || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeArmourLost + 1500 || CTimer::m_FrameCounter & 8)
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
		else if (GetGameMode() == GAMEMODE_SA) {
			DrawProgressBar(SCREEN_RIGHT(x + w), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, SCREEN_TOP(2.5f), 0, CRGBA(s.HUD_COLOR_ARMOUR), CRGBA(s.HUD_COLOR_ARMOUR.r, s.HUD_COLOR_ARMOUR.g, s.HUD_COLOR_ARMOUR.b, 150));
		}
		else if (GetGameMode() == GAMEMODE_LCS) {

		}
		else if (GetGameMode() == GAMEMODE_VCS) {

		}
		else if (GetGameMode() == GAMEMODE_ADVANCE) {

		}
	}
}

void CHudNew::DrawBreath(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];
	float fProgress = CWorld::Players[PlayerID].m_pPed->m_pPlayerData->m_fBreath / CStats::GetFatAndMuscleModifier(STAT_MOD_AIR_IN_LUNG);
	int nProgress = static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_pPlayerData->m_fBreath * 100.0f / CStats::GetFatAndMuscleModifier(STAT_MOD_AIR_IN_LUNG));

	if (CWorld::Players[PlayerID].m_pPed->m_nPhysicalFlags.bSubmergedInWater) {
		if (CHud::m_ItemToFlash != 10 || CTimer::m_FrameCounter & 8) {
			if (GetGameMode() == GAMEMODE_III) {
				sprintf_s(TextToPrint, "%03d", nProgress);

				CFont::SetProportional(false);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(III_FONT_PRICEDOWN);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOR_BREATH.a));
				CFont::SetColor(CRGBA(s.HUD_COLOR_BREATH));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 69.5f), SCREEN_TOP(y + 7.5f), SCREEN_RIGHT(x + 69.5f) + SCREEN_LEFT(w * 25.0f), SCREEN_TOP(y + 7.5f) + SCREEN_TOP(h * 13.0f)), CRGBA(0, 0, 0, s.HUD_COLOR_BREATH.a));
				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 71.0f), SCREEN_TOP(y + 6.0f), SCREEN_RIGHT(x + 71.0f) + SCREEN_LEFT(w * 25.0f), SCREEN_TOP(y + 6.0f) + SCREEN_TOP(h * 13.0f)), CRGBA(s.HUD_COLOR_BREATH));
			}
			else if (GetGameMode() == GAMEMODE_VC) {
				sprintf_s(TextToPrint, "%03d", nProgress);

				CFont::SetProportional(false);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(III_FONT_PRICEDOWN);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, 255));
				CFont::SetColor(CRGBA(s.HUD_COLOR_BREATH));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 69.5f), SCREEN_TOP(y + 7.5f), SCREEN_RIGHT(x + 69.5f) + SCREEN_LEFT(w * 25.0f), SCREEN_TOP(y + 7.5f) + SCREEN_TOP(h * 13.0f)), CRGBA(0, 0, 0, s.HUD_COLOR_BREATH.a));
				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 71.5f), SCREEN_TOP(y + 6.0f), SCREEN_RIGHT(x + 71.5f) + SCREEN_LEFT(w * 25.0f), SCREEN_TOP(y + 6.0f) + SCREEN_TOP(h * 13.0f)), CRGBA(s.HUD_COLOR_BREATH));
			}
			else if (GetGameMode() == GAMEMODE_SA) {
				DrawProgressBar(SCREEN_RIGHT(x + w), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, SCREEN_TOP(2.5f), 0, CRGBA(s.HUD_COLOR_BREATH), CRGBA(s.HUD_COLOR_BREATH.r, s.HUD_COLOR_BREATH.g, s.HUD_COLOR_BREATH.b, 150));
			}
			else if (GetGameMode() == GAMEMODE_LCS) {

			}
			else if (GetGameMode() == GAMEMODE_VCS) {

			}
			else if (GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}
	}
}

void CHudNew::DrawWeaponIcon(int PlayerID, float x, float y, float w, float h) {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	int ModelId = CWeaponInfo::GetWeaponInfo(CWorld::Players[PlayerID].m_pPed->m_aWeapons[CWorld::Players[PlayerID].m_pPed->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1;
	int Slot = CTxdStore::FindTxdSlot("weapons");
	CBaseModelInfo *index = CModelInfo::GetModelInfo(ModelId);
	RwTexDictionary *txd = CTxdStore::ms_pTxdPool->GetAt(Slot)->m_pRwDictionary;
	CRGBA color = CRGBA(s.HUD_COLOR_WEAPON_ICON);

	if (ModelId <= MODEL_NULL) 
		SpritesNew[HUD_UNARMED].Draw(CRect(SCREEN_RIGHT(x), SCREEN_TOP(y), SCREEN_RIGHT(w), SCREEN_TOP(h)), CRGBA(color));
	else {
		if (txd) {
			RwTexture *pTexture = RwTexDictionaryFindHashNamedTexture(txd, index->m_nKey);
			if (pTexture) {
				CSprite2d::SetVertices(CRect(SCREEN_RIGHT(x), SCREEN_TOP(y), SCREEN_RIGHT(w), SCREEN_TOP(h)), color, color, color, color);
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, pTexture->raster);
				RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, CSprite2d::maVertices, 4);
				RwRenderStateSet(rwRENDERSTATETEXTURERASTER, nullptr);
			}
		}
	}
}

void CHudNew::DrawAmmo(int PlayerID, float x, float y, float w, float h) {
	CPlayerPed *Player = CWorld::Players[PlayerID].m_pPed;
	char TextToPrint[16];

	eWeaponType WeaponType = Player->m_aWeapons[Player->m_nActiveWeaponSlot].m_nType;
	int AmmoAmount = CWeaponInfo::GetWeaponInfo(Player->m_aWeapons[Player->m_nActiveWeaponSlot].m_nType, Player->GetWeaponSkill())->m_nAmmoClip;
	int AmmoInClip = Player->m_aWeapons[Player->m_nActiveWeaponSlot].m_nAmmoInClip;
	int TotalAmmo = Player->m_aWeapons[Player->m_nActiveWeaponSlot].m_nTotalAmmo;
	int Ammo, Clip;

	if (AmmoAmount <= 1 || AmmoAmount >= 1000)
		sprintf(TextToPrint, "%d", TotalAmmo);
	else {
		if (WeaponType == WEAPON_FTHROWER) {
			Clip = AmmoInClip / 10;

			if ((TotalAmmo - AmmoInClip) / 10 <= 9999)
				Ammo = (TotalAmmo - AmmoInClip) / 10;
			else
				Ammo = 9999;
		}
		else {
			Clip = AmmoInClip;

			if ((TotalAmmo - AmmoInClip) > 9999)
				Ammo = 9999;
			else
				Ammo = TotalAmmo - AmmoInClip;
		}

		sprintf(TextToPrint, "%d-%d", Ammo, Clip);
	}

	if ((TotalAmmo - AmmoInClip) != 9999 && (TotalAmmo - AmmoInClip) < 9999)
	if (!CDarkel::FrenzyOnGoing() && WeaponType &&
		WeaponType != WEAPON_DETONATOR &&
		(WeaponType <= WEAPON_DILDO1 || WeaponType > WEAPON_GRENADE) &&
		WeaponType != WEAPON_PARACHUTE &&
		CWeaponInfo::GetWeaponInfo(WeaponType, 1)->m_nWeaponFire != WEAPON_FIRE_USE &&
		CWeaponInfo::GetWeaponInfo(WeaponType, 1)->m_nSlot > 1) {
		if (GetGameMode() == GAMEMODE_III) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(III_FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(0);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (GetGameMode() == GAMEMODE_VC) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(VC_FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (GetGameMode() == GAMEMODE_SA) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(SA_FONT_SUBTITLES);
			CFont::SetDropShadowPosition(0);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (GetGameMode() == GAMEMODE_LCS) {

		}
		else if (GetGameMode() == GAMEMODE_VCS) {

		}
		else if (GetGameMode() == GAMEMODE_ADVANCE) {

		}
	}
}

void CHudNew::DrawWanted(float x, float y, float w, float h) {
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		strcpy_s(IconToPrint, "]");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_N));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8))
				CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_A));
			else {
				CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_N));
				CFont::SetDropShadowPosition(0);
			}

			CFont::PrintString(SCREEN_RIGHT(x + 23.0f * i), SCREEN_TOP(y), IconToPrint);
		}
	}
	else if (GetGameMode() == GAMEMODE_VC) {
		strcpy_s(IconToPrint, ">");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_N));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8)) 
				CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_A));
			else
				CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_N));

			CFont::PrintString(SCREEN_RIGHT(x + 23.0f * i), SCREEN_TOP(y), IconToPrint);
		}
	}
	else if (GetGameMode() == GAMEMODE_SA) {
		strcpy_s(IconToPrint, "]");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(SA_FONT_GOTHIC);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(0);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		float fOffset = 0.0f;
		if (static_cast<float>(CWorld::Players[CWorld::PlayerInFocus].m_nMaxHealth) > 101.0f)
			fOffset = 12.0f;

		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8)) {
				CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_A));
				CFont::SetEdge(1);
			}
			else {
				CFont::SetEdge(0);
				CFont::SetColor(CRGBA(s.HUD_COLOR_WANTED_N));
			}

			CFont::PrintString(SCREEN_RIGHT(x + 18.0f * i), SCREEN_TOP(y + fOffset), IconToPrint);
		}
	}
	else if (GetGameMode() == GAMEMODE_LCS) {

	}
	else if (GetGameMode() == GAMEMODE_VCS) {

	}
	else if (GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawStatsBox(float x, float y, float w, float h) {
	if (GetGameMode() == GAMEMODE_III) {

	}
	else if (GetGameMode() == GAMEMODE_VC) {

	}
	else if (GetGameMode() == GAMEMODE_SA) {

	}
	else if (GetGameMode() == GAMEMODE_LCS) {

	}
	else if (GetGameMode() == GAMEMODE_VCS) {

	}
	else if (GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawAreaName(float x, float y, float w, float h) {
	if (CHud::m_pZoneName) {
		float fZoneAlpha = 0.0f;
		int State = CHud::m_ZoneState;
		if (CHud::m_pZoneName != CHud::m_pLastZoneName) {
			switch (CHud::m_ZoneState) {
			case 0:
				if (!CTheScripts::bPlayerIsOffTheMap && CTheScripts::bDisplayHud ||
					CEntryExitManager::ms_exitEnterState == 1 ||
					CEntryExitManager::ms_exitEnterState == 2) {
					State = 2;
					CHud::m_ZoneState = 2;
					CHud::m_ZoneNameTimer = 0;
					CHud::m_ZoneFadeTimer = 0;
					CHud::m_ZoneToPrint = CHud::m_pZoneName;

					if (CHud::m_VehicleState == 1 || CHud::m_VehicleState == 2)
						CHud::m_VehicleState = 3;
				}
				break;
			case 1:
			case 2:
			case 3:
				State = 4;
				CHud::m_ZoneState = 4;
			case 4:
				CHud::m_ZoneNameTimer = 0;
				break;
			default:
				break;
			}
			CHud::m_pLastZoneName = CHud::m_pZoneName;
		}

		fZoneAlpha = 255.0f;

		if (State) {
			switch (State) {
			case 1:
				if (CHud::m_ZoneNameTimer > AREA_NAME_TIME) {
					CHud::m_ZoneFadeTimer = 1000;
					CHud::m_ZoneState = 3;
				}

				fZoneAlpha = 255.0f;
				CHud::m_ZoneFadeTimer = 1000;
				break;
			case 2:
				CHud::m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02f * 1000.0f;
				if (CHud::m_ZoneFadeTimer > 1000) {
					CHud::m_ZoneState = 1;
					CHud::m_ZoneFadeTimer = 1000;
				}
				fZoneAlpha = CHud::m_ZoneFadeTimer * 0.001f * 255.0f;
				break;
			case 3:
				CHud::m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02f * -1000.0f;
				if (CHud::m_ZoneFadeTimer < 0) {
					CHud::m_ZoneState = 0;
					CHud::m_ZoneFadeTimer = 0;
				}
				fZoneAlpha = CHud::m_ZoneFadeTimer * 0.001f * 255.0f;
				break;
			case 4:
				CHud::m_ZoneFadeTimer += CTimer::ms_fTimeStep * 0.02f * -1000.0f;
				if (CHud::m_ZoneFadeTimer < 0) {
					CHud::m_ZoneFadeTimer = 0;
					CHud::m_ZoneState = 2;
					CHud::m_ZoneToPrint = CHud::m_pZoneName;
				}
				fZoneAlpha = CHud::m_ZoneFadeTimer * 0.001f * 255.0f;
				break;
			default:
				break;

			}
			if (!CHud::m_Message[0]) {
				CHud::m_ZoneNameTimer += CTimer::ms_fTimeStep * 0.02f * 1000.0f;
				if (GetGameMode() == GAMEMODE_III) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_RIGHT);
					CFont::SetRightJustifyWrap(0.0f);
					CFont::SetFontStyle(III_FONT_SUBTITLES);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
					CFont::SetColor(CRGBA(s.HUD_COLOR_ZONE_NAME.r, s.HUD_COLOR_ZONE_NAME.g, s.HUD_COLOR_ZONE_NAME.b, fZoneAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
				}
				else if (GetGameMode() == GAMEMODE_VC) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_RIGHT);
					CFont::SetRightJustifyWrap(0.0f);
					CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
					CFont::SetSlant(0.15f);
					CFont::SetFontStyle(VC_FONT_RAGE);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
					CFont::SetColor(CRGBA(s.HUD_COLOR_ZONE_NAME.r, s.HUD_COLOR_ZONE_NAME.g, s.HUD_COLOR_ZONE_NAME.b, fZoneAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
					CFont::SetSlant(0.0f);
				}
				else if (GetGameMode() == GAMEMODE_SA) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_RIGHT);
					CFont::SetRightJustifyWrap(0.0f);
					CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
					CFont::SetSlant(0.0f);
					CFont::SetFontStyle(SA_FONT_GOTHIC);
					CFont::SetDropShadowPosition(0);
					CFont::SetEdge(SA_EDGE_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
					CFont::SetColor(CRGBA(s.HUD_COLOR_ZONE_NAME.r, s.HUD_COLOR_ZONE_NAME.g, s.HUD_COLOR_ZONE_NAME.b, fZoneAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
					CFont::SetSlant(0.0f);
				}
				else if (GetGameMode() == GAMEMODE_LCS) {

				}
				else if (GetGameMode() == GAMEMODE_VCS) {

				}
				else if (GetGameMode() == GAMEMODE_ADVANCE) {

				}
			}
		}
	}
	else {
		CHud::m_pLastZoneName = NULL;
		CHud::m_ZoneState = 0;
		CHud::m_ZoneFadeTimer = 0;
		CHud::m_ZoneNameTimer = 0;
	}
}

void CHudNew::DrawVehicleName(float x, float y, float w, float h) {
	if (CHud::m_pVehicleName) {
		float fVehicleAlpha = 0.0f;
		int State = CHud::m_VehicleState;
		char *pLast = CHud::m_pLastVehicleName;

		if (CHud::m_pVehicleName != CHud::m_pLastVehicleName) {
			if (CHud::m_VehicleState) {
				if (CHud::m_VehicleState > 0 && CHud::m_VehicleState <= 4) {
					State = 4;
					CHud::m_VehicleState = 4;
					CHud::m_VehicleNameTimer = 0;
				}
			}
			else {
				State = 2;
				CHud::m_VehicleState = 2;
				CHud::m_VehicleNameTimer = 0;
				CHud::m_VehicleFadeTimer = 0;
				CHud::m_pVehicleNameToPrint = CHud::m_pVehicleName;
				if (CHud::m_ZoneState == 1 || CHud::m_ZoneState == 2)
					CHud::m_ZoneState = 3;
			}
			pLast = CHud::m_pVehicleName;
			CHud::m_pLastVehicleName = CHud::m_pVehicleName;
		}

		if (State) {
			switch (State) {
			case 1:
				if (CHud::m_VehicleNameTimer > VEHICLE_NAME_TIME) {
					CHud::m_VehicleFadeTimer = 1000;
					CHud::m_VehicleState = 3;
				}

				fVehicleAlpha = 255.0f;
				CHud::m_VehicleFadeTimer = 1000;
				break;
			case 2:
				CHud::m_VehicleFadeTimer += CTimer::ms_fTimeStep * 0.02f * 1000.0f;
				if (CHud::m_VehicleFadeTimer > 1000) {
					CHud::m_VehicleState = 1;
					CHud::m_VehicleFadeTimer = 1000;
				}
				fVehicleAlpha = CHud::m_VehicleFadeTimer * 0.001f * 255.0f;
				break;
			case 3:
				CHud::m_VehicleFadeTimer += CTimer::ms_fTimeStep * 0.02f * -1000.0f;
				if (CHud::m_VehicleFadeTimer < 0) {
					CHud::m_VehicleState = 0;
					CHud::m_VehicleFadeTimer = 0;
				}
				fVehicleAlpha = CHud::m_VehicleFadeTimer * 0.001f * 255.0f;
				break;
			case 4:
				CHud::m_VehicleFadeTimer += CTimer::ms_fTimeStep * 0.02f * -1000.0f;
				if (CHud::m_VehicleFadeTimer < 0) {
					CHud::m_VehicleFadeTimer = 0;
					CHud::m_VehicleState = 2;
					CHud::m_pVehicleNameToPrint = pLast;
				}
				fVehicleAlpha = CHud::m_VehicleFadeTimer * 0.001f * 255.0f;
				break;
			default:
				break;

			}

			if (!CHud::m_Message[0]) {
				CHud::m_VehicleNameTimer += CTimer::ms_fTimeStep * 0.02f * 1000.0f;
				if (GetGameMode() == GAMEMODE_III) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_RIGHT);
					CFont::SetRightJustifyWrap(0.0f);
					CFont::SetFontStyle(III_FONT_SUBTITLES);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
					CFont::SetColor(CRGBA(s.HUD_COLOR_VEHICLE_NAME.r, s.HUD_COLOR_VEHICLE_NAME.g, s.HUD_COLOR_VEHICLE_NAME.b, fVehicleAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
				}
				else if (GetGameMode() == GAMEMODE_VC) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_RIGHT);
					CFont::SetRightJustifyWrap(0.0f);
					CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
					CFont::SetSlant(0.15f);
					CFont::SetFontStyle(VC_FONT_RAGE);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
					CFont::SetColor(CRGBA(s.HUD_COLOR_VEHICLE_NAME.r, s.HUD_COLOR_VEHICLE_NAME.g, s.HUD_COLOR_VEHICLE_NAME.b, fVehicleAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
					CFont::SetSlant(0.0f);
				}
				else if (GetGameMode() == GAMEMODE_SA) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_RIGHT);
					CFont::SetRightJustifyWrap(0.0f);
					CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
					CFont::SetSlant(0.0f);
					CFont::SetFontStyle(SA_FONT_MENU);
					CFont::SetDropShadowPosition(0);
					CFont::SetEdge(SA_EDGE_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
					CFont::SetColor(CRGBA(s.HUD_COLOR_VEHICLE_NAME.r, s.HUD_COLOR_VEHICLE_NAME.g, s.HUD_COLOR_VEHICLE_NAME.b, fVehicleAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
					CFont::SetSlant(0.0f);
				}
				else if (GetGameMode() == GAMEMODE_LCS) {

				}
				else if (GetGameMode() == GAMEMODE_VCS) {

				}
				else if (GetGameMode() == GAMEMODE_ADVANCE) {

				}
			}
		}
	}
	else {
		CHud::m_pLastVehicleName = NULL;
		CHud::m_VehicleState = 0;
		CHud::m_VehicleFadeTimer = 0;
		CHud::m_VehicleNameTimer = 0;
	}
}

void CHudNew::DrawRadioStation(float x, float y, float w, float h) {
	int pRetune = 0;

	if (!CTimer::m_UserPause
		&& !CTimer::m_CodePause
		&& !TheCamera.m_bWideScreenOn
		&& FindPlayerVehicle(-1, 0)
		&& CReplay::Mode != 1)
	{
		if (AERadioTrackManager.field_1 && AERadioTrackManager.IsVehicleRadioActive())
		{
			AERadioTrackManager.m_nTimeToDisplayRadioName = CTimer::m_snTimeInMilliseconds + 2500;
			AERadioTrackManager.field_1 = 0;
		}
		if (CTimer::m_snTimeInMilliseconds < AERadioTrackManager.m_nTimeToDisplayRadioName)
		{
			pRetune = AERadioTrackManager.m_nStationsListed + AERadioTrackManager.m_nCurrentRadioStation;
			if (pRetune) {
				if (pRetune > 0) {
					if (pRetune >= 14)
						pRetune = LOBYTE(pRetune) - 13;
				}
				else 
					pRetune = LOBYTE(pRetune) + 13;

				char *pRadioName = AERadioTrackManager.GetRadioStationName(pRetune);

				if (GetGameMode() == GAMEMODE_III) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetCentreSize(SCREEN_LEFT(640.0f));
					CFont::SetFontStyle(III_FONT_PRICEDOWN);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

					if (AERadioTrackManager.m_nStationsListed || AERadioTrackManager.m_nStationsListDown)
						CFont::SetColor(CRGBA(s.HUD_COLOR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (GetGameMode() == GAMEMODE_VC) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetCentreSize(SCREEN_LEFT(640.0f));
					CFont::SetFontStyle(VC_FONT_SUBTITLES);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(III_VC_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

					if (AERadioTrackManager.m_nStationsListed || AERadioTrackManager.m_nStationsListDown)
						CFont::SetColor(CRGBA(s.HUD_COLOR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (GetGameMode() == GAMEMODE_SA) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetCentreSize(SCREEN_LEFT(640.0f));
					CFont::SetFontStyle(SA_FONT_MENU);
					CFont::SetDropShadowPosition(0);
					CFont::SetEdge(1);
					CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

					if (AERadioTrackManager.m_nStationsListed || AERadioTrackManager.m_nStationsListDown)
						CFont::SetColor(CRGBA(s.HUD_COLOR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (GetGameMode() == GAMEMODE_LCS) {

				}
				else if (GetGameMode() == GAMEMODE_VCS) {

				}
				else if (GetGameMode() == GAMEMODE_ADVANCE) {

				}
				CFont::DrawFonts();
			}
		}
	}
}

void CHudNew::DrawTextBox() {
	if (CTimer::m_UserPause || CReplay::Mode != 1)
		return;

	if (CHud::m_pHelpMessage[0]) {
		if (!CMessages::StringCompare(CHud::m_pHelpMessage, CHud::m_pLastHelpMessage, 400)) {
			switch (CHud::m_nHelpMessageState) {
			case 0:
				CHud::m_nHelpMessageState = 2;
				CHud::m_nHelpMessageTimer = 0;
				CHud::m_nHelpMessageFadeTimer = 0;
				CMessages::StringCopy(CHud::m_pHelpMessageToPrint, CHud::m_pHelpMessage, 400);
				CHud::m_fHelpMessageTime = CMessages::GetStringLength(CHud::m_pHelpMessage) * 0.05f + 3.0f;
				// CAudioEngine::ReportFrontendAudioEvent missing.
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				CHud::m_nHelpMessageState = 4;
				CHud::m_nHelpMessageTimer = 5;
				break;
			default:
				break;
			}
			CMessages::StringCopy(CHud::m_pLastHelpMessage, CHud::m_pHelpMessage, 400);
		}

		float fAlpha = 255.0f;

		if (CHud::m_nHelpMessageState) {
			switch (CHud::m_nHelpMessageState) {
			case 1:
				fAlpha = 255.0f;
				CHud::m_nHelpMessageFadeTimer = 600;
				if (CHud::m_nHelpMessageFadeTimer > CHud::m_fHelpMessageTime * 1000 || CHud::m_bHelpMessageQuick && CHud::m_nHelpMessageTimer > 1500) {
					CHud::m_nHelpMessageFadeTimer = 0;
					CHud::m_nHelpMessageState = 1;
				}
				break;
			case 2:
				CHud::m_nHelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02f * 1000.0f);
				if (CHud::m_nHelpMessageFadeTimer > 0) {
					CHud::m_nHelpMessageFadeTimer = 0;
					CHud::m_nHelpMessageState = 1;
				}
				fAlpha = CHud::m_nHelpMessageFadeTimer * 0.001f * 255.0f;
				break;
			case 3:
				CHud::m_nHelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02f * -1000.0f);
				if (CHud::m_nHelpMessageFadeTimer >= 0) {
					CHud::m_nHelpMessageFadeTimer = 0;
					CHud::m_nHelpMessageState = 0;
				}
				fAlpha = CHud::m_nHelpMessageFadeTimer * 0.001f * 255.0f;
				break;
			case 4:
				CHud::m_nHelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02f * -1000.0f);
				if (CHud::m_nHelpMessageFadeTimer >= 0) {
					CHud::m_nHelpMessageFadeTimer = 0;
					CHud::m_nHelpMessageState = 2;
					CMessages::StringCopy(CHud::m_pHelpMessageToPrint, CHud::m_pLastHelpMessage, 400);
				}
				fAlpha = CHud::m_nHelpMessageFadeTimer * 0.001f * 255.0f;
				break;
			default:
				break;
			}

			CHud::m_nHelpMessageTimer += (CTimer::ms_fTimeStep * 0.02f * 1000.0f);

			if (GetGameMode() == GAMEMODE_III) {

			}
			else if (GetGameMode() == GAMEMODE_VC) {

			}
			else if (GetGameMode() == GAMEMODE_SA) {

			}
			else if (GetGameMode() == GAMEMODE_LCS) {

			}
			else if (GetGameMode() == GAMEMODE_VCS) {

			}
			else if (GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}
	}
	else {
		CHud::m_nHelpMessageState = 0;
	}
}

void CHudNew::DrawProgressBar(float x, float y, float width, float height, float progress, char border, char shadow, CRGBA colorFront, CRGBA colorBack) {
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

void CHudNew::InjectPatches() {
	Events::initRwEvent += CHudNew::Initialise;
	Events::drawMenuBackgroundEvent += CHudNew::ReInitialise;
	Events::shutdownRwEvent += CHudNew::Shutdown;
	Events::drawHudEvent += CHudNew::Draw;

#if GTASA
	patch::Set(0x58FBD6, 5); // CHud::DrawPlayerInfo
	patch::Set(0x58FBDB, 5); // CHud::DrawWanted
	patch::Set(0x58D542, 5); // CHud::DrawAreaName
	patch::Set(0x58FBE9, 5); // CHud::DrawVehicleName
	patch::Set(0x53E4FA, 5); // CAudioEngine::DisplayRadioStationName
#endif
}