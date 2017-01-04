#include "Main.hpp"

#define	ENERGY_LOSS_FLASH_DURATION 1000

using namespace plugin;

short& m_ItemToFlash = *(short*)0xBAB1DC;

CSprite2d hudIcons[10];
CSprite2d sprite;
CSprite2d weaponicons[50];
CSprite2d window;

CRGBA AreaCRGBA;

unsigned int m_CurrentStar;

class CHudNew {
public:
	static void ClassicHudTextures() {
		int v0;

		v0 = CTxdStore::AddTxdSlot("classichud");
		CTxdStore::LoadTxd(v0, ".\\CLASSICHUD\\TXD\\CLASSICHUD.TXD");
		CTxdStore::AddRef(v0);
		CTxdStore::PushCurrentTxd();
		CTxdStore::SetCurrentTxd(v0);
		hudIcons[0].SetTexture("health");
		hudIcons[1].SetTexture("armour");
		hudIcons[2].SetTexture("breath");
		hudIcons[3].SetTexture("star");
		hudIcons[4].SetTexture("arrow");
		hudIcons[5].SetTexture("bar_squareOutline");
		hudIcons[6].SetTexture("bar_roundOutline");
		hudIcons[7].SetTexture("star_Outline");
		CTxdStore::PopCurrentTxd();
	}

	static void WeaponTextures() {
		int v1;

		v1 = CTxdStore::AddTxdSlot("weapons");
		if (Style == 0) {
			CTxdStore::LoadTxd(v1, ".\\CLASSICHUD\\TXD\\WEAPONS\\WEAPONS_III.TXD");
		}
		if (Style == 1) {
			CTxdStore::LoadTxd(v1, ".\\CLASSICHUD\\TXD\\WEAPONS\\WEAPONS_VC.TXD");
		}
		if (Style == 2) {
			CTxdStore::LoadTxd(v1, ".\\CLASSICHUD\\TXD\\WEAPONS\\WEAPONS_STORIES.TXD");
		}
		if (Style == 3) {
			CTxdStore::LoadTxd(v1, ".\\CLASSICHUD\\TXD\\WEAPONS\\WEAPONS_STORIES.TXD");
		}
		if (Style == 4) {
			CTxdStore::LoadTxd(v1, ".\\CLASSICHUD\\TXD\\WEAPONS\\WEAPONS_CUSTOM.TXD");
		}
		CTxdStore::AddRef(v1);
		CTxdStore::PushCurrentTxd();
		CTxdStore::SetCurrentTxd(v1);
		weaponicons[0].SetTexture("UNARMED");
		weaponicons[1].SetTexture("GUN_DILDO1");
		weaponicons[2].SetTexture("GUN_DILDO2");
		weaponicons[3].SetTexture("GUN_VIBE1");
		weaponicons[4].SetTexture("GUN_VIBE2");
		weaponicons[5].SetTexture("FLOWERA");
		weaponicons[6].SetTexture("GUN_CANE");
		weaponicons[7].SetTexture("GUN_BOXWEE");
		weaponicons[8].SetTexture("GUN_BOXBIG");
		weaponicons[9].SetTexture("CELLPHONE");
		weaponicons[10].SetTexture("BRASSKNUCKLE");
		weaponicons[11].SetTexture("GOLFCLUB");
		weaponicons[12].SetTexture("NITESTICK");
		weaponicons[13].SetTexture("KNIFECUR");
		weaponicons[14].SetTexture("BAT");
		weaponicons[15].SetTexture("SHOVEL");
		weaponicons[16].SetTexture("POOLCUE");
		weaponicons[17].SetTexture("KATANA");
		weaponicons[18].SetTexture("CHNSAW");
		weaponicons[19].SetTexture("GRENADE");
		weaponicons[20].SetTexture("TEARGAS");
		weaponicons[21].SetTexture("MOLOTOV");
		weaponicons[22].SetTexture("MISSILE");
		weaponicons[23].SetTexture("COLT45");
		weaponicons[24].SetTexture("SILENCED");
		weaponicons[25].SetTexture("DESERT_EAGLE");
		weaponicons[26].SetTexture("CHROMEGUN");
		weaponicons[27].SetTexture("SAWNOFF");
		weaponicons[28].SetTexture("SHOTGSPA");
		weaponicons[29].SetTexture("MICRO_UZI");
		weaponicons[30].SetTexture("MP5LNG");
		weaponicons[31].SetTexture("FLARE");
		weaponicons[32].SetTexture("AK47");
		weaponicons[33].SetTexture("M4");
		weaponicons[34].SetTexture("CUNTGUN");
		weaponicons[35].SetTexture("SNIPER");
		weaponicons[36].SetTexture("ROCKETLA");
		weaponicons[37].SetTexture("HEATSEEK");
		weaponicons[38].SetTexture("FLAME");
		weaponicons[39].SetTexture("MINIGUN");
		weaponicons[40].SetTexture("SATCHEL");
		weaponicons[41].SetTexture("DETONATOR");
		weaponicons[42].SetTexture("SPRAYCAN");
		weaponicons[43].SetTexture("FIRE_EX");
		weaponicons[44].SetTexture("CAMERA");
		weaponicons[45].SetTexture("GOGGLES");
		weaponicons[46].SetTexture("GOGGLES");
		weaponicons[47].SetTexture("JETPACK");
		weaponicons[48].SetTexture("GUN_PARA");
		weaponicons[49].SetTexture("TEC9");
		CTxdStore::PopCurrentTxd();
	}

