#include "plugin.h"
#include "ClassicHud.h"
#include "CHudNew.h"
#include "CHudNumbers.h"
#include "CProgressBar.h"
#include "CFontNew.h"
#include "CRadarNew.h"
#include "ScreenAddons.h"
#include "Settings.h"

#if GTASA
#include "CHudColours.h"
#include "CCamera.h"
#include "CClock.h"
#include "CHud.h"
#include "CFont.h"
#include "CMenuManager.h"
#include "CTheScripts.h"
#include "CTimer.h"
#include "CCutsceneMgr.h"
#include "CModelInfo.h"
#include "CStats.h"
#include "CMessages.h"
#include "CGarages.h"
#include "CReplay.h"
#include "eModelID.h"
#include "CDarkel.h"
#include "CEntryExitManager.h"
#include "CAERadioTrackManager.h"
#include "CText.h"
#include "CRadar.h"
#include "CMenuSystem.h"
#include "CTxdStore.h"
#include "CWorld.h"
#include "eWeaponType.h"
#endif

using namespace plugin;

CHudNew chudnew;
CSprite2d CHudNew::SpritesNew[HUD_TOTALSPRITES];
CSprite2d CHudNew::SharedSprites[SHARED_TOTALSPRITES];
bool CHudNew::ms_bSpritesLoaded;

char *SpritesModNames[HUD_TOTALSPRITES] = {
	"unarmed"
};

char *SharedNames[SHARED_TOTALSPRITES] = {
	"breathicon",
	"skipicon"
};

CHudNew::CHudNew() {
	s.readIni();

	if (!s.m_bEnable)
		return;

#if GTASA
	patch::PutRetn(0x58FAE0); // CHud::Draw
	patch::PutRetn(0x58D490); // CHud::DrawAfterFade
	patch::Set(0x53E4FA, 5); // CAudioEngine::DisplayRadioStationName
#endif
}

void CHudNew::Initialise() {
	s.readDat();

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

void CHudNew::Draw() {
	ScreenAddons::SetScreenMult(s.m_fHudW, s.m_fHudH);

	if (CReplay::Mode != 1
		&& !CWeapon::ms_bTakePhoto
		&& !FrontEndMenuManager.m_bActivateMenuNextFrame) {
		RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);
		RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
		RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
		RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSMIRROR);
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)FALSE);
		RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
		RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);

		if (!TheCamera.m_bWideScreenOn) {
			CHud::DrawCrossHairs();

			if (FrontEndMenuManager.m_bHudOn && CTheScripts::bDisplayHud) {
				/*
					DrawPlayerInfo
				*/
				// 1st
				if (CWorld::Players[0].m_pPed) {
					if (CHud::bDrawClock)
						DrawClock(s.m_fClock.left, s.m_fClock.top, s.m_fClock.right, s.m_fClock.bottom);

					DrawMoneyCounter(s.m_fMoney.left, s.m_fMoney.top, s.m_fMoney.right, s.m_fMoney.bottom);
					DrawHealth(0, s.m_fHealth.left, s.m_fHealth.top, s.m_fHealth.right, s.m_fHealth.bottom);
					DrawArmour(0, s.m_fArmour.left, s.m_fArmour.top, s.m_fArmour.right, s.m_fArmour.bottom);
					if (CWorld::Players[0].m_pPed->m_fArmour > 1.0)
						DrawBreath(0, s.m_fBreath.left, s.m_fBreath.top, s.m_fBreath.right, s.m_fBreath.bottom);
					else
						DrawBreath(0, s.m_fArmour.left, s.m_fArmour.top, s.m_fArmour.right, s.m_fArmour.bottom);
					DrawWeaponIcon(0, s.m_fWeapon.left, s.m_fWeapon.top, s.m_fWeapon.right, s.m_fWeapon.bottom);
					DrawAmmo(0, s.m_fAmmo.left, s.m_fAmmo.top, s.m_fAmmo.right, s.m_fAmmo.bottom);
				}
				// 2nd
				int _2nd = 1;
				if (CWorld::Players[_2nd].m_pPed) {
					float fOffset = 134.0f;
					DrawHealth(_2nd, s.m_fHealth.left, s.m_fHealth.top + fOffset, s.m_fHealth.right, s.m_fHealth.bottom);
					DrawArmour(_2nd, s.m_fArmour.left, s.m_fArmour.top + fOffset, s.m_fArmour.right, s.m_fArmour.bottom);
					if (CWorld::Players[_2nd].m_pPed->m_fArmour > 1.0)
						DrawBreath(_2nd, s.m_fBreath.left, s.m_fBreath.top + fOffset, s.m_fBreath.right, s.m_fBreath.bottom);
					else
						DrawBreath(_2nd, s.m_fArmour.left, s.m_fArmour.top + fOffset, s.m_fArmour.right, s.m_fArmour.bottom);
					DrawWeaponIcon(_2nd, s.m_fWeapon.left, s.m_fWeapon.top + fOffset, s.m_fWeapon.right, s.m_fWeapon.bottom + fOffset);
					DrawAmmo(_2nd, s.m_fAmmo.left, s.m_fAmmo.top + fOffset, s.m_fAmmo.right, s.m_fAmmo.bottom);
				}
			}

			if (!CHud::bScriptDontDisplayVehicleName)
				CHudNew::DrawVehicleName(s.m_fVehicleName.left, s.m_fVehicleName.top, s.m_fVehicleName.right, s.m_fVehicleName.bottom);
			
			if (!CHud::bScriptDontDisplayAreaName && !TheCamera.m_bWideScreenOn)
				DrawAreaName(s.m_fZoneName.left, s.m_fZoneName.top, s.m_fZoneName.right, s.m_fZoneName.bottom);

			CHud::DrawMissionTimers();

			DrawWanted(s.m_fWanted.left, s.m_fWanted.top, s.m_fWanted.right, s.m_fWanted.bottom);
			DrawRadioStation(s.m_fRadioName.left, s.m_fRadioName.top, s.m_fRadioName.right, s.m_fRadioName.bottom);
		}
		if (!CHud::bScriptDontDisplayRadar && !TheCamera.m_bWideScreenOn) {
			if (!CPad::GetPad(0)->GetDisplayVitalStats(FindPlayerPed()) || FindPlayerVehicle(-1, 0)) {
				CHud::bDrawingVitalStats = 0;
				CRadarNew::DrawRadar();
			}
			else {
				CHud::bDrawingVitalStats = 1;
				CHud::DrawVitalStats();
			}
			if (/*!CGameLogic::SkipCanBeActivated() || */CHud::bDrawingVitalStats) {
				//HelpTripSkipShown = 0;
			}
			else {
				//CHud::DrawTripSkip();
				if (0 /*!HelpTripSkipShown*/) {
					//CHud::SetHelpMessage(TheText.Get("SKIP_1"), 1, 0, 0);
					// HelpTripSkipShown = 1;
				}
			}
		}

		if (CHud::m_bDraw3dMarkers && !TheCamera.m_bWideScreenOn)
			CRadar::Draw3dMarkers();

		if (!CTimer::m_UserPause) {
			if (!CHud::m_BigMessage[0][0]) {
				if (CMenuSystem::num_menus_in_use)
					CMenuSystem::Process(-99);
				CHud::DrawScriptText(1);
			}

			if (CTheScripts::bDrawSubtitlesBeforeFade)
				CHudNew::DrawSubtitles(s.m_fSubtitles.left, s.m_fSubtitles.top, s.m_fSubtitles.right, s.m_fSubtitles.bottom);

			DrawHelpText(s.m_fTextBox.left, s.m_fTextBox.top, s.m_fTextBox.right, s.m_fTextBox.bottom);
			DrawOddJobMessage(s.m_fTextBox.left, s.m_fTextBox.top, s.m_fTextBox.right, s.m_fTextBox.bottom);;
			DrawSuccessFailedMessage(s.m_fSuccessFailedMessage.left, s.m_fSuccessFailedMessage.top, s.m_fSuccessFailedMessage.right, s.m_fSuccessFailedMessage.bottom);
			DrawWastedBustedMessage(s.m_fWastedBustedText.left, s.m_fWastedBustedText.top, s.m_fWastedBustedText.right, s.m_fWastedBustedText.bottom);
		}
	
		ClassicHud::ms_bReload = false;
	}

	ScreenAddons::SetScreenMult(DEFAULT_HUD_SCALE);
}

