#include "plugin.h"
#include "CClock.h"
#include "CHud.h"
#include "CFont.h"
#include "CTxdStore.h"
#include "CWorld.h"
#include "eModelID.h"
#include "ScreenAddition.h"
#include "CHudNew.h"
#include "eWeaponType.h"
#include "CDarkel.h"
#include "CFontNew.h"
#include "CMenuManager.h"
#include "CTheScripts.h"
#include "CCamera.h"
#include "Settings.h"
#include "CTimer.h"
#include "CCutsceneMgr.h"
#include "CEntryExitManager.h"

using namespace plugin;

eGameMode CHudNew::ms_nGameMode;
CSprite2d CHudNew::SpritesMod[HUD_TOTALWEAPONS];
bool CHudNew::ms_bSpritesLoaded;
char *CHudNew::ms_nGamePrefix;
bool CHudNew::ms_bReload;

char *SpritesModNames[HUD_TOTALWEAPONS] = {
	"unarmed",
	"dildo1", // 321
	"dildo2",
	"vibe1",
	"vibe2",
	"flowers",
	"cane",
	"boxwee",
	"boxbig",
	BLANK,
	"cellphone",
	"brassknuckle",
	BLANK,
	"golfclub",
	"nightstick",
	"knife",
	"baseballbat",
	"shovel",
	"poolcue",
	"katana",
	BLANK,
	"chainsaw",
	"grenade",
	"teargas",
	"molotov",
	"missile",
	"pistol",
	"pistol_silenced",
	"desert_eagle",
	"shotgun",
	"sawnoff",
	"spas12",
	"micro_uzi",
	"mp5",
	"flare",
	"ak47",
	"m4",
	"countryrifle",
	"sniperrifle",
	"rlauncher",
	"heatseek",
	"flamethrower",
	"minigun",
	"satchel_charge",
	"detonator",
	"spraycan",
	"extinguisher",
	"camera",
	"nightvision",
	"infrared",
	"jetpack",
	"parachute",
	"tec9",
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
		int Slot = CTxdStore::AddTxdSlot("weapons");
		char FileName[128];
		strcpy_s(FileName, "classichud\\weapons\\"); // Set folder,
		strcat_s(FileName, GetGamePrefix()); // game prefix,
		strcat_s(FileName, "_");
		strcat_s(FileName, "weapons.txd"); // file name.
		puts(FileName);
		CTxdStore::LoadTxd(Slot, PLUGIN_PATH(FileName));
		CTxdStore::AddRef(Slot);
		CTxdStore::PopCurrentTxd();
		CTxdStore::SetCurrentTxd(Slot);

		for (int i = 0; i < HUD_TOTALWEAPONS; i++)
			SpritesMod[i].SetTexture(SpritesModNames[i]);

		CTxdStore::PopCurrentTxd();

		ms_bSpritesLoaded = true;

		s.readDat();
	}
}

void CHudNew::Shutdown() {
	if (ms_bSpritesLoaded) {
		for (int i = 0; i < HUD_TOTALWEAPONS; ++i)
			SpritesMod[i].Delete();

		int Slot = CTxdStore::FindTxdSlot("weapons");
		CTxdStore::RemoveTxdSlot(Slot);

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
		ms_bReload = true;
	}
}

void CHudNew::Draw() {
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
					PrintClock(s.m_fClock.left, s.m_fClock.top, s.m_fClock.right, s.m_fClock.bottom);

				PrintMoneyCounter(s.m_fMoney.left, s.m_fMoney.top, s.m_fMoney.right, s.m_fMoney.bottom);
				PrintHealth(0, s.m_fHealth.left, s.m_fHealth.top, s.m_fHealth.right, s.m_fHealth.bottom);
				PrintArmour(0, s.m_fArmour.left, s.m_fArmour.top, s.m_fArmour.right, s.m_fArmour.bottom);
				PrintWeaponIcon(0, s.m_fWeapon.left, s.m_fWeapon.top, s.m_fWeapon.right, s.m_fWeapon.bottom);
				PrintAmmo(0, s.m_fAmmo.left, s.m_fAmmo.top, s.m_fAmmo.right, s.m_fAmmo.bottom);
				DrawWanted(s.m_fWanted.left, s.m_fWanted.top, s.m_fWanted.right, s.m_fWanted.bottom);

				ms_bReload = false;
			}

			if (!CHud::bScriptDontDisplayVehicleName)
				DrawVehicleName(s.m_fVehicleName.left, s.m_fVehicleName.top, s.m_fVehicleName.right, s.m_fVehicleName.bottom);
		}
	}
}