	static void PrintIcon() {
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
		sprite.Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_WEAPON_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_WEAPON_POS_Y), SCREEN_MULTIPLIER(HUD_WEAPON_SIZE_X), SCREEN_MULTIPLIER(HUD_WEAPON_SIZE_Y), CRGBA(255, 255, 255, HUD_COLOUR_WEAPON_A));
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	}

	static void DrawWeaponIcon(CPed *player) {
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void *)rwFILTERLINEAR);
		int modelId = CWeaponInfo::GetWeaponInfo(player->m_aWeapons[player->m_nActiveWeaponSlot].m_Type, 1)->m_dwModelId1;
		if (modelId <= 0) {
			sprite = weaponicons[0];
			PrintIcon();
			stat3_2 = CStats::GetStatValue(0);
			STAT_WEP_ALPHA = 50;
		}
		else {
			switch (modelId) {
			case MODEL_GUN_DILDO1:
				sprite = weaponicons[1];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_DILDO2:
				sprite = weaponicons[2];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_VIBE1:
				sprite = weaponicons[3];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_VIBE2:
				sprite = weaponicons[4];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_FLOWERA:
				sprite = weaponicons[5];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_CANE:
				sprite = weaponicons[6];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_BOXWEE:
				sprite = weaponicons[7];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_BOXBIG:
				sprite = weaponicons[8];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_CELLPHONE:
				sprite = weaponicons[9];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_BRASSKNUCKLE:
				sprite = weaponicons[10];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GOLFCLUB:
				sprite = weaponicons[11];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_NITESTICK:
				sprite = weaponicons[12];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_KNIFECUR:
				sprite = weaponicons[13];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_BAT:
				sprite = weaponicons[14];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_SHOVEL:
				sprite = weaponicons[15];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_POOLCUE:
				sprite = weaponicons[16];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_KATANA:
				sprite = weaponicons[17];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_CHNSAW:
				sprite = weaponicons[18];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GRENADE:
				sprite = weaponicons[19];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_TEARGAS:
				sprite = weaponicons[20];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_MOLOTOV:
				sprite = weaponicons[21];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_MISSILE:
				sprite = weaponicons[22];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_COLT45:
				sprite = weaponicons[23];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(69)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_SILENCED:
				sprite = weaponicons[24];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(70)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_DESERT_EAGLE:
				sprite = weaponicons[25];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(71)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_CHROMEGUN:
				sprite = weaponicons[26];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(72)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_SAWNOFF:
				sprite = weaponicons[27];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(73)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_SHOTGSPA:
				sprite = weaponicons[28];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(74)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_MICRO_UZI:
				sprite = weaponicons[29];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(75)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_MP5LNG:
				sprite = weaponicons[30];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(76)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_FLARE:
				sprite = weaponicons[31];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_AK47:
				sprite = weaponicons[32];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(77)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_M4:
				sprite = weaponicons[33];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(78)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_CUNTGUN:
				sprite = weaponicons[34];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_SNIPER:
				sprite = weaponicons[35];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(79)* 0.001 * 100.0;
				STAT_WEP_ALPHA = 255;
				break;
			}
			switch (modelId) {
			case MODEL_ROCKETLA:
				sprite = weaponicons[36];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_HEATSEEK:
				sprite = weaponicons[37];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_FLAME:
				sprite = weaponicons[38];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_MINIGUN:
				sprite = weaponicons[39];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_SATCHEL:
				sprite = weaponicons[40];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_BOMB:
				sprite = weaponicons[41];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_SPRAYCAN:
				sprite = weaponicons[42];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_FIRE_EX:
				sprite = weaponicons[43];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_CAMERA:
				sprite = weaponicons[44];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_NVGOGGLES:
				sprite = weaponicons[45];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_IRGOGGLES:
				sprite = weaponicons[46];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_JETPACK:
				sprite = weaponicons[47];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_GUN_PARA:
				sprite = weaponicons[48];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
			switch (modelId) {
			case MODEL_TEC9:
				sprite = weaponicons[49];
				PrintIcon();
				stat3_2 = CStats::GetStatValue(0);
				STAT_WEP_ALPHA = 50;
				break;
			}
		}
	}

	static void HudTextures(int index) {
		CTxdStore::LoadTxd(index, hudfilename);
	}

	static void FontTextures(int index) {
		CTxdStore::LoadTxd(index, fontfilename);
	}

	static void FontData() {
		CFileMgr::OpenFile(fontdatafilename, "rb");
	}

	static void DrawWeaponAmmo(float x, float y, char *string) {
		CFont::SetBackground(0, 0);
		CFont::SetDropShadowPosition(HUD_AMMO_SHADOW);
		CFont::SetOutlinePosition(HUD_AMMO_OUTLINE);
		CFont::SetDropColor(CRGBA(HUD_DROPCOLOUR_AMMO_R, HUD_DROPCOLOUR_AMMO_G, HUD_DROPCOLOUR_AMMO_B, 255));
		CFont::SetColor(CRGBA(HUD_COLOUR_AMMO_R, HUD_COLOUR_AMMO_G, HUD_COLOUR_AMMO_B, 255));
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_AMMO_SIZE_X), SCREEN_MULTIPLIER(HUD_AMMO_SIZE_Y));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_AMMO_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_AMMO_POS_Y), string);
	}

	enum
	{
		FLASH_Armour = 3,
		FLASH_Health = 4,
		//FLASH_Breath = 10
	};

	static inline bool ShowFlashingItem(signed int nOnDuration, signed int nOffDuration)
	{
		return CTimer::m_snTimeInMillisecondsPauseMode % (nOnDuration + nOffDuration) < nOnDuration;
	}

	static void PrintHealthString(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_HEALTH_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTH_SIZE_Y));
		CFont::SetColor(CRGBA(HUD_COLOUR_HEALTH_R, HUD_COLOUR_HEALTH_G, HUD_COLOUR_HEALTH_B, HUD_COLOUR_HEALTH_A));
		CFont::SetDropColor(CRGBA(0, 0, 0, HUD_COLOUR_HEALTH_A));
		CFont::SetDropShadowPosition(HUD_GLOBAL_SHADOW);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);
		char str[10];
		sprintf(str, "%03d", (int)fPercentage);
		if (m_ItemToFlash != FLASH_Health || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_dwLastTimeArmourLost == CWorld::Players[0].m_dwLastTimeEnergyLost || CWorld::Players[0].m_dwLastTimeEnergyLost + ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				//CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTH_POS_X + 160.0f), SCREEN_COORD(HUD_POS_Y + HUD_HEALTH_POS_Y), "?");
				CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTH_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_HEALTH_POS_Y), str);

				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				hudIcons[0].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHICON_POS_X + -2.0), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHICON_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_HEALTHICON_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHICON_SIZE_Y), CRGBA(0, 0, 0, 255));
				hudIcons[0].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHICON_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHICON_POS_Y), SCREEN_MULTIPLIER(HUD_HEALTHICON_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHICON_SIZE_Y), CRGBA(HUD_COLOUR_HEALTH_R, HUD_COLOUR_HEALTH_G, HUD_COLOUR_HEALTH_B, 255));
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintArmourString(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_ARMOUR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOUR_SIZE_Y));
		CFont::SetColor(CRGBA(HUD_COLOUR_ARMOUR_R, HUD_COLOUR_ARMOUR_G, HUD_COLOUR_ARMOUR_B, HUD_COLOUR_ARMOUR_A));
		CFont::SetDropColor(CRGBA(0, 0, 0, HUD_COLOUR_ARMOUR_A));
		CFont::SetDropShadowPosition(HUD_GLOBAL_SHADOW);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);
		char str[10];
		sprintf(str, "%03d", (int)fPercentage);
		if (m_ItemToFlash != FLASH_Armour || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_dwLastTimeArmourLost == 0 || CWorld::Players[0].m_dwLastTimeArmourLost + ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				//CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTH_POS_X + 160.0f), SCREEN_COORD(HUD_POS_Y + HUD_HEALTH_POS_Y), "?");
				CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOUR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOUR_POS_Y), str);

				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				hudIcons[1].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURICON_POS_X + -2.0), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURICON_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_ARMOURICON_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURICON_SIZE_Y), CRGBA(0, 0, 0, 255));
				hudIcons[1].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURICON_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURICON_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURICON_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURICON_SIZE_Y), CRGBA(HUD_COLOUR_ARMOUR_R, HUD_COLOUR_ARMOUR_G, HUD_COLOUR_ARMOUR_B, 255));
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintBreathString(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_BREATH_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATH_SIZE_Y));
		CFont::SetColor(CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, HUD_COLOUR_BREATH_A));
		CFont::SetDropColor(CRGBA(0, 0, 0, HUD_COLOUR_BREATH_A));
		CFont::SetDropShadowPosition(HUD_GLOBAL_SHADOW);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);
		char str[10];
		sprintf(str, "%03d", (int)fPercentage);
		CPed *playa = FindPlayerPed(0);
		if (playa && playa->m_fArmour > 0.0f && playa->m_fArmour > 0.1f) {
			//CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTH_POS_X + 160.0f), SCREEN_COORD(HUD_POS_Y + HUD_HEALTH_POS_Y), "?");
			CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATH_POS_X + HUD_BREATH_POS_X_2), SCREEN_COORD(HUD_POS_Y + HUD_BREATH_POS_Y), str);

			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
			hudIcons[2].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATHICON_POS_X + -2.0 + HUD_BREATH_POS_X_2), SCREEN_COORD(HUD_POS_Y + HUD_BREATHICON_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_Y), CRGBA(0, 0, 0, 255));
			hudIcons[2].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATHICON_POS_X + HUD_BREATH_POS_X_2), SCREEN_COORD(HUD_POS_Y + HUD_BREATHICON_POS_Y), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_Y), CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, 255));
			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
		}
		else {
			//CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTH_POS_X + 160.0f), SCREEN_COORD(HUD_POS_Y + HUD_HEALTH_POS_Y), "?");
			CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATH_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_BREATH_POS_Y), str);

			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
			hudIcons[2].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATHICON_POS_X + -2.0), SCREEN_COORD(HUD_POS_Y + HUD_BREATHICON_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_Y), CRGBA(0, 0, 0, 255));
			hudIcons[2].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATHICON_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_BREATHICON_POS_Y), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATHICON_SIZE_Y), CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, 255));
			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
		}
	}

	static void PrintHealthBar(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		if (m_ItemToFlash != FLASH_Health || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_dwLastTimeArmourLost == CWorld::Players[0].m_dwLastTimeEnergyLost || CWorld::Players[0].m_dwLastTimeEnergyLost + ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				if (SquareBar == 1) {
					CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHBAR_POS_Y), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_HEALTH_R, HUD_COLOUR_HEALTH_G, HUD_COLOUR_HEALTH_B, HUD_COLOUR_HEALTH_A), CRGBA(0, 0, 0, HUD_COLOUR_HEALTH_A));
					hudIcons[5].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHBAR_POS_Y), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
				}
				else {
					CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHBAR_POS_X + -5.0f), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHBAR_POS_Y + 6.0f), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_X + -9.0f), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_Y + -11.0f), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_HEALTH_R, HUD_COLOUR_HEALTH_G, HUD_COLOUR_HEALTH_B, HUD_COLOUR_HEALTH_A), CRGBA(0, 0, 0, HUD_COLOUR_HEALTH_A));
					hudIcons[6].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHBAR_POS_Y), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
				}
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintArmourBar(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		if (m_ItemToFlash != FLASH_Armour || ShowFlashingItem(300, 300)) {
			if (CWorld::Players[0].m_dwLastTimeArmourLost == 0 || CWorld::Players[0].m_dwLastTimeArmourLost + ENERGY_LOSS_FLASH_DURATION < CTimer::m_snTimeInMilliseconds || ShowFlashingItem(150, 150))
			{
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
				if (SquareBar == 1) {
					CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_ARMOUR_R, HUD_COLOUR_ARMOUR_G, HUD_COLOUR_ARMOUR_B, HUD_COLOUR_ARMOUR_A), CRGBA(0, 0, 0, HUD_COLOUR_ARMOUR_A));
					hudIcons[5].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
				}
				else {
					CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + -5.0f), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y + 6.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X + -9.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y + -11.0f), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_ARMOUR_R, HUD_COLOUR_ARMOUR_G, HUD_COLOUR_ARMOUR_B, HUD_COLOUR_ARMOUR_A), CRGBA(0, 0, 0, HUD_COLOUR_ARMOUR_A));
					hudIcons[6].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
				}
				RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
			}
		}
	}

	static void PrintBreathBar(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {
		CPed *playa = FindPlayerPed(0);
		if (playa && playa->m_fArmour > 0.0f && playa->m_fArmour > 0.1f) {
			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
			if (SquareBar == 1) {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + HUD_BREATH_POS_X_2), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, HUD_COLOUR_BREATH_A), CRGBA(0, 0, 0, HUD_COLOUR_BREATH_A));

				hudIcons[5].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + HUD_BREATH_POS_X_2), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
			}
			else {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + HUD_BREATH_POS_X_2 + -5.0f), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y + 6.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X + -9.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y + -11.0f), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, HUD_COLOUR_BREATH_A), CRGBA(0, 0, 0, HUD_COLOUR_BREATH_A));
				hudIcons[6].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + HUD_BREATH_POS_X_2), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
			}
			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
		}
		else {

			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
			if (SquareBar == 1) {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, HUD_COLOUR_BREATH_A), CRGBA(0, 0, 0, HUD_COLOUR_BREATH_A));
				hudIcons[5].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
			}
			else {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + -5.0f), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y + 6.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X + -9.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y + -11.0f), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, HUD_COLOUR_BREATH_A), CRGBA(0, 0, 0, HUD_COLOUR_BREATH_A));
				hudIcons[6].Draw(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), CRGBA(0, 0, 0, 255));
			}
			RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
		}
	}

	static void DrawHealth(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {

		if (Style == 0) {
			PrintHealthString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 1) {
			PrintHealthString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 2) {
			PrintHealthBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 3) {
			PrintHealthBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 4) {
			if (DrawBarChart == 1) {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHBAR_POS_X + -2.0f), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHBAR_POS_Y + 3.0f), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(0, 0, 0, 255), CRGBA(0, 0, 0, 255));
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_HEALTHBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_HEALTHBAR_POS_Y), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_HEALTHBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_HEALTH_R, HUD_COLOUR_HEALTH_G, HUD_COLOUR_HEALTH_B, 255), CRGBA(0, 0, 0, 255));
			}
			else {
				PrintHealthString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
					drawPercentage, drawBorder, dwColor, dwForeColor);
			}
		}
	}

	static void DrawArmour(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {

		if (Style == 0) {
			PrintArmourString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 1) {
			PrintArmourString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 2) {
			PrintArmourBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 3) {
			PrintArmourBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 4) {
			if (DrawBarChart == 1) {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X + -2.0f), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y + 3.0f), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(0, 0, 0, 255), CRGBA(0, 0, 0, 255));
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_ARMOURBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_ARMOURBAR_POS_Y), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_ARMOURBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_ARMOUR_R, HUD_COLOUR_ARMOUR_G, HUD_COLOUR_ARMOUR_B, 255), CRGBA(0, 0, 0, 255));
			}
			else {
				PrintArmourString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
					drawPercentage, drawBorder, dwColor, dwForeColor);
			}
		}
	}
	
	static void DrawBreath(float posX, float posY, WORD wWidth, WORD wHeight, float fPercentage, BYTE drawBlueLine,
		BYTE drawPercentage, BYTE drawBorder, CRGBA dwColor, CRGBA dwForeColor) {

		if (Style == 0) {
			PrintBreathString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 1) {
			PrintBreathString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 2) {
			PrintBreathBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 3) {
			PrintBreathBar(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
				drawPercentage, drawBorder, dwColor, dwForeColor);
		}

		if (Style == 4) {
			if (DrawBarChart == 1) {
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATHBAR_POS_X + -2.0f), SCREEN_COORD(HUD_POS_Y + HUD_BREATHBAR_POS_Y + 3.0f), SCREEN_MULTIPLIER(HUD_BREATHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATHBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(0, 0, 0, 255), CRGBA(0, 0, 0, 255));
				CSprite2d::DrawBarChart(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_BREATHBAR_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_BREATHBAR_POS_Y), SCREEN_MULTIPLIER(HUD_BREATHBAR_SIZE_X), SCREEN_MULTIPLIER(HUD_BREATHBAR_SIZE_Y), fPercentage, 0, 0, 0, CRGBA(HUD_COLOUR_BREATH_R, HUD_COLOUR_BREATH_G, HUD_COLOUR_BREATH_B, 255), CRGBA(0, 0, 0, 255));
			}
			else {
				PrintBreathString(posX, posY, wWidth, wHeight, fPercentage, drawBlueLine,
					drawPercentage, drawBorder, dwColor, dwForeColor);
			}
		}
	}

	static void DrawMoney() {
		int displayedScore = CWorld::Players[CWorld::PlayerInFocus].m_dwDisplayMoney;
		const char* displayScore;
		char str[16];

		if (displayedScore < 0)
		{
			CFont::SetColor(CRGBA(255, 38, 41, HUD_COLOUR_MONEY_A));
			displayedScore = -displayedScore;
			displayScore = MinusMoneyString;
		}
		else
		{
			CFont::SetColor(CRGBA(HUD_COLOUR_MONEY_R, HUD_COLOUR_MONEY_G, HUD_COLOUR_MONEY_B, HUD_COLOUR_MONEY_A));
			displayScore = MoneyString;
		}

		_snprintf(str, sizeof(str), displayScore, displayedScore);

		CFont::SetBackground(0, 0);
		CFont::SetProp(false);
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetRightJustifyWrap(0.0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_MONEY_SIZE_X), SCREEN_MULTIPLIER(HUD_MONEY_SIZE_Y));
		CFont::SetDropShadowPosition(HUD_GLOBAL_SHADOW);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);
		CFont::SetDropColor(CRGBA(HUD_DROPCOLOUR_CLOCK_R, HUD_DROPCOLOUR_CLOCK_G, HUD_DROPCOLOUR_CLOCK_B, HUD_DROPCOLOUR_CLOCK_A));
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_MONEY_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_MONEY_POS_Y), str);
	}

	static void DrawClock(float posX, float posY) {
		char str[16];
		CClock* CClock;

		CFont::SetProp(0);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_CLOCK_SIZE_X), SCREEN_MULTIPLIER(HUD_CLOCK_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetRightJustifyWrap(0.0);
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetDropColor(CRGBA(HUD_DROPCOLOUR_CLOCK_R, HUD_DROPCOLOUR_CLOCK_G, HUD_DROPCOLOUR_CLOCK_B, HUD_DROPCOLOUR_CLOCK_A));
		CFont::SetColor(CRGBA(HUD_COLOUR_CLOCK_R, HUD_COLOUR_CLOCK_G, HUD_COLOUR_CLOCK_B, HUD_COLOUR_CLOCK_A));
		CFont::SetDropShadowPosition(HUD_GLOBAL_SHADOW);
		//CFont::SetOutlinePosition(HUD_GLOBAL_OUTLINE);

		_snprintf(str, sizeof(str), "%02d:%02d", CClock->ms_nGameClockHours, CClock->ms_nGameClockMinutes);

		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_CLOCK_POS_X), SCREEN_COORD(HUD_POS_Y + HUD_CLOCK_POS_Y), str);
	}

	static void PrintStar(float x, float y, float width, float height, CRGBA color) {
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZTESTENABLE, 0);
		hudIcons[3].Draw((x), (y), (width), (height), CRGBA(color));
		RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEZWRITEENABLE, 0);
	}

	static void DrawWanted() {
		int nWantedLevel = FindPlayerWanted(-1)->m_dwWantedLevel;
		int nTimeOfWLChange = FindPlayerWanted(-1)->m_dwLastTimeWantedLevelChanged;

		if (nWantedLevel) {
			float fCurrentPos = RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_STAR_POS_X);
			for (int i = 0; i < 6; ++i, fCurrentPos -= SCREEN_MULTIPLIER(HUD_STAR_SPACE)) {
				if (nWantedLevel > i && (CTimer::m_snTimeInMilliseconds > nTimeOfWLChange + 2000 || ShowFlashingItem(150, 150))) {
					PrintStar(fCurrentPos + 2.0, SCREEN_COORD(HUD_POS_Y + HUD_STAR_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_STAR_SIZE_X), SCREEN_MULTIPLIER(HUD_STAR_SIZE_Y), CRGBA(0, 0, 0, HUD_COLOUR_SHADOWSTAR_A)); // Shadow
					PrintStar(fCurrentPos, SCREEN_COORD(HUD_POS_Y + HUD_STAR_POS_Y), SCREEN_MULTIPLIER(HUD_STAR_SIZE_X), SCREEN_MULTIPLIER(HUD_STAR_SIZE_Y), CRGBA(HUD_COLOUR_STAR_R, HUD_COLOUR_STAR_G, HUD_COLOUR_STAR_B, 255));
				}
				else {
					PrintStar(fCurrentPos + 2.0, SCREEN_COORD(HUD_POS_Y + HUD_STAR_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_STAR_SIZE_X), SCREEN_MULTIPLIER(HUD_STAR_SIZE_Y), CRGBA(0, 0, 0, HUD_COLOUR_SHADOWSTAR_A2)); // Shadow
					PrintStar(fCurrentPos, SCREEN_COORD(HUD_POS_Y + HUD_STAR_POS_Y), SCREEN_MULTIPLIER(HUD_STAR_SIZE_X), SCREEN_MULTIPLIER(HUD_STAR_SIZE_Y), CRGBA(HUD_COLOUR_STARBACK_R, HUD_COLOUR_STARBACK_G, HUD_COLOUR_STARBACK_B, HUD_COLOUR_SHADOWSTAR_A));
				}
			}
		}
		else {
			float fCurrentPos = RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_STAR_POS_X);
			for (int i = 0; i < 6; ++i, fCurrentPos -= SCREEN_MULTIPLIER(HUD_STAR_SPACE)) {
				PrintStar(fCurrentPos + 2.0, SCREEN_COORD(HUD_POS_Y + HUD_STAR_POS_Y + 2.0), SCREEN_MULTIPLIER(HUD_STAR_SIZE_X), SCREEN_MULTIPLIER(HUD_STAR_SIZE_Y), CRGBA(0, 0, 0, HUD_COLOUR_SHADOWSTAR_A3)); // Shadow
				PrintStar(fCurrentPos, SCREEN_COORD(HUD_POS_Y + HUD_STAR_POS_Y), SCREEN_MULTIPLIER(HUD_STAR_SIZE_X), SCREEN_MULTIPLIER(HUD_STAR_SIZE_Y), CRGBA(HUD_COLOUR_STARBACK_R, HUD_COLOUR_STARBACK_G, HUD_COLOUR_STARBACK_B, HUD_COLOUR_SHADOWSTAR_A));
			}
		}
	}

	static void DrawSubtitles(float x, float y, char *str) {
		CFont::SetBackground(0, 0);
		CFont::SetColor(CRGBA(255, 255, 255, 255));
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetDropShadowPosition(2);
		CFont::SetScale(SCREEN_MULTIPLIER(1.0f), SCREEN_MULTIPLIER(2.5f));
		CFont::PrintString(SCREEN_COORD_CENTER_LEFT(0.0f), SCREEN_COORD_BOTTOM(150.0f), str);
	}

	static void DrawVitalStats() {

		float POSX = 90.0f;
		float POSY = 450.0f;

		CPed *player = FindPlayerPed(-1);
		CPad *pad = CPad::GetPad(0);

		if (!pad->GetDisplayVitalStats(player) || FindPlayerVehicle(-1, 0)) {
		}
		else {
			window.Draw(SCREEN_COORD(POSX), SCREEN_COORD(POSY), SCREEN_MULTIPLIER(170.0f), SCREEN_MULTIPLIER(350.0f), CRGBA(0, 0, 0, 180));

			// Header
			CFont::SetBackground(0, 0);
			CFont::SetScale(SCREEN_MULTIPLIER(1.4), SCREEN_MULTIPLIER(3.0));
			CFont::SetAlignment(ALIGN_LEFT);
			CFont::SetRightJustifyWrap(0.0);
			CFont::SetFontStyle(FONT_GOTHIC);
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetDropShadowPosition(0);
			CFont::SetOutlinePosition(1);

			char *key = TheText.Get("FEH_STA");
			//CFont::PrintString(SCREEN_COORD(POSX + (10.0f)), SCREEN_COORD(POSY + (-30.0f)), key);

			// Stats
			CFont::SetBackground(0, 0);
			CFont::SetScale(SCREEN_MULTIPLIER(0.5), SCREEN_MULTIPLIER(1.2));
			CFont::SetAlignment(ALIGN_LEFT);
			CFont::SetRightJustifyWrap(0.0);
			CFont::SetFontStyle(FONT_SUBTITLES);
			CFont::SetDropColor(CRGBA(0, 0, 0, 255));
			CFont::SetDropShadowPosition(0);
			CFont::SetOutlinePosition(0);

			float POS_TEXT_Y = 10.0f;
			float SPACE = 15.5f;

			float POS_BAR_Y = 37.0f;
			float SIZE_BAR_X = 150.0f;
			float SIZE_BAR_Y = 10.0f;

			char *stat1 = TheText.Get("STAT068");
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (0.0f)), stat1);

			char *stat2 = TheText.Get("STAT225");
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (SPACE * 3)), stat2);

			char *stat3 = TheText.Get("CURWSKL");
			CFont::SetColor(CRGBA(255, 255, 255, STAT_WEP_ALPHA));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (SPACE * 6)), stat3);

			char *stat4 = TheText.Get("STAT022");
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (SPACE * 9)), stat4);

			char *stat5 = TheText.Get("STAT023");
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (SPACE * 12)), stat5);

			char *stat6 = TheText.Get("STAT021");
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (SPACE * 15)), stat6);

			char *stat7 = TheText.Get("STAT025");
			CFont::SetColor(CRGBA(255, 255, 255, 255));
			CFont::PrintString(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_TEXT_Y + (SPACE * 18)), stat7);

			// Bar
			float stat1_1 = CStats::GetStatValue(64) * 0.001 * 100.0; // RESPECT_TOTAL
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (0.0f)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat1_1, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, 255), CRGBA(255, 255, 255, 255));

			float stat2_2 = CStats::GetStatValue(225) * 0.001 * 100.0; // UNDERWATER_STAMINA
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (SPACE * 3)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat2_2, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, 255), CRGBA(255, 255, 255, 255));

			// WEAPON_SKILLS
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (SPACE * 6)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat3_2, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, STAT_WEP_ALPHA), CRGBA(255, 255, 255, 255));

			float stat4_2 = CStats::GetStatValue(22) * 0.001 * 100.0; // STAMINA
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (SPACE * 9)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat4_2, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, 255), CRGBA(255, 255, 255, 255));

			float stat5_2 = CStats::GetStatValue(23) * 0.001 * 100.0; // BODY_MUSCLE
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (SPACE * 12)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat5_2, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, 255), CRGBA(255, 255, 255, 255));

			float stat6_2 = CStats::GetStatValue(21) * 0.001 * 100.0; // FAT
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (SPACE * 15)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat6_2, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, 255), CRGBA(255, 255, 255, 255));

			float stat7_2 = CStats::GetStatValue(25) * 0.001 * 100.0; // SEX_APPEAL
			CSprite2d::DrawBarChart(SCREEN_COORD(HUD_POS_X + POSX + (10.0f)), SCREEN_COORD(POSY + POS_BAR_Y + (SPACE * 18)), SCREEN_MULTIPLIER(SIZE_BAR_X), SCREEN_MULTIPLIER(SIZE_BAR_Y), stat7_2, 0, 0, 0, CRGBA(HUD_COLOUR_STATBAR_R, HUD_COLOUR_STATBAR_G, HUD_COLOUR_STATBAR_B, 255), CRGBA(255, 255, 255, 255));
		}
	}

	static void AreaName(float x, float y, char* str) {
		CFont::SetFontStyle(HUD_AREA_FONT);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_AREA_SIZE_X), SCREEN_MULTIPLIER(HUD_AREA_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetDropShadowPosition(1);
		CFont::SetSlantRefPoint(RsGlobal.maximumWidth - SCREEN_COORD(HUD_AREA_POS_X), SCREEN_COORD_BOTTOM(HUD_AREA_POS_Y));
		CFont::SetSlant(HUD_AREA_SLANT);
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_AREA_POS_X), SCREEN_COORD_BOTTOM(HUD_POS_Y + HUD_AREA_POS_Y), str);
	}

	static void AreaNameColor(CRGBA value)
	{
		CFont::SetColor(CRGBA(HUD_COLOUR_AREA_R, HUD_COLOUR_AREA_G, HUD_COLOUR_AREA_B, value.alpha));
		AreaCRGBA = value;
	}

	static void VehicleName(float x, float y, char* str) {
		CFont::SetFontStyle(HUD_VEHICLE_FONT);
		CFont::SetBackground(0, 0);
		CFont::SetScale(SCREEN_MULTIPLIER(HUD_VEHICLE_SIZE_X), SCREEN_MULTIPLIER(HUD_VEHICLE_SIZE_Y));
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::SetDropShadowPosition(1);
		CFont::SetSlantRefPoint(RsGlobal.maximumWidth - SCREEN_COORD(HUD_VEHICLE_POS_X), SCREEN_COORD_BOTTOM(HUD_VEHICLE_POS_Y));
		CFont::SetSlant(HUD_VEHICLE_SLANT);
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(HUD_POS_X + HUD_VEHICLE_POS_X), SCREEN_COORD_BOTTOM(HUD_POS_Y + HUD_VEHICLE_POS_Y), str);
	}

	static void VehicleNameColor(CRGBA value)
	{
		CFont::SetColor(CRGBA(HUD_COLOUR_VEHICLE_R, HUD_COLOUR_VEHICLE_G, HUD_COLOUR_VEHICLE_B, value.alpha));
		AreaCRGBA = value;
	}

	static float GetHelpBoxXShift() {
		// if radar drawn
		if ((*(unsigned __int8 *)0xBA676C) != 2)
		{
			if (FindPlayerVehicle(-1, 0) && FindPlayerVehicle(-1, 0)->m_wModelIndex != 0x21B &&
				((*(unsigned __int32 *)((unsigned int)FindPlayerVehicle(-1, 0) + 0x594)) == 4 ||
				(*(unsigned __int32 *)((unsigned int)FindPlayerVehicle(-1, 0) + 0x594)) == 3))
				return fTextBoxPosnXWithRadarAndPlane;
			else if (FindPlayerPed(-1) && FindPlayerPed(-1)->m_aWeapons[FindPlayerPed(-1)->m_nActiveWeaponSlot].m_Type == WEAPON_PARACHUTE)
				return fTextBoxPosnXWithRadarAndPlane;
			else return fTextBoxPosnXWithRadar;
		}
		else return fTextBoxPosnX;
	}

	static void DrawHelpText() {
		char baseY;
		float alpha;
		float posX;
		float progress;
		char myText[300];
		if (g_pHelpMessage[0])
		{
			if (!CMessages::StringCompare(g_pHelpMessage, m_pLastHelpMessage, 400))
			{
				switch (g_HelpMessageState)
				{
				case 0: // STATE_FREE_TO_USE
					g_HelpMessageState = 2; // STATE_FADE_OUT
					g_HelpMessageTimer = 0;
					g_HelpMessageFadeTimer = 0;
					CMessages::StringCopy(g_pHelpMessageToPrint, g_pHelpMessage, 400);
					CFont::SetAlignment(ALIGN_LEFT);
					CFont::SetJustify(false);
					CFont::SetWrapx(SCREEN_MULTIPLIER(28 + 350 - 16));
					CFont::SetFontStyle(FONT_SUBTITLES);
					CFont::SetBackground(true, true);
					CFont::SetDropShadowPosition(0);
					g_fTextBoxNumLines = CFont::GetNumberLinesNoPrint(SCREEN_COORD(34.0), SCREEN_COORD(28.0), g_pHelpMessageToPrint) + 3;
					CFont::SetWrapx(RsGlobal.maximumWidth);
					PLAYONESHOT(32, 0.0);
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					g_HelpMessageState = 4; // STATE_REPLACE
					g_HelpMessageTimer = 5;
					break;
				default:
					break;
				}
				CMessages::StringCopy(m_pLastHelpMessage, g_pHelpMessage, 400);
			}
			alpha = 170.0;
			if (g_HelpMessageState)
			{
				switch (g_HelpMessageState)
				{
				case 2: // FADE_OUT
					if (!Camera_WidescreenOn)
					{
						g_HelpMessageFadeTimer += 2 * (CTimer::ms_fTimeStep * 0.02 * 1000.0);
						if (g_HelpMessageFadeTimer > 0)
						{
							g_HelpMessageFadeTimer = 0;
							g_HelpMessageState = 1; // STATE_IDLE
						}
						alpha = g_HelpMessageFadeTimer * 0.001 * 200.0;
					}
					break;
				case 3: // FADE_IN
					g_HelpMessageFadeTimer -= 2 * (CTimer::ms_fTimeStep * 0.02 * 1000.0);
					if (g_HelpMessageFadeTimer < 0 || Camera_WidescreenOn)
					{
						g_HelpMessageFadeTimer = 0;
						g_HelpMessageState = 0; // STATE_FREE_TO_USE
					}
					alpha = g_HelpMessageFadeTimer * 0.001 * 200.0;
					break;
				case 4: // REPLACE
					g_HelpMessageFadeTimer -= 2 * (CTimer::ms_fTimeStep * 0.02 * 1000.0);
					if (g_HelpMessageFadeTimer < 0)
					{
						g_HelpMessageFadeTimer = 0;
						g_HelpMessageState = 2; // FADE_OUT
						CMessages::StringCopy(g_pHelpMessageToPrint, m_pLastHelpMessage, 400);
					}
					alpha = g_HelpMessageFadeTimer * 0.001 * 200.0;
					break;
				case 1: // IDLE
					alpha = 200.0;
					g_HelpMessageFadeTimer = 600;
					if (!bTextBoxPermanent2)
					{
						if (g_HelpMessageTimer > g_fTextBoxNumLines * 1000.0 || m_HelpMessageQuick && g_HelpMessageTimer > 3000)
						{
							g_HelpMessageState = 3; // FADE_IN
							g_HelpMessageFadeTimer = 600;
						}
					}
					break;
				default:
					break;
				}
				if (!Cutscene_Running)
				{
					g_HelpMessageTimer += (CTimer::ms_fTimeStep * 0.02 * 1000.0);
					CFont::SetAlphaFade(alpha);
					CFont::SetProp(true);
					CFont::SetScale(SCREEN_MULTIPLIER(fTextBoxFontScaleX), SCREEN_MULTIPLIER(fTextBoxFontScaleY));
					if (g_HelpMessageStatId)
					{
						if (Camera_WidescreenOn)
						{
							CFont::SetAlphaFade(255.0);
							return;
						}
						if (g_HelpMessageStatId >= 10)
						{
							if (g_HelpMessageStatId >= 100)
								sprintf(g_filenameBuffer, "STAT%d", g_HelpMessageStatId);
							else
								sprintf(g_filenameBuffer, "STAT0%d", g_HelpMessageStatId);
						}
						else
							sprintf(g_filenameBuffer, "STAT00%d", g_HelpMessageStatId);
						CFont::SetAlignment(ALIGN_LEFT);
						CFont::SetJustify(false);
						CFont::SetWrapx(RsGlobal.maximumWidth);
						CFont::SetFontStyle(FONT_SUBTITLES);
						CFont::SetBackground(true, true);
						CFont::SetDropShadowPosition(0);
						CFont::SetBackgroundColor(CRGBA(0, 0, 0, alpha));
						CFont::SetColor(CRGBA(255, 255, 255, 255));
						posX = CFont::GetStringWidth((char *)TheText.Get(g_filenameBuffer), 1, 0) + SCREEN_MULTIPLIER(GetHelpBoxXShift() + fTextBoxBorderSize) + 4.0;
						CFont::SetWrapx(SCREEN_MULTIPLIER(164.0) + posX + 4.0);
						CFont::PrintString(SCREEN_COORD(GetHelpBoxXShift() + fTextBoxBorderSize) + 4.0, SCREEN_COORD((150.0 - flt_8D0938) * 0.6) + SCREEN_COORD(fTextBoxPosnY), (char *)TheText.Get(g_filenameBuffer));
						if (g_HelpMessageStatId == 336)
							progress = GetGroupMembersCount(FindPlayerPed(-1)->m_pPlayerData->m_dwPlayerGroup);
						else
							progress = GetPlayerStat(g_HelpMessageStatId);
						if (g_pHelpMessageToPrint[0] == '+')
						{
							CSprite2d::DrawBarChart(posX + SCREEN_COORD(15.0), SCREEN_COORD((157.0 - flt_8D0938) * 0.60000002) + SCREEN_COORD(fTextBoxPosnY + 4.0), SCREEN_MULTIPLIER(130.0), SCREEN_MULTIPLIER(20.0), fmax(1.0 / StatMax * progress * 100.0, 2.0), fmax((1.0 / StatMax) * dword_BAA468 * 100.0, 3.0), 0, 0, CRGBA(255, 255, 255, 255), CRGBA(255, 255, 255, 255));
							CFont::SetColor(CRGBA(255, 255, 255, 255));
							CFont::PrintString(posX + SCREEN_COORD(155.0), SCREEN_COORD((153.0 - flt_8D0938) * 0.6) + SCREEN_COORD(fTextBoxPosnY),
								g_pHelpMessageToPrint);
						}
						else
						{
							CSprite2d::DrawBarChart(posX + SCREEN_COORD(15.0), SCREEN_COORD((157.0 - flt_8D0938) * 0.60000002) + SCREEN_COORD(fTextBoxPosnY + 4.0), SCREEN_MULTIPLIER(130.0), SCREEN_MULTIPLIER(20.0), fmax(1.0 / StatMax * progress * 100.0, 2.0), fmax((1.0 / StatMax) * dword_BAA468 * 100.0, 3.0), 0, 0, CRGBA(255,255,255,255), CRGBA(255, 255, 255, 255));
							CFont::SetColor(CRGBA(255, 255, 255, 255));
							CFont::PrintString(posX + SCREEN_COORD(155.0), SCREEN_COORD((153.0 - flt_8D0938) * 0.6) + SCREEN_COORD(fTextBoxPosnY),
								g_pHelpMessageToPrint);
						}
					}
					else
					{
						if (g_BigMessage0[0] || g_BigMessage4[0] || byte_96C014[0])
						{
							CFont::SetAlphaFade(255.0);
							return;
						}
						CFont::SetAlignment(ALIGN_LEFT);
						CFont::SetJustify(false);
						if (g_fScriptTextBoxesWidth == 200.0)
						{
							CFont::SetWrapx(SCREEN_MULTIPLIER(GetHelpBoxXShift() + fTextBoxWidth - fTextBoxBorderSize) - 4.0);
						}
						else
							CFont::SetWrapx(SCREEN_MULTIPLIER(GetHelpBoxXShift() + (g_fScriptTextBoxesWidth - fTextBoxBorderSize)) - 4.0);
						CFont::SetFontStyle(FONT_SUBTITLES);
						CFont::SetBackground(true, true);
						CFont::SetDropShadowPosition(0);
						CFont::SetBackgroundColor(CRGBA(0, 0, 0, alpha));
						CFont::SetColor(CRGBA(255, 255, 255, 255));
						baseY = 0;
						if (Camera_WidescreenOn && !Menu_WidescreenOn)
							baseY = 56;
						CFont::PrintString(SCREEN_COORD(GetHelpBoxXShift() + fTextBoxBorderSize) + 4.0, SCREEN_COORD((baseY + 150.0 - flt_8D0938) * 0.6) + SCREEN_COORD(fTextBoxPosnY), g_pHelpMessageToPrint);
					}
					CFont::SetWrapx(RsGlobal.maximumWidth);
					CFont::SetAlphaFade(255.0);
					return;
				}
			}
		}
		else
			g_HelpMessageState = 0;
	}

	static void _cdecl DrawYouAreHereSprite(float x, float y) {
		CVector playaCoors = FindPlayerCentreOfWorld(0);
		CVector2D coors = { playaCoors.x, playaCoors.y };
		CVector2D radarCoors;
		CRadar::TransformRealWorldPointToRadarSpace(radarCoors, coors);
		CRadar::TransformRadarPointToScreenSpace(coors, radarCoors);
		x = coors.x * SCREEN_WIDTH * 0.0015625f;
		y = coors.y * SCREEN_HEIGHT * 0.002232143f;
		CRadar::LimitToMap(&x, &y);
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetBackground(0, 0);
		CFont::SetColor(CRGBA(MAP_COLOUR_ARROW_R, MAP_COLOUR_ARROW_G, MAP_COLOUR_ARROW_B, 255));
		CFont::SetScale(SCREEN_MULTIPLIER(0.8), SCREEN_MULTIPLIER(1.6));
		CFont::SetAlignment(ALIGN_LEFT);
		CFont::SetWrapx(0);
		CFont::SetDropShadowPosition(1);
		CFont::PrintString(x + SCREEN_COORD(80.0f), y + SCREEN_COORD(-60.0f), TheText.Get("MAP_YAH"));
		CFont::DrawFonts();

		hudIcons[4].Draw(x + SCREEN_COORD(-6.0f), y + SCREEN_COORD(-58.0f), SCREEN_MULTIPLIER(80.0), SCREEN_MULTIPLIER(80.0), CRGBA(0, 0, 0, 255)); // Shadow
		hudIcons[4].Draw(x + SCREEN_COORD(-8.0f), y + SCREEN_COORD(-60.0f), SCREEN_MULTIPLIER(80.0), SCREEN_MULTIPLIER(80.0), CRGBA(MAP_COLOUR_ARROW_R, MAP_COLOUR_ARROW_G, MAP_COLOUR_ARROW_B, 255));
	};

	static void DrawBustedWastedMessage(float x, float y, char *str) {

		CFont::SetOutlinePosition(0);
		CFont::SetDropShadowPosition(1);
		CFont::SetFontStyle(FONT_PRICEDOWN);
		CFont::SetScale(SCREEN_MULTIPLIER(3.0f), SCREEN_MULTIPLIER(5.0f));
		CFont::SetColor(CRGBA(HUD_COLOUR_WB_R, HUD_COLOUR_WB_G, HUD_COLOUR_WB_B, 255));
		CFont::PrintStringFromBottom(x, y, str);
	}

	CHudNew() {
		Ini();

		if (Enable == 1) {
			Events::initRwEvent += CHudNew::ClassicHudTextures;
			Events::initRwEvent += CHudNew::WeaponTextures;
			patch::RedirectCall(0x5BA865, CHudNew::HudTextures);
			patch::RedirectCall(0x5BA6A4, CHudNew::FontTextures);
			patch::RedirectCall(0x7187DB, CHudNew::FontData);

			patch::RedirectJump(0x58D7D0, CHudNew::DrawWeaponIcon);
			patch::RedirectCall(0x58962A, CHudNew::DrawWeaponAmmo);
			patch::RedirectCall(0x589395, CHudNew::DrawHealth);
			patch::RedirectCall(0x58917A, CHudNew::DrawArmour);
			patch::RedirectCall(0x589252, CHudNew::DrawBreath);
			patch::RedirectCall(0x58F607, CHudNew::DrawMoney);
			patch::RedirectCall(0x58EC21, CHudNew::DrawClock);
			patch::RedirectCall(0x58FBDB, CHudNew::DrawWanted);
			patch::RedirectCall(0x58C68A, CHudNew::DrawSubtitles);
			if (StatBox) {
				Events::drawHudEvent += CHudNew::DrawVitalStats;
				patch::Set<BYTE>(0x58FC2C, 0xEB); // Disable Default statbox
			}
			patch::RedirectCall(0x58AE5D, CHudNew::AreaName);
			patch::RedirectCall(0x58AE02, CHudNew::AreaNameColor);

			patch::RedirectCall(0x58B156, CHudNew::VehicleName);
			patch::RedirectCall(0x58B0ED, CHudNew::VehicleNameColor);

			patch::RedirectCall(0x58FCFA, CHudNew::DrawHelpText);

			patch::RedirectCall(0x584A52, CHudNew::DrawYouAreHereSprite);

			patch::RedirectCall(0x58CC6A, CHudNew::DrawBustedWastedMessage);
		}
	}
} CHudNew;