void CHudNew::DrawAfterFade() {
	RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)TRUE);
	RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSMIRROR);
	RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);

	if (!CTimer::m_UserPause && CReplay::Mode != 1 && !CWeapon::ms_bTakePhoto) {
		if ((!FindPlayerVehicle(-1, 0)
			|| FindPlayerVehicle(-1, 0)->m_nVehicleSubClass != 4
			&& FindPlayerVehicle(-1, 0)->m_nVehicleSubClass != 3)
			&& !CCutsceneMgr::ms_cutsceneProcessing
			&& !FrontEndMenuManager.m_bMenuActive
			&& !TheCamera.m_bWideScreenOn
			&& !CHud::bScriptDontDisplayAreaName) {
		}

		if (!CHud::m_BigMessage[0][0])
			CHud::DrawScriptText(0);

		if (!CTheScripts::bDrawSubtitlesBeforeFade)
			DrawSubtitles(s.m_fSubtitles.left, s.m_fSubtitles.top, s.m_fSubtitles.right, s.m_fSubtitles.bottom);

		DrawMissionTitle(s.m_fMissionTitle.left, s.m_fMissionTitle.top, s.m_fMissionTitle.right, s.m_fMissionTitle.bottom);
	}
}

void CHudNew::DrawClock(float x, float y, float w, float h) {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "%02d:%02d", CClock::ms_nGameClockHours, CClock::ms_nGameClockMinutes);

	if (ClassicHud::GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_CLOCK));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(VC_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_CLOCK));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(SA_FONT_PRICEDOWN);
		CFont::SetDropShadowPosition(0);
		CFont::SetEdge(EDGE_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_CLOCK));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
		CHudNumbers::SetFontStyle(0);
		CHudNumbers::SetColor(CRGBA(s.HUD_COLOUR_CLOCK));
		CHudNumbers::SetFontScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CHudNumbers::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawMoneyCounter(float x, float y, float w, float h) {
	char TextToPrint[16];
	sprintf_s(TextToPrint, s.bMinimalMoneyCounter ? "$%d" : "$%08d", CWorld::Players[CWorld::PlayerInFocus].m_nDisplayMoney);

	if (ClassicHud::GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_CASH));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(VC_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_CASH));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(SA_FONT_PRICEDOWN);
		CFont::SetDropShadowPosition(0);
		CFont::SetEdge(EDGE_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_CASH));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		float fOffset = 0.0f;
		if (static_cast<float>(CWorld::Players[CWorld::PlayerInFocus].m_nMaxHealth) > 101.0f)
			fOffset = 12.0f;

		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y + fOffset), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
		CHudNumbers::SetFontStyle(1);
		CHudNumbers::SetColor(CRGBA(s.HUD_COLOUR_CASH));
		CHudNumbers::SetFontScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CHudNumbers::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawHealth(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];
	float fProgress = CWorld::Players[PlayerID].m_pPed->m_fHealth / static_cast<float>(CWorld::Players[PlayerID].m_nMaxHealth);
	int nProgress = static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_fHealth);

	if (CWorld::Players[PlayerID].m_pPed->m_fHealth >= 10 || CTimer::m_FrameCounter & 8)
	if (CHud::m_ItemToFlash != 4 || CTimer::m_FrameCounter & 8) {
		if (ClassicHud::GetGameMode() == GAMEMODE_III) {
			strcpy_s(IconToPrint, "{");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(III_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_HEALTH));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == CWorld::Players[PlayerID].m_nLastTimeEnergyLost || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeEnergyLost + 1500 || CTimer::m_FrameCounter & 8)
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
			strcpy_s(IconToPrint, "{");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(VC_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_HEALTH));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == CWorld::Players[PlayerID].m_nLastTimeEnergyLost || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeEnergyLost + 1500 || CTimer::m_FrameCounter & 8) {
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
			}
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
			float fOffset = 0.0f;

			if (static_cast<float>(CWorld::Players[PlayerID].m_nMaxHealth) > 101.0f)
				fOffset = 12.0f;

			float fBarSize = static_cast<float>(CWorld::Players[PlayerID].m_nMaxHealth) / CStats::GetFatAndMuscleModifier(STAT_MOD_10);

			if (fBarSize > 1.0f)
				fBarSize = 1.0f;

			CProgressBar::DrawProgressBar(SCREEN_RIGHT(x + (w * fBarSize)), SCREEN_TOP(y + fOffset), SCREEN_LEFT(w * fBarSize), SCREEN_TOP(h), fProgress, SCREEN_TOP(2.5f), 0, CRGBA(s.HUD_COLOUR_HEALTH), CRGBA(s.HUD_COLOUR_HEALTH.r, s.HUD_COLOUR_HEALTH.g, s.HUD_COLOUR_HEALTH.b, 150));
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
			strcpy_s(IconToPrint, "+");

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(LCS_FONT_SUBTITLES);
			CFont::SetDropShadowPosition(0);
			CFont::SetEdge(EDGE_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_HEALTH.a));
			CFont::SetColor(CRGBA(255, 255, 255, s.HUD_COLOUR_HEALTH.a));
			CFont::SetScale(SCREEN_LEFT(w * 0.008f), SCREEN_TOP(h * 0.08f));

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == CWorld::Players[PlayerID].m_nLastTimeEnergyLost || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeEnergyLost + 1500 || CTimer::m_FrameCounter & 8) {
				CProgressBar::DrawProgressBarWithSprites(SCREEN_RIGHT(x), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, CRGBA(s.HUD_COLOUR_HEALTH));
				
				if (CWorld::Players[PlayerID].m_nMaxHealth >= 150)
					CFont::PrintString(SCREEN_RIGHT(x - 34.5f), SCREEN_TOP(y - 4.0f), IconToPrint);
			}
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

		}
	}
}

