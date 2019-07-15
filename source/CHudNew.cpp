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

using namespace plugin;

eGameMode CHudNew::ms_nGameMode;
CSprite2d CHudNew::SpritesMod[HUD_TOTALWEAPONS];
bool CHudNew::ms_bSpritesLoaded;
char *CHudNew::ms_nGamePrefix;

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
	SetGameMode(GAMEMODE_III);

	switch (CHudNew::GetGameMode()) {
	default:
	case GAMEMODE_III:
		ms_nGamePrefix = "iii";
		break;
	case GAMEMODE_VC:
		ms_nGamePrefix = "vc";
		break;
	case GAMEMODE_SA:
		ms_nGamePrefix = "sa";
		break;
	case GAMEMODE_LCS:
		ms_nGamePrefix = "lcs";
		break;
	case GAMEMODE_VCS:
		ms_nGamePrefix = "vcs";
		break;
	case GAMEMODE_ADVANCE:
		ms_nGamePrefix = "advance";
		break;
	};

	if (!ms_bSpritesLoaded) {
		int Slot = CTxdStore::AddTxdSlot("wep_icons");
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
	}
}

void CHudNew::Shutdown() {
	if (ms_bSpritesLoaded) {
		for (int i = 0; i < HUD_TOTALWEAPONS; ++i)
			SpritesMod[i].Delete();

		int Slot = CTxdStore::FindTxdSlot("wep_icons");
		CTxdStore::RemoveTxdSlot(Slot);

		ms_bSpritesLoaded = false;
	}
}

void CHudNew::DrawPlayerInfo() {
	if (!TheCamera.m_bWideScreenOn)
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
				PrintClock();

			PrintMoneyCounter();
			PrintHealth(0);
			PrintArmour(0);
			PrintWeaponIcon(0);
			PrintAmmo(0);
		}
	}
}

void CHudNew::PrintClock() {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "%02d:%02d", CClock::ms_nGameClockHours, CClock::ms_nGameClockMinutes);

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(194, 165, 120, 255));
		CFont::SetScale(SCREEN_LEFT(0.8f), SCREEN_TOP(1.35f));
		CFont::PrintString(SCREEN_RIGHT(111.0f), SCREEN_TOP(22.0f), TextToPrint);
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