void CHudNew::DrawAfterFade() {
	RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERNEAREST);
	RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
	RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, 0);
	if (!CTimer::m_UserPause && /*CReplay::Mode != 1 &&*/ !CWeapon::ms_bTakePhoto)
	{
		if ((!FindPlayerVehicle(-1, 0)
			|| FindPlayerVehicle(-1, 0)->m_nVehicleSubClass != 4
			&& FindPlayerVehicle(-1, 0)->m_nVehicleSubClass != 3)
			&& !CCutsceneMgr::ms_cutsceneProcessing
			&& !FrontEndMenuManager.m_bMenuActive
			&& !TheCamera.m_bWideScreenOn)
		{
			if (!CHud::bScriptDontDisplayAreaName)
				DrawAreaName(s.m_fZoneName.left, s.m_fZoneName.top, s.m_fZoneName.right, s.m_fZoneName.bottom);
		}
		if (!CHud::m_BigMessage[0][0])
			CHud::DrawScriptText(0);
		if (!CTheScripts::bDrawSubtitlesBeforeFade)
			CHud::DrawSubtitles();
		CHud::DrawMissionTitle();
		CHud::DrawOddJobMessage(0);
	}
}

void CHudNew::PrintClock(float x, float y, float w, float h) {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "%02d:%02d", CClock::ms_nGameClockHours, CClock::ms_nGameClockMinutes);

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(194, 165, 120, 255));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
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

void CHudNew::PrintMoneyCounter(float x, float y, float w, float h) {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "$%08d", CWorld::Players[CWorld::PlayerInFocus].m_nDisplayMoney);

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(89, 115, 150, 255));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
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

void CHudNew::PrintHealth(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		strcpy_s(IconToPrint, "{");
		sprintf_s(TextToPrint, "%03d", static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_fHealth));

		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(186, 101, 50, 255));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
		CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
		CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
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

void CHudNew::PrintArmour(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_fArmour > 1.0f) {
			strcpy_s(IconToPrint, "[");
			sprintf_s(TextToPrint, "%03d", static_cast<int>(CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_fArmour));

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(124, 140, 95, 255));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
			CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
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

void CHudNew::PrintBreath(int PlayerID, float x, float y, float w, float h) {
	char TextToPrint[16];
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_pPlayerData->m_fBreath > 1.0f) {
			strcpy_s(IconToPrint, "[");
			sprintf_s(TextToPrint, "%03d", static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_pPlayerData->m_fBreath));

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetRightJustifyWrap(0.0f);
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(124, 140, 95, 255));
			CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
			CFont::PrintString(SCREEN_RIGHT(x + 54.0f), SCREEN_TOP(y), IconToPrint);
		}
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

void CHudNew::PrintWeaponIcon(int PlayerID, float x, float y, float w, float h) {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
	int ModelId = CWeaponInfo::GetWeaponInfo(CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_aWeapons[CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1;
	int SpriteId = ModelId - 320;

	if (SpriteId <= 0)
		SpriteId = 0;
	else if (SpriteId >= HUD_TOTALWEAPONS - 1)
		SpriteId = HUD_TOTALWEAPONS - 1;

	if (GetGameMode() == GAMEMODE_III) {
		SpritesMod[SpriteId].Draw(CRect(SCREEN_RIGHT(x), SCREEN_TOP(y), SCREEN_RIGHT(w), SCREEN_TOP(h)), CRGBA(255, 255, 255, 255));
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

void CHudNew::PrintAmmo(int PlayerID, float x, float y, float w, float h) {
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

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_CENTER);
		CFont::SetCentreSize(640.0f);
		CFont::SetFontStyle(FONT_GOTHIC);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(0);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(0, 0, 0, 255));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		if (!CDarkel::FrenzyOnGoing() && WeaponType &&
			WeaponType != WEAPON_DETONATOR &&
			(WeaponType < WEAPON_DILDO1 || WeaponType >= WEAPON_GRENADE) &&
			WeaponType != WEAPON_PARACHUTE &&
			CWeaponInfo::GetWeaponInfo(WeaponType, 1)->m_nWeaponFire != WEAPON_FIRE_USE &&
			CWeaponInfo::GetWeaponInfo(WeaponType, 1)->m_nSlot > 1)
			CFont::PrintString(SCREEN_RIGHT(x), SCREEN_TOP(y), TextToPrint);
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

void CHudNew::DrawWanted(float x, float y, float w, float h) {
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		strcpy_s(IconToPrint, "]");
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0f);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(0);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(0, 0, 0, 255));
		CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));

		for (int i = 0; i < 6; i++) {
			CFont::SetColor(CRGBA(0, 0, 0, 255));
			CFont::PrintString(SCREEN_RIGHT(x - 2.0f + 24.0f * i), SCREEN_TOP(y + 2.0f), IconToPrint);
			if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nWantedLevel > i
				&& (CTimer::m_snTimeInMilliseconds > CWorld::Players[CWorld::PlayerInFocus].m_pPed->GetWanted()->m_nLastTimeWantedLevelChanged
					+ 2000 || CTimer::m_FrameCounter & 8)) {

				CFont::SetColor(CRGBA(193, 164, 120, 255));
				CFont::PrintString(SCREEN_RIGHT(x + 24.0f * i), SCREEN_TOP(y), IconToPrint);
			}
		}
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