void CHudNew::DrawArmour(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];
	float fProgress = CWorld::Players[PlayerID].m_pPed->m_fArmour / static_cast<float>(CWorld::Players[PlayerID].m_nMaxArmour);
	int nProgress = static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_fArmour);

	if ((CHud::m_ItemToFlash != 3 || CTimer::m_FrameCounter & 8) && CWorld::Players[PlayerID].m_pPed->m_fArmour > 1.0) {
		if (ClassicHud::GetGameMode() == GAMEMODE_III) {
			strcpy_s(IconToPrint, "[");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(III_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_ARMOUR.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_ARMOUR));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == 0 || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeArmourLost + 1500 || CTimer::m_FrameCounter & 8)
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
			strcpy_s(IconToPrint, "<");
			sprintf_s(TextToPrint, "%03d", nProgress);

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(VC_FONT_PRICEDOWN);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_ARMOUR.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_ARMOUR));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

			if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == 0 || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeArmourLost + 1500 || CTimer::m_FrameCounter & 8) {
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
				CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
			}
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
			CProgressBar::DrawProgressBar(SCREEN_RIGHT(x + w), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, SCREEN_TOP(2.5f), 0, CRGBA(s.HUD_COLOUR_ARMOUR), CRGBA(s.HUD_COLOUR_ARMOUR.r, s.HUD_COLOUR_ARMOUR.g, s.HUD_COLOUR_ARMOUR.b, 150));
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
			strcpy_s(IconToPrint, "+");

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(LCS_FONT_SUBTITLES);
			CFont::SetDropShadowPosition(0);
			CFont::SetEdge(EDGE_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_ARMOUR.a));
			CFont::SetColor(CRGBA(255, 255, 255, s.HUD_COLOUR_ARMOUR.a));
			CFont::SetScale(SCREEN_LEFT(w * 0.008f), SCREEN_TOP(h * 0.08f));

			if (CWorld::Players[PlayerID].m_pPed->m_nPhysicalFlags.bSubmergedInWater && ((s.m_fArmour.left == s.m_fBreath.left) || (s.m_fArmour.top == s.m_fBreath.top))) {
			}
			else {
				if (CWorld::Players[PlayerID].m_nLastTimeArmourLost == 0 || CTimer::m_snTimeInMilliseconds > CWorld::Players[PlayerID].m_nLastTimeArmourLost + 1500 || CTimer::m_FrameCounter & 8) {
					CProgressBar::DrawProgressBarWithSprites(SCREEN_RIGHT(x), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, CRGBA(s.HUD_COLOUR_ARMOUR));

					if (CWorld::Players[PlayerID].m_nMaxArmour >= 150)
						CFont::PrintString(SCREEN_RIGHT(x - 34.5f), SCREEN_TOP(y - 4.0f), IconToPrint);
				}
			}
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

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
			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				sprintf_s(TextToPrint, "%03d", nProgress);

				CFont::SetProportional(false);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(III_FONT_PRICEDOWN);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_BREATH.a));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_BREATH));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 70.5f - HUD_SHADOW_SIZE), SCREEN_TOP(y + 6.0f + HUD_SHADOW_SIZE), SCREEN_RIGHT(x + 70.5f - HUD_SHADOW_SIZE) + SCREEN_LEFT(w * 24.6f), SCREEN_TOP(y + 6.0f + HUD_SHADOW_SIZE) + SCREEN_TOP(h * 13.0f)), CRGBA(0, 0, 0, s.HUD_COLOUR_BREATH.a));
				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 70.5f), SCREEN_TOP(y + 6.0f), SCREEN_RIGHT(x + 70.5f) + SCREEN_LEFT(w * 24.6f), SCREEN_TOP(y + 6.0f) + SCREEN_TOP(h * 13.0f)), CRGBA(s.HUD_COLOUR_BREATH));
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				sprintf_s(TextToPrint, "%03d", nProgress);

				CFont::SetProportional(false);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(VC_FONT_PRICEDOWN);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_BREATH.a));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_BREATH));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);

				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 69.5f), SCREEN_TOP(y + 7.5f), SCREEN_RIGHT(x + 69.5f) + SCREEN_LEFT(w * 25.0f), SCREEN_TOP(y + 7.5f) + SCREEN_TOP(h * 13.0f)), CRGBA(0, 0, 0, s.HUD_COLOUR_BREATH.a));
				SharedSprites[SHARED_BREATHICON].Draw(CRect(SCREEN_RIGHT(x + 71.5f), SCREEN_TOP(y + 6.0f), SCREEN_RIGHT(x + 71.5f) + SCREEN_LEFT(w * 25.0f), SCREEN_TOP(y + 6.0f) + SCREEN_TOP(h * 13.0f)), CRGBA(s.HUD_COLOUR_BREATH));
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CProgressBar::DrawProgressBar(SCREEN_RIGHT(x + w), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, SCREEN_TOP(2.5f), 0, CRGBA(s.HUD_COLOUR_BREATH), CRGBA(s.HUD_COLOUR_BREATH.r, s.HUD_COLOUR_BREATH.g, s.HUD_COLOUR_BREATH.b, 150));
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				strcpy_s(IconToPrint, "+");

				CFont::SetProportional(false);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(LCS_FONT_SUBTITLES);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(EDGE_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_BREATH.a));
				CFont::SetColor(CRGBA(255, 255, 255, s.HUD_COLOUR_BREATH.a));
				CFont::SetScale(SCREEN_LEFT(w * 0.008f), SCREEN_TOP(h * 0.08f));

				CProgressBar::DrawProgressBarWithSprites(SCREEN_RIGHT(x), SCREEN_TOP(y), SCREEN_LEFT(w), SCREEN_TOP(h), fProgress, CRGBA(s.HUD_COLOUR_BREATH));
				CFont::PrintString(SCREEN_RIGHT(x - 34.5f), SCREEN_TOP(y - 4.0f), IconToPrint);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}
	}
}