void CHudNew::PrintMoneyCounter() {
	char TextToPrint[16];
	sprintf_s(TextToPrint, "$%08d", CWorld::Players[CWorld::PlayerInFocus].m_nDisplayMoney);

	if (GetGameMode() == GAMEMODE_III) {
		CFont::SetProportional(true);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(89, 115, 150, 255));
		CFont::SetScale(SCREEN_LEFT(0.8f), SCREEN_TOP(1.35f));
		CFont::PrintString(SCREEN_RIGHT(110.0f), SCREEN_TOP(43.0f), TextToPrint);
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

void CHudNew::PrintHealth(int PlayerID) {
	char TextToPrint[16];
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		strcpy_s(IconToPrint, "{");
		sprintf_s(TextToPrint, "%03d", static_cast<int>(CWorld::Players[PlayerID].m_pPed->m_fHealth));

		CFont::SetProportional(false);
		CFont::SetBackground(false, false);
		CFont::SetOrientation(ALIGN_RIGHT);
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetEdge(0);
		CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
		CFont::SetDropColor(CRGBA(0, 0, 0, 255));
		CFont::SetColor(CRGBA(186, 101, 50, 255));
		CFont::SetScale(SCREEN_LEFT(0.8f), SCREEN_TOP(1.35f));
		CFont::PrintString(SCREEN_RIGHT(110.0f), SCREEN_TOP(65.0f), TextToPrint);
		CFont::PrintString(SCREEN_RIGHT(164.0f), SCREEN_TOP(65.0f), IconToPrint);
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

void CHudNew::PrintArmour(int PlayerID) {
	char TextToPrint[16];
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_fArmour > 1.0f) {
			strcpy_s(IconToPrint, "[");
			sprintf_s(TextToPrint, "%03d", static_cast<int>(CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_fArmour));

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(124, 140, 95, 255));
			CFont::SetScale(SCREEN_LEFT(0.8f), SCREEN_TOP(1.35f));
			CFont::PrintString(SCREEN_RIGHT(182.0f), SCREEN_TOP(65.0f), TextToPrint);
			CFont::PrintString(SCREEN_RIGHT(234.0f), SCREEN_TOP(65.0f), IconToPrint);
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

void CHudNew::PrintBreath(int PlayerID) {
	char TextToPrint[16];
	char IconToPrint[16];

	if (GetGameMode() == GAMEMODE_III) {
		if (CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_pPlayerData->m_fBreath > 1.0f) {
			strcpy_s(IconToPrint, "[");
			sprintf_s(TextToPrint, "%03d", static_cast<int>(CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_pPlayerData->m_fBreath));

			CFont::SetProportional(false);
			CFont::SetBackground(false, false);
			CFont::SetOrientation(ALIGN_RIGHT);
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetEdge(0);
			CFont::SetDropShadowPosition(HUD_SHADOW_SIZE);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetColor(CRGBA(124, 140, 95, 255));
			CFont::SetScale(SCREEN_LEFT(0.8f), SCREEN_TOP(1.35f));
			CFont::PrintString(SCREEN_RIGHT(254.0f), SCREEN_TOP(65.0f), TextToPrint);
			CFont::PrintString(SCREEN_RIGHT(304.0f), SCREEN_TOP(65.0f), IconToPrint);
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

void CHudNew::PrintWeaponIcon(int PlayerID) {
	RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, reinterpret_cast<void *>(rwFILTERLINEAR));
	int ModelId = CWeaponInfo::GetWeaponInfo(CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_aWeapons[CWorld::Players[CWorld::PlayerInFocus].m_pPed->m_nActiveWeaponSlot].m_nType, 1)->m_nModelId1;
	int SpriteId = ModelId - 320;

	if (SpriteId <= 0)
		SpriteId = 0;
	else if (SpriteId >= HUD_TOTALWEAPONS - 1)
		SpriteId = HUD_TOTALWEAPONS - 1;

	if (GetGameMode() == GAMEMODE_III) {
		SpritesMod[SpriteId].Draw(CRect(SCREEN_RIGHT(99.0f), SCREEN_TOP(27.0f), SCREEN_RIGHT(35.0f), SCREEN_TOP(91.0f)), CRGBA(255, 255, 255, 255));
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

void CHudNew::PrintAmmo(int PlayerID) {
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
		CFont::SetScale(SCREEN_LEFT(0.4f), SCREEN_TOP(0.6f));

		if (!CDarkel::FrenzyOnGoing() && WeaponType &&
			WeaponType != WEAPON_DETONATOR &&
			(WeaponType < WEAPON_DILDO1 || WeaponType >= WEAPON_GRENADE) &&
			WeaponType != WEAPON_PARACHUTE &&
			CWeaponInfo::GetWeaponInfo(WeaponType, 1)->m_nWeaponFire != WEAPON_FIRE_USE &&
			CWeaponInfo::GetWeaponInfo(WeaponType, 1)->m_nSlot > 1)
			CFont::PrintString(SCREEN_RIGHT(66.0f), SCREEN_TOP(73.0f), TextToPrint);
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

void CHudNew::PrintStatsBox() {
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

void CHudNew::DrawProgressBar(float x, float y, float w, float h, float percentage) {

}

void CHudNew::DrawProgressString(CSprite2d sprite, float, float y, float w, float h) {

}

void CHudNew::InjectPatches() {
	Events::initRwEvent += CHudNew::Initialise;
	Events::shutdownRwEvent += CHudNew::Shutdown;
	Events::drawHudEvent += CHudNew::DrawPlayerInfo;

#if GTASA
	patch::Set(0x58FBD6, 5); // CHud::DrawPlayerInfo
#endif
}