void CHudNew::PrintStatsBox(float x, float y, float w, float h) {
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
						CHud::m_VehicleState == 3;
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
				if (CHud::m_ZoneNameTimer > 3000) {
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
					CFont::SetFontStyle(FONT_GOTHIC);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fZoneAlpha));
					CFont::SetColor(CRGBA(152, 154, 82, fZoneAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_ZoneToPrint);
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
		if (CHud::m_pVehicleName != CHud::m_pLastVehicleName) {
			switch (CHud::m_VehicleState) {
			case 0:
				if (!CTheScripts::bPlayerIsOffTheMap && CTheScripts::bDisplayHud ||
					CEntryExitManager::ms_exitEnterState == 1 ||
					CEntryExitManager::ms_exitEnterState == 2) {
					State = 2;
					CHud::m_VehicleState = 2;
					CHud::m_VehicleNameTimer = 0;
					CHud::m_VehicleFadeTimer = 0;
					CHud::m_pVehicleNameToPrint = CHud::m_pVehicleName;

					if (CHud::m_VehicleState == 1 || CHud::m_VehicleState == 2)
						CHud::m_VehicleState == 3;
				}
				break;
			case 1:
			case 2:
			case 3:
				State = 4;
				CHud::m_VehicleState = 4;
			case 4:
				CHud::m_VehicleNameTimer = 0;
				break;
			default:
				break;
			}
			CHud::m_pLastVehicleName = CHud::m_pVehicleName;
		}

		fVehicleAlpha = 255.0f;

		if (State) {
			switch (State) {
			case 1:
				if (CHud::m_VehicleNameTimer > 3000) {
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
					CHud::m_pVehicleNameToPrint = CHud::m_pVehicleName;
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
					CFont::SetFontStyle(FONT_GOTHIC);
					CFont::SetEdge(0);
					CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
					CFont::SetDropColor(CRGBA(0, 0, 0, fVehicleAlpha));
					CFont::SetColor(CRGBA(194, 165, 120, fVehicleAlpha));
					CFont::SetScale(SCREEN_LEFT(w), SCREEN_TOP(h));
					CFont::PrintString(SCREEN_RIGHT(x), SCREEN_BOTTOM(y), CHud::m_pVehicleNameToPrint);
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
	}
	else {
		CHud::m_pLastVehicleName = NULL;
		CHud::m_VehicleState = 0;
		CHud::m_VehicleFadeTimer = 0;
		CHud::m_VehicleNameTimer = 0;
	}
}

void CHudNew::DrawProgressBar(float x, float y, float w, float h, float percentage) {

}

void CHudNew::DrawProgressString(CSprite2d sprite, float, float y, float w, float h) {

}

void CHudNew::InjectPatches() {
	Events::initRwEvent += CHudNew::Initialise;
	Events::drawMenuBackgroundEvent += CHudNew::ReInitialise;
	Events::shutdownRwEvent += CHudNew::Shutdown;
	Events::drawHudEvent += CHudNew::Draw;
	Events::drawHudEvent += CHudNew::DrawAfterFade;

#if GTASA
	patch::Set(0x58FBD6, 5); // CHud::DrawPlayerInfo
	patch::Set(0x58D542, 5); // CHud::DrawAreaName
	patch::Set(0x58FBDB, 5); // CHud::DrawWanted
	patch::Set(0x58FBE9, 5); // CHud::DrawVehicleName
	patch::Set(0x53E550, 5); // CHud::DrawAfterFade
	patch::Set(0x53E83C, 5); // CHud::DrawAfterFade
	patch::Set(0x53EBA2, 5); // CHud::DrawAfterFade
#endif
}