void CHudNew::DrawWeaponIcon(int PlayerID, float x, float y, float w, float h) {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERMIPNEAREST);
	int ModelId = CWeaponInfo::GetWeaponInfo(CWorld::Players[PlayerID].m_pPed->m_aWeapons[CWorld::Players[PlayerID].m_pPed->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1;
	int Slot = CTxdStore::FindTxdSlot("weapons");
	CBaseModelInfo *index = CModelInfo::GetModelInfo(ModelId);
	RwTexDictionary *txd = CTxdStore::ms_pTxdPool->GetAt(Slot)->m_pRwDictionary;
	CRGBA color = CRGBA(s.HUD_COLOUR_WEAPON_ICON);

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
		if (ClassicHud::GetGameMode() == GAMEMODE_III) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(III_FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(0);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_AMMO.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(VC_FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(SA_FONT_SUBTITLES);
			CFont::SetDropShadowPosition(0);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_AMMO.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
			CFont::SetProportional(true);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetCentreSize(SCREEN_LEFT(640.0f));
			CFont::SetFontStyle(LCS_FONT_SUBTITLES);
			CFont::SetDropShadowPosition(0);
			CFont::SetEdge(1);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_AMMO.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_AMMO));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

		}
	}
}

void CHudNew::DrawWanted(float x, float y, float w, float h) {
	char IconToPrint[16];

	if (ClassicHud::GetGameMode() == GAMEMODE_III) {
		strcpy_s(IconToPrint, "]");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_WANTED_N.a));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_N));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8))
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_A));
			else {
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_N));
				CFont::SetDropShadowPosition(0);
			}

			CFont::PrintString(SCREEN_RIGHT(x + 23.0f * i), SCREEN_TOP(y), IconToPrint);
		}
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
		strcpy_s(IconToPrint, ">");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(III_FONT_PRICEDOWN);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_WANTED_N.a));
		CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_N));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8)) 
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_A));
			else
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_N));

			CFont::PrintString(SCREEN_RIGHT(x + 23.0f * i), SCREEN_TOP(y), IconToPrint);
		}
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
		strcpy_s(IconToPrint, "]");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(SA_FONT_GOTHIC);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(0);
		CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_WANTED_N.a));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		float fOffset = 0.0f;
		if (static_cast<float>(CWorld::Players[CWorld::PlayerInFocus].m_nMaxHealth) > 101.0f)
			fOffset = 12.0f;

		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8)) {
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_A));
				CFont::SetEdge(1);
			}
			else {
				CFont::SetEdge(0);
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WANTED_N));
			}

			CFont::PrintString(SCREEN_RIGHT(x + 18.0f * i), SCREEN_TOP(y + fOffset), IconToPrint);
		}
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
		strcpy_s(IconToPrint, "*");

		CHudNumbers::SetFontStyle(0);
		for (unsigned int i = 0; i < 6; i++) {
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8))
				CHudNumbers::SetColor(CRGBA(s.HUD_COLOUR_WANTED_A));
			else
				CHudNumbers::SetColor(CRGBA(s.HUD_COLOUR_WANTED_N));

			CHudNumbers::SetFontScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CHudNumbers::PrintString(SCREEN_RIGHT(x + 20.0f * i), SCREEN_TOP(y), IconToPrint);
		}
	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawStatsBox(float x, float y, float w, float h) {
	if (ClassicHud::GetGameMode() == GAMEMODE_III) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CHudNew::DrawAreaName(float x, float y, float w, float h) {
	float fZoneAlpha;

	if (CHud::m_pZoneName) {
		int State = CHud::m_ZoneState;
		char *pLast = CHud::m_pLastZoneName;
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
			pLast = CHud::m_pZoneName;
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
					CHud::m_ZoneToPrint = pLast;
				}
				fZoneAlpha = CHud::m_ZoneFadeTimer * 0.001f * 255.0f;
				break;
			default:
				break;

			}
			CHud::m_ZoneNameTimer += CTimer::ms_fTimeStep * 0.02f * 1000.0f;
			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(III_FONT_SUBTITLES);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_ZONE_NAME.r, s.HUD_COLOUR_ZONE_NAME.g, s.HUD_COLOUR_ZONE_NAME.b, fZoneAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
				CFont::SetSlant(0.15f);
				CFont::SetFontStyle(VC_FONT_RAGE);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_ZONE_NAME.r, s.HUD_COLOUR_ZONE_NAME.g, s.HUD_COLOUR_ZONE_NAME.b, fZoneAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
				CFont::SetSlant(0.0f);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
				CFont::SetSlant(0.0f);
				CFont::SetFontStyle(SA_FONT_GOTHIC);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(EDGE_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_ZONE_NAME.r, s.HUD_COLOUR_ZONE_NAME.g, s.HUD_COLOUR_ZONE_NAME.b, fZoneAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
				CFont::SetSlant(0.0f);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
				CFont::SetSlant(0.0f);
				CFont::SetFontStyle(LCS_FONT_BANK);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_ZONE_NAME.r, s.HUD_COLOUR_ZONE_NAME.g, s.HUD_COLOUR_ZONE_NAME.b, fZoneAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
				CFont::SetSlant(0.0f);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

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

			CHud::m_VehicleNameTimer += CTimer::ms_fTimeStep * 0.02f * 1000.0f;
			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetFontStyle(III_FONT_SUBTITLES);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_VEHICLE_NAME.r, s.HUD_COLOUR_VEHICLE_NAME.g, s.HUD_COLOUR_VEHICLE_NAME.b, fVehicleAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
				CFont::SetSlant(0.15f);
				CFont::SetFontStyle(VC_FONT_RAGE);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_VEHICLE_NAME.r, s.HUD_COLOUR_VEHICLE_NAME.g, s.HUD_COLOUR_VEHICLE_NAME.b, fVehicleAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
				CFont::SetSlant(0.0f);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
				CFont::SetSlant(0.0f);
				CFont::SetFontStyle(SA_FONT_MENU);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(EDGE_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_VEHICLE_NAME.r, s.HUD_COLOUR_VEHICLE_NAME.g, s.HUD_COLOUR_VEHICLE_NAME.b, fVehicleAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
				CFont::SetSlant(0.0f);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetRightJustifyWrap(0.0f);
				CFont::SetSlantRefPoint(SCREEN_RIGHT(x), SCREEN_BOTTOM(y));
				CFont::SetSlant(0.0f);
				CFont::SetFontStyle(LCS_FONT_BANK);
				CFont::SetEdge(0);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_VEHICLE_NAME.r, s.HUD_COLOUR_VEHICLE_NAME.g, s.HUD_COLOUR_VEHICLE_NAME.b, fVehicleAlpha));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
				CFont::SetSlant(0.0f);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

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

				if (ClassicHud::GetGameMode() == GAMEMODE_III) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetCentreSize(SCREEN_LEFT(640.0f));
					CFont::SetFontStyle(III_FONT_PRICEDOWN);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

					if (AERadioTrackManager.m_nStationsListed || AERadioTrackManager.m_nStationsListDown)
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetCentreSize(SCREEN_LEFT(640.0f));
					CFont::SetFontStyle(VC_FONT_SUBTITLES);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

					if (AERadioTrackManager.m_nStationsListed || AERadioTrackManager.m_nStationsListDown)
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
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
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
					CFont::SetProportional(true);
					CFont::SetBackground(false, false);
					CFont::SetOrientation(ALIGN_CENTER);
					CFont::SetCentreSize(SCREEN_LEFT(640.0f));
					CFont::SetFontStyle(LCS_FONT_BANK);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, 255));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

					if (AERadioTrackManager.m_nStationsListed || AERadioTrackManager.m_nStationsListDown)
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_N));
					else
						CFont::SetColor(CRGBA(s.HUD_COLOUR_RADIO_NAME_A));

					CFont::PrintString((SCREEN_WIDTH / 2) + SCREEN_LEFT(x), SCREEN_TOP(y), pRadioName);
				}
				else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

				}
				else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

				}
				CFont::DrawFonts();
			}
		}
	}
}

