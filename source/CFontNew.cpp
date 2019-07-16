#include "plugin.h"
#include "CTxdStore.h"
#include "CFontNew.h"
#include "CHudNew.h"

using namespace plugin;

tFontData *gFontData = (tFontData *)0xC718B0;

bool CFontNew::ms_bFontsLoaded;

int III_Size[2][13 * 16] = {
	{ // font2
	13, 12, 31, 35, 23, 35, 31,  9, 14, 15, 25, 30, 11, 17, 13, 31,
	23, 16, 22, 21, 24, 23, 23, 20, 23, 22, 10, 35, 26, 26, 26, 26,
	30, 26, 24, 23, 24, 22, 21, 24, 26, 10, 20, 26, 22, 29, 26, 25,
	23, 25, 24, 24, 22, 25, 24, 29, 29, 23, 25, 37, 22, 37, 35, 37,
	35, 21, 22, 21, 21, 22, 13, 22, 21, 10, 16, 22, 11, 32, 21, 21,
	23, 22, 16, 20, 14, 21, 20, 30, 25, 21, 21, 33, 33, 33, 33, 35,
	27, 27, 27, 27, 32, 24, 23, 23, 23, 23, 11, 11, 11, 11, 26, 26,
	26, 26, 26, 26, 26, 25, 26, 21, 21, 21, 21, 32, 23, 22, 22, 22,
	22, 11, 11, 11, 11, 22, 22, 22, 22, 22, 22, 22, 22, 26, 21, 24,
	12, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 18, 26, 26,
	26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
	20,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	},
	
	{ // font1
	15, 14, 16, 25, 19, 26, 22, 11, 18, 18, 27, 26, 13, 19,  9, 27,
	19, 18, 19, 19, 22, 19, 20, 18, 19, 20, 12, 32, 15, 32, 15, 35,
	15, 19, 19, 19, 19, 19, 16, 19, 20,  9, 19, 20, 14, 29, 19, 20,
	19, 19, 19, 19, 21, 19, 20, 32, 20, 19, 19, 33, 31, 39, 37, 39,
	37, 21, 21, 21, 23, 21, 19, 23, 23, 10, 19, 20, 16, 26, 23, 23,
	20, 20, 20, 22, 21, 22, 22, 26, 22, 22, 23, 35, 35, 35, 35, 37,
	19, 19, 19, 19, 29, 19, 19, 19, 19, 19,  9,  9,  9,  9, 19, 19,
	19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 30, 19, 19, 19, 19,
	19, 10, 10, 10, 10, 19, 19, 19, 19, 19, 19, 19, 19, 19, 23, 35,
	12, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
	19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 11, 19, 19,
	19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
	19,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	}
};

int VC_Size[2][13 * 16] = {
	{ // font2
	12, 13, 13, 28, 28, 28, 28,  8, 17, 17, 30, 28, 28, 12,  9, 21,
	28, 14, 28, 28, 28, 28, 28, 28, 28, 28, 13, 13, 30, 30, 30, 30,
	10, 25, 23, 21, 24, 22, 20, 24, 24, 17, 20, 22, 20, 30, 27, 27,
	26, 26, 24, 23, 24, 31, 23, 31, 24, 23, 21, 28, 33, 33, 14, 28,
	10, 11, 12,  9, 11, 10, 10, 12, 12,  7,  7, 13,  5, 18, 12, 10,
	12, 11, 10, 12,  8, 13, 13, 18, 17, 13, 12, 30, 30, 37, 35, 37,
	25, 25, 25, 25, 33, 21, 24, 24, 24, 24, 17, 17, 17, 17, 27, 27,
	27, 27, 31, 31, 31, 31, 11, 11, 11, 11, 11, 20,  9, 10, 10, 10,
	10,  7,  7,  7,  7, 10, 10, 10, 10, 13, 13, 13, 13, 27, 12, 30,
	27, 16, 27, 27, 27, 27, 27, 27, 27, 27, 18, 29, 26, 25, 28, 26,
	25, 27, 28, 12, 24, 25, 24, 30, 27, 29, 26, 26, 25, 26, 25, 26,
	28, 32, 27, 26, 26, 29, 29, 29, 29, 33, 25, 26, 26, 26, 26, 14,
	14, 14, 14, 29, 29, 29, 29, 26, 26, 26, 26, 21, 25, 30, 27, 27,
	},

	{ // font1
	15,  9, 17, 27, 20, 34, 23, 12, 12, 12, 21, 20, 12, 14, 12, 15,
	23, 15, 21, 21, 21, 21, 21, 21, 20, 21, 12, 12, 24, 24, 24, 19,
	10, 22, 21, 21, 22, 18, 19, 24, 22,  9, 16, 21, 16, 28, 24, 23,
	22, 23, 22, 20, 18, 23, 23, 31, 23, 19, 21, 21, 13, 35, 11, 21,
	10, 17, 19, 15, 18, 17, 12, 19, 17,  6,  9, 19,  7, 26, 17, 17,
	17, 17, 10, 16, 11, 17, 18, 24, 20, 18, 16, 21, 17, 20, 15, 15,
	22, 22, 22, 22, 29, 19, 16, 16, 16, 16, 11, 11, 11, 11, 27, 27,
	27, 27, 23, 23, 23, 23, 20, 19, 19, 19, 19, 30, 14, 19, 19, 19,
	19,  9,  9,  9,  9, 21, 21, 21, 21, 18, 18, 18, 18, 24, 19, 19,
	20, 18, 19, 19, 21, 19, 19, 19, 19, 19, 16, 19, 19, 19, 20, 19,
	16, 19, 19,  9, 19, 20, 14, 29, 19, 19, 19, 19, 19, 19, 21, 19,
	20, 32, 21, 19, 19, 19, 19, 19, 19, 29, 19, 19, 19, 19, 19,  9,
	 9,  9,  9, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 19, 10, 20,
	}
};