void CHudNew::PrintHelpText(float x, float y, char *str, float alpha) {
	if (CHud::m_nHelpMessageStatId) {
		if (CHud::m_nHelpMessageStatId >= 10) {
			if (CHud::m_nHelpMessageStatId >= 100)
				sprintf(gString, "STAT%d", CHud::m_nHelpMessageStatId);
			else
				sprintf(gString, "STAT0%d", CHud::m_nHelpMessageStatId);
		}
		else
			sprintf(gString, "STAT00%d", CHud::m_nHelpMessageStatId);

		CFont::PrintString(x, y, TheText.Get(gString));

		float fProgress = 0.0f;
		if (CHud::m_nHelpMessageStatId == 336)
			fProgress = plugin::CallMethodAndReturn<unsigned int, 0x5F6AA0>(0xC09928 + FindPlayerPed(-1)->m_pPlayerData->m_nPlayerGroup * 0x2D4);
		else
			fProgress = CStats::GetStatValue(CHud::m_nHelpMessageStatId);

		float posX(x + CFont::GetStringWidth(TheText.Get(gString), 1, 0)), posY(y);
		float offsetX(SCREEN_LEFT(38.0f)), offsetY(SCREEN_TOP(4.0f));
		float barW(SCREEN_LEFT(70.0f)), barH(SCREEN_TOP(14.0f));
		float wrapX(offsetX + barW + SCREEN_LEFT(8.0f));
		if (CHud::m_pHelpMessageToPrint[0] == '+') {
			CFont::SetBackground(false, false);
			CProgressBar::DrawProgressBarWithProgressDelta(posX + offsetX, posY + offsetY, (barW), (barH), fmaxf((1.0f / CHud::m_nHelpMessageMaxStatValue) * fProgress * 100.0f, 2.0f),
				HudColour.GetRGB(HUD_COLOUR_WHITE, alpha), fmaxf((1.0f / CHud::m_nHelpMessageMaxStatValue) * CHud::m_fHelpMessageStatUpdateValue * 100.0f, 3.0f), HudColour.GetRGB(HUD_COLOUR_GREEN, alpha));
			CFont::PrintString(posX + wrapX, posY, CHud::m_pHelpMessageToPrint);
		}
		else {
			CFont::SetBackground(false, false);
			CProgressBar::DrawProgressBarWithProgressDelta(posX + offsetX, posY + offsetY, (barW), (barH), fmaxf((1.0f / CHud::m_nHelpMessageMaxStatValue) * fProgress * 100.0f, 2.0f),
				HudColour.GetRGB(HUD_COLOUR_WHITE, alpha), fmaxf((1.0f / CHud::m_nHelpMessageMaxStatValue) * CHud::m_fHelpMessageStatUpdateValue * 100.0f, 3.0f), HudColour.GetRGB(HUD_COLOUR_RED, alpha));
			CFont::PrintString(posX + wrapX, posY, CHud::m_pHelpMessageToPrint);
		}
	}
	else {
		CFont::PrintString(x, y, str);
	}
}

void CHudNew::DrawHelpText(float x, float y, float w, float h) {
	ScreenAddons::SetScreenMult(s.m_fHudW, s.m_fHudH);

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

		float fAlphaText = s.HUD_COLOUR_HELP_TEXT.a;
		float fAlphaBox = s.HUD_COLOUR_HELP_BOX.a;

		if (CHud::m_nHelpMessageState) {
			switch (CHud::m_nHelpMessageState) {
			case 1:
				fAlphaText = s.HUD_COLOUR_HELP_TEXT.a;
				fAlphaBox = s.HUD_COLOUR_HELP_BOX.a;
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
				fAlphaText = CHud::m_nHelpMessageFadeTimer * 0.001f *  s.HUD_COLOUR_HELP_TEXT.a;
				fAlphaBox = CHud::m_nHelpMessageFadeTimer * 0.001f * s.HUD_COLOUR_HELP_BOX.a;
				break;
			case 3:
				CHud::m_nHelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02f * -1000.0f);
				if (CHud::m_nHelpMessageFadeTimer >= 0) {
					CHud::m_nHelpMessageFadeTimer = 0;
					CHud::m_nHelpMessageState = 0;
				}
				fAlphaText = CHud::m_nHelpMessageFadeTimer * 0.001f *  s.HUD_COLOUR_HELP_TEXT.a;
				fAlphaBox = CHud::m_nHelpMessageFadeTimer * 0.001f * s.HUD_COLOUR_HELP_BOX.a;
				break;
			case 4:
				CHud::m_nHelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02f * -1000.0f);
				if (CHud::m_nHelpMessageFadeTimer >= 0) {
					CHud::m_nHelpMessageFadeTimer = 0;
					CHud::m_nHelpMessageState = 2;
					CMessages::StringCopy(CHud::m_pHelpMessageToPrint, CHud::m_pLastHelpMessage, 400);
				}
				fAlphaText = CHud::m_nHelpMessageFadeTimer * 0.001f *  s.HUD_COLOUR_HELP_TEXT.a;
				fAlphaBox = CHud::m_nHelpMessageFadeTimer * 0.001f * s.HUD_COLOUR_HELP_BOX.a;
				break;
			default:
				break;
			}

			CHud::m_nHelpMessageTimer += (CTimer::ms_fTimeStep * 0.02f * 1000.0f);

			CFont::SetAlphaFade(fAlphaText);
			CFont::SetBackground(true, false);
			CFont::SetBackgroundColor(CRGBA(s.HUD_COLOUR_HELP_BOX.r, s.HUD_COLOUR_HELP_BOX.g, s.HUD_COLOUR_HELP_BOX.b, fAlphaBox));
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_LEFT);

			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				CFont::SetWrapx(SCREEN_LEFT(200.0f + 26.0f - 4.0f));
				CFont::SetFontStyle(III_FONT_SUBTITLES);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(0);
				CFont::SetDropColor(CRGBA(0, 0, 0, fAlphaText));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_HELP_TEXT.r, s.HUD_COLOUR_HELP_TEXT.g, s.HUD_COLOUR_HELP_TEXT.b, fAlphaText));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				PrintHelpText(SCREEN_LEFT(x), SCREEN_TOP(y), CHud::m_pHelpMessageToPrint, fAlphaText);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				CFont::SetWrapx(SCREEN_LEFT(200.0f + 26.0f - 4.0f));
				CFont::SetFontStyle(VC_FONT_SUBTITLES);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(0);
				CFont::SetDropColor(CRGBA(0, 0, 0, fAlphaText));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_HELP_TEXT.r, s.HUD_COLOUR_HELP_TEXT.g, s.HUD_COLOUR_HELP_TEXT.b, fAlphaText));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				PrintHelpText(SCREEN_LEFT(x), SCREEN_TOP(y), CHud::m_pHelpMessageToPrint, fAlphaText);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CFont::SetWrapx(SCREEN_LEFT(200.0f + 26.0f - 4.0f));
				CFont::SetFontStyle(SA_FONT_SUBTITLES);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(0);
				CFont::SetDropColor(CRGBA(0, 0, 0, fAlphaText));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_HELP_TEXT.r, s.HUD_COLOUR_HELP_TEXT.g, s.HUD_COLOUR_HELP_TEXT.b, fAlphaText));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				PrintHelpText(SCREEN_LEFT(x), SCREEN_TOP(y), CHud::m_pHelpMessageToPrint, fAlphaText);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				CFont::SetWrapx(SCREEN_LEFT(300.0f + 26.0f - 4.0f));
				CFont::SetFontStyle(LCS_FONT_SUBTITLES);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(0);
				CFont::SetDropColor(CRGBA(0, 0, 0, fAlphaText));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_HELP_TEXT.r, s.HUD_COLOUR_HELP_TEXT.g, s.HUD_COLOUR_HELP_TEXT.b, fAlphaText));
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				PrintHelpText(SCREEN_LEFT(x), SCREEN_TOP(y), CHud::m_pHelpMessageToPrint, fAlphaText);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

			}
			CFont::DrawFonts();
		}
	}
	else {
		CHud::m_nHelpMessageState = 0;
	}
}