int SA_Size[2][13 * 16] = {
	{ // font2
	12, 13, 13, 28, 28, 28, 28,  8, 17, 17, 30, 28, 28, 12,  9, 21,
	28, 14, 28, 28, 28, 28, 28, 28, 28, 28, 13, 13, 30, 30, 30, 30,
	10, 25, 23, 21, 24, 22, 20, 24, 24, 17, 20, 22, 20, 30, 27, 27,
	26, 26, 24, 23, 24, 31, 23, 31, 24, 23, 21, 28, 33, 33, 14, 28,
	10, 11, 12,  9, 11, 10, 10, 12, 12,  7,  7, 13,  5, 18, 12, 10,
	12, 11, 10, 12,  8, 13, 13, 18, 17, 13, 12, 30, 30, 37, 35, 37,
	25, 25, 25, 25, 33, 21, 24, 24, 24, 24, 17, 17, 17, 17, 27, 27,
	27, 27, 31, 31, 31, 31, 11, 11, 11, 11, 11, 20,  9, 10, 10, 10,
	10,  7,  7,  7,  7, 10, 10, 10, 10, 13, 13, 13, 13, 27, 12, 30,
	27, 16, 27, 27, 27, 27, 27, 27, 27, 27, 18, 29, 26, 25, 28, 26,
	25, 27, 28, 12, 24, 25, 24, 30, 27, 29, 26, 26, 25, 26, 25, 26,
	28, 32, 27, 26, 26, 29, 29, 29, 29, 33, 25, 26, 26, 26, 26, 14,
	14, 14, 14, 29, 29, 29, 29, 26, 26, 26, 26, 21, 25, 30, 27, 27,
	},

	{ // font1
	15,  9, 17, 27, 20, 34, 23, 12, 12, 12, 21, 20, 12, 14, 12, 15,
	23, 15, 21, 21, 21, 21, 21, 21, 20, 21, 12, 12, 24, 24, 24, 19,
	10, 22, 19, 19, 22, 16, 19, 24, 22, 11, 16, 21, 15, 28, 24, 27,
	20, 25, 19, 19, 18, 23, 23, 31, 23, 19, 21, 21, 13, 35, 11, 21,
	10, 19, 20, 14, 20, 19, 13, 20, 19,  9,  9, 19,  9, 29, 19, 21,
	19, 19, 15, 15, 14, 18, 19, 27, 20, 20, 17, 21, 17, 20, 15, 15,
	22, 22, 22, 22, 29, 19, 16, 16, 16, 16, 11, 11, 11, 11, 27, 27,
	27, 27, 23, 23, 23, 23, 20, 19, 19, 19, 19, 30, 14, 19, 19, 19,
	19,  9,  9,  9,  9, 21, 21, 21, 21, 18, 18, 18, 18, 24, 19, 19,
	20, 18, 19, 19, 21, 19, 19, 19, 19, 19, 16, 19, 19, 19, 20, 19,
	16, 19, 19,  9, 19, 20, 14, 29, 19, 19, 19, 19, 19, 19, 21, 19,
	20, 32, 21, 19, 19, 19, 19, 19, 19, 29, 19, 19, 19, 19, 19,  9,
	 9,  9,  9, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 19, 10,  9,
	}
};