void CHudNew::DrawSubtitles(float x, float y, float w, float h) {
	ScreenAddons::SetScreenMult(s.m_fHudW * s.m_fSubsW, s.m_fHudH * s.m_fSubsH);

	if (CHud::m_Message[0] && FrontEndMenuManager.m_bShowSubtitles) {
		if (ClassicHud::GetGameMode() == GAMEMODE_III) {
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetBackground(false, false);
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::SetFontStyle(III_FONT_SUBTITLES);

			if (TheCamera.m_bWideScreenOn)
				CFont::SetCentreSize(SCREEN_RIGHT(120.0f));
			else
				CFont::SetCentreSize(SCREEN_RIGHT(280.0f));

			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_SUBTITLES.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_SUBTITLES));
			CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_BOTTOM(y), CHud::m_Message);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetBackground(false, false);
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::SetFontStyle(VC_FONT_SUBTITLES);

			if (TheCamera.m_bWideScreenOn)
				CFont::SetCentreSize(SCREEN_RIGHT(120.0f));
			else
				CFont::SetCentreSize(SCREEN_RIGHT(280.0f));

			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_SUBTITLES.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_SUBTITLES));
			CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_BOTTOM(y), CHud::m_Message);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetBackground(false, false);
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::SetFontStyle(SA_FONT_SUBTITLES);

			if (TheCamera.m_bWideScreenOn)
				CFont::SetCentreSize(SCREEN_RIGHT(120.0f));
			else
				CFont::SetCentreSize(SCREEN_RIGHT(280.0f));

			CFont::SetDropShadowPosition(SUBS_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_SUBTITLES.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_SUBTITLES));

			if (TheCamera.m_bWideScreenOn) {
				x = 0.0f;
				y -= 25.0f;
			}

			CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_BOTTOM(y), CHud::m_Message);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
			CFont::SetProportional(true);
			CFont::SetOrientation(ALIGN_CENTER);
			CFont::SetBackground(false, false);
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::SetFontStyle(LCS_FONT_SUBTITLES);

			float fPositionX = x;
			float fPositionY = y;

			if (TheCamera.m_bWideScreenOn) {
				fPositionX = SCREEN_WIDTH * 0.50f;
				fPositionY += 18.0f;
				CFont::SetCentreSize(SCREEN_RIGHT(280.0f));
			}
			else {
				if (CFont::GetStringWidth(CHud::m_Message, true, false) > (SCREEN_WIDTH * 0.45f) * ScreenAddons::GetAspectRatio())
					fPositionX = SCREEN_WIDTH * 0.55f;
				else
					fPositionX = SCREEN_WIDTH * 0.50f;

				fPositionY = y;

				CFont::SetCentreSize(SCREEN_RIGHT(260.0f));
			}

			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, s.HUD_COLOUR_SUBTITLES.a));
			CFont::SetColor(CRGBA(s.HUD_COLOUR_SUBTITLES));
			CFont::PrintString(fPositionX, SCREEN_BOTTOM(fPositionY), CHud::m_Message);
		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

		}
		else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

		}
	}

	ScreenAddons::SetScreenMult(DEFAULT_HUD_SCALE);
}

void CHudNew::DrawWastedBustedMessage(float x, float y, float w, float h) {
	if (CHud::m_BigMessage[2][0]) {
		if (BigMessageX[2] == 0.0f) {
			BigMessageX[2] = 1.0f;
			BigMessageAlpha[2] = 0.0f;

			if (CHud::m_VehicleState)
				CHud::m_VehicleState = 0;
			if (CHud::m_ZoneState)
				CHud::m_ZoneState = 0;
		}
		else {
			BigMessageAlpha[2] += (CTimer::ms_fTimeStep * 0.02f * 255.0f);
			if (BigMessageAlpha[2] > 255.0f)
				BigMessageAlpha[2] = 255.0;

			if (CWorld::Players[CWorld::PlayerInFocus].m_nPlayerState == 2)
				CFont::SetColor(CRGBA(s.HUD_COLOUR_WASTED.r, s.HUD_COLOUR_WASTED.g, s.HUD_COLOUR_WASTED.b, BigMessageAlpha[2]));
			else
				CFont::SetColor(CRGBA(s.HUD_COLOUR_BUSTED.r, s.HUD_COLOUR_BUSTED.g, s.HUD_COLOUR_BUSTED.b, BigMessageAlpha[2]));

			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(III_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[2]));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_BigMessage[2]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(VC_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[2]));
				CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_BigMessage[2]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(SA_FONT_GOTHIC);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(EDGE_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[2]));
				CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_MIDDLE_Y(-y), CHud::m_BigMessage[2]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(LCS_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[2]));
				CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_MIDDLE_Y(-y), CHud::m_BigMessage[2]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}
	}
	else {
		BigMessageX[2] = 0.0f;
	}
}

void CHudNew::DrawMissionTitle(float x, float y, float w, float h) {
	if (CHud::m_BigMessage[1][0]) {
		if (BigMessageInUse[1] == 0.0f) {
			BigMessageInUse[1] = -60.0f;
			BigMessageX[1] = 1.0f;
			BigMessageAlpha[1] = 0.0f;
		}
		else {
			if (BigMessageX[1] >= (SCREEN_WIDTH - 20)) {
				BigMessageInUse[1] += (CTimer::ms_fTimeStep * 0.02f * 120.0f);

				if (BigMessageInUse[1] >= 120.0f) {
					BigMessageInUse[1] = 120.0;
					BigMessageAlpha[1] += (CTimer::ms_fTimeStep * 0.02f * -255.0f);
				}

				if (BigMessageAlpha[1] <= 0.0f) {
					CHud::m_BigMessage[1][0] = 0;
					BigMessageAlpha[1] = 0.0;
				}
			}
			else {
				BigMessageX[1] += (CTimer::ms_fTimeStep * 0.02f * 255.0f);
				BigMessageAlpha[1] += (CTimer::ms_fTimeStep * 0.02f * 255.0f);

				if (BigMessageAlpha[1] >= 255.0f)
					BigMessageAlpha[1] = 255.0f;
			}

			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(III_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[1]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_TITLE.r, s.HUD_COLOUR_MISSION_TITLE.g, s.HUD_COLOUR_MISSION_TITLE.b, BigMessageAlpha[1]));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_BigMessage[1]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(VC_FONT_RAGE);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[1]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_TITLE.r, s.HUD_COLOUR_MISSION_TITLE.g, s.HUD_COLOUR_MISSION_TITLE.b, BigMessageAlpha[1]));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_BigMessage[1]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(SA_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(EDGE_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[1]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_TITLE.r, s.HUD_COLOUR_MISSION_TITLE.g, s.HUD_COLOUR_MISSION_TITLE.b, BigMessageAlpha[1]));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_BigMessage[1]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_RIGHT);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(LCS_FONT_BANK);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[1]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_TITLE.r, s.HUD_COLOUR_MISSION_TITLE.g, s.HUD_COLOUR_MISSION_TITLE.b, BigMessageAlpha[1]));
				CFont::PrintStringFromBottom(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_BigMessage[1]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}
	}
	else {
		BigMessageX[1] = 0.0f;
	}
}

void CHudNew::DrawSuccessFailedMessage(float x, float y, float w, float h) {
	if (CHud::m_BigMessage[0][0]) {
		if (BigMessageInUse[0] == 0.0f) {
			BigMessageInUse[0] = -60.0f;
			BigMessageX[0] = 1.0f;
			BigMessageAlpha[0] = 0.0f;
		}
		else {
			if (BigMessageX[0] >= (SCREEN_WIDTH - 20)) {
				BigMessageInUse[0] += (CTimer::ms_fTimeStep * 0.02f * 120.0f);

				if (BigMessageInUse[0] >= 120.0f) {
					BigMessageInUse[0] = 120.0;
					BigMessageAlpha[0] += (CTimer::ms_fTimeStep * 0.02f * -255.0f);
				}

				if (BigMessageAlpha[0] <= 0.0f) {
					CHud::m_BigMessage[0][0] = 0;
					BigMessageAlpha[0] = 0.0;
				}
			}
			else {
				BigMessageX[0] += (CTimer::ms_fTimeStep * 0.02f * 255.0f);
				BigMessageAlpha[0] += (CTimer::ms_fTimeStep * 0.02f * 255.0f);

				if (BigMessageAlpha[0] >= 255.0f)
					BigMessageAlpha[0] = 255.0f;
			}
			if (ClassicHud::GetGameMode() == GAMEMODE_III) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(III_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[0]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_RESULT.r, s.HUD_COLOUR_MISSION_RESULT.g, s.HUD_COLOUR_MISSION_RESULT.b, BigMessageAlpha[0]));
				CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_MIDDLE_Y(-y), CHud::m_BigMessage[0]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VC) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(VC_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[0]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_RESULT.r, s.HUD_COLOUR_MISSION_RESULT.g, s.HUD_COLOUR_MISSION_RESULT.b, BigMessageAlpha[0]));
				CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_MIDDLE_Y(-y), CHud::m_BigMessage[0]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_SA) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(SA_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(0);
				CFont::SetEdge(EDGE_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[0]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_RESULT.r, s.HUD_COLOUR_MISSION_RESULT.g, s.HUD_COLOUR_MISSION_RESULT.b, BigMessageAlpha[0]));
				CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_MIDDLE_Y(-y), CHud::m_BigMessage[0]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_LCS) {
				CFont::SetProportional(true);
				CFont::SetBackground(false, false);
				CFont::SetOrientation(ALIGN_CENTER);
				CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
				CFont::SetFontStyle(LCS_FONT_PRICEDOWN);
				CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
				CFont::SetDropColor(CRGBA(0, 0, 0, BigMessageAlpha[0]));
				CFont::SetColor(CRGBA(s.HUD_COLOUR_MISSION_RESULT.r, s.HUD_COLOUR_MISSION_RESULT.g, s.HUD_COLOUR_MISSION_RESULT.b, BigMessageAlpha[0]));
				CFont::PrintString(SCREEN_MIDDLE_X(x), SCREEN_MIDDLE_Y(-y), CHud::m_BigMessage[0]);
			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (ClassicHud::GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}
	}
	else {
		BigMessageX[0] = 0.0f;
	}
}

void CHudNew::DrawOddJobMessage(float x, float y, float w, float h) {

}