void CFontNew::Initialise() {
	if (!ms_bFontsLoaded) {
		int Slot = CTxdStore::AddTxdSlot("fonts_new");
		char FileName[128];
		strcpy_s(FileName, "classichud\\fonts\\"); // Set folder,
		strcat_s(FileName, CHudNew::GetGamePrefix()); // game prefix,
		strcat_s(FileName, "_");
		strcat_s(FileName, "fonts.txd"); // file name.
		puts(FileName);
		CTxdStore::LoadTxd(Slot, PLUGIN_PATH(FileName));
		CTxdStore::AddRef(Slot);
		CTxdStore::PopCurrentTxd();
		CTxdStore::SetCurrentTxd(Slot);

		CFont::Sprite[0].SetTexture("font2");
		CFont::Sprite[1].SetTexture("font1");

		CTxdStore::PopCurrentTxd();

		for (int i = 0; i < 208; i++) {
			if (CHudNew::GetGameMode() == GAMEMODE_III) {
				gFontData[0].m_propValues[i] = III_Size[0][i];
				gFontData[1].m_propValues[i] = III_Size[1][i];
				gFontData[0].m_unpropValue = III_Size[1][192];
				gFontData[1].m_unpropValue = III_Size[1][192];
			}
			else if (CHudNew::GetGameMode() == GAMEMODE_VC) {
				gFontData[0].m_propValues[i] = VC_Size[0][i];
				gFontData[1].m_propValues[i] = VC_Size[1][i];
				gFontData[0].m_unpropValue = VC_Size[1][207];
				gFontData[1].m_unpropValue = VC_Size[1][207];
			}
			else if (CHudNew::GetGameMode() == GAMEMODE_SA) {
				gFontData[0].m_propValues[i] = SA_Size[0][i];
				gFontData[1].m_propValues[i] = SA_Size[1][i];
				gFontData[0].m_unpropValue = SA_Size[0][192];
				gFontData[1].m_unpropValue = SA_Size[1][192];
			}
			else if (CHudNew::GetGameMode() == GAMEMODE_LCS) {

			}
			else if (CHudNew::GetGameMode() == GAMEMODE_VCS) {

			}
			else if (CHudNew::GetGameMode() == GAMEMODE_ADVANCE) {

			}
		}

		ms_bFontsLoaded = true;
	}
}

void CFontNew::Shutdown() {
	if (ms_bFontsLoaded) {
		for (int i = 0; i < 2; ++i)
			CFont::Sprite[i].Delete();

		int Slot = CTxdStore::FindTxdSlot("fonts_new");
		CTxdStore::RemoveTxdSlot(Slot);

		ms_bFontsLoaded = false;
	}
}

void CFontNew::SetFontStyle(int Font) {
	if (CHudNew::GetGameMode() == GAMEMODE_III) {
		if (Font > III_FONT_PRICEDOWN)
			Font = III_FONT_PRICEDOWN;

		CFont::m_FontTextureId = Font;
		CFont::m_FontStyle = 0;
	}
	else if (CHudNew::GetGameMode() == GAMEMODE_VC) {
		if (Font >= VC_FONT_PRICEDOWN) {
			CFont::m_FontTextureId = 1;
			CFont::m_FontStyle = 1;
		}
		else {
			CFont::m_FontTextureId = Font;
			CFont::m_FontStyle = 0;
		}
	}
	else if (CHudNew::GetGameMode() == GAMEMODE_SA) {
		if (Font == 2) {
			CFont::m_FontTextureId = 0;
			CFont::m_FontStyle = 2;
		}
		else if (Font == 3) {
			CFont::m_FontTextureId = 1;
			CFont::m_FontStyle = 1;
		}
		else {
			CFont::m_FontTextureId = Font;
			CFont::m_FontStyle = 0;
		}
	}
	else if (CHudNew::GetGameMode() == GAMEMODE_LCS) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CFontNew::InjectPatches() {
	Events::initRwEvent += Initialise;
	Events::shutdownRwEvent += Shutdown;

#if GTASA
	patch::RedirectJump(0x719490, CFontNew::SetFontStyle);
#endif
}