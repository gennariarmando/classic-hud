#include "plugin.h"
#include "CHudNumbers.h"
#include "CSprite2d.h"
#include "CTxdStore.h"
#include "Settings.h"

CHudNumbers chudnumbers;
CSprite2d CHudNumbers::Sprite[HUDNUMBERS_TOTALSPRITES];
CVector2D CHudNumbers::m_vecFontScale;
CRGBA CHudNumbers::m_Color;
int CHudNumbers::m_nFontStyle;
bool CHudNumbers::ms_bHudNumbersLoaded;

CHudNumbers::CHudNumbers() {
	s.readIni();

	if (!s.m_bEnable)
		return;
}

void CHudNumbers::Initialise() {
	if (IsHudNumbersRequired()) {
		if (!ms_bHudNumbersLoaded) {
			int Slot = CTxdStore::AddTxdSlot("hudnumbers");
			CTxdStore::LoadTxd(Slot, PLUGIN_PATH(s.SetFileWithPrefix("classichud\\hudnumbers\\", "hudnumbers.txd")));
			CTxdStore::AddRef(Slot);
			CTxdStore::PopCurrentTxd();
			CTxdStore::SetCurrentTxd(Slot);

			Sprite[HUDNUMBERS].SetTexture("hudnumbers");

			CTxdStore::PopCurrentTxd();

			ms_bHudNumbersLoaded = true;
		}
	}
}

void CHudNumbers::Shutdown() {
	if (ms_bHudNumbersLoaded) {
		for (int i = 0; i < HUDNUMBERS_TOTALSPRITES; ++i)
			Sprite[i].Delete();

		int Slot = CTxdStore::FindTxdSlot("hudnumbers");
		CTxdStore::RemoveTxdSlot(Slot);

		ms_bHudNumbersLoaded = false;
	}
}

bool CHudNumbers::IsHudNumbersRequired() {
	return
		ClassicHud::GetGameMode() == GAMEMODE_LCS ||
		ClassicHud::GetGameMode() == GAMEMODE_VCS;
}

void CHudNumbers::SetFontScale(float w, float h) {
	m_vecFontScale.x = w;
	m_vecFontScale.y = h;
}

void CHudNumbers::SetColor(CRGBA & color) {
	m_Color = color;
}

void CHudNumbers::SetFontStyle(int style) {
	m_nFontStyle = style;
}

void CHudNumbers::PrintString(float x, float y, std::string text) {
	const float fSpriteWidth(512), fSpriteHeight(256);
	CRect rect;
	rect.left = x;

	Sprite[HUDNUMBERS].SetRenderState();

	for (unsigned int i = 0; i < text.length(); i++) {
		float fCharWidth(52), fCharHeight(64), fSpacing(0.25);
		char chr = text[i] - 48;

		char textFormat[2][13] = {
			{ '0', '1', '2', '3', '4', '5', '6' , '7', '8', '9', ':', },
			{ '0', '1', '2', '3', '4', '5', '6' , '7', '8', '9', '$', '*', ' ' },
		};

		if (m_nFontStyle) {
			if (text[i] == '$')
				chr = 10;
			chr += m_nFontStyle * 3;
		}
		else {
			if (text[i] == ':')
				fCharWidth = 32;
			else
				if (text[i] == '*') {
					chr = 22;
					fCharWidth = 64;
					m_vecFontScale.x *= 1.2f;
				}
		}

		rect.right = rect.left + (fCharWidth / 4) * m_vecFontScale.x;
		rect.bottom = y;
		rect.top = y + 20.0 * m_vecFontScale.y;
		float U1lef = (64 / fSpriteWidth) * (chr % 8);
		float V3top = (68 / fSpriteHeight) * (m_nFontStyle + chr / 8);
		float U2rig = U1lef + fCharWidth / fSpriteWidth;
		float V4top = V3top;
		float U3lef = U1lef;
		float V1bot = V3top + fCharHeight / fSpriteHeight;
		float U4rig = U2rig;
		float V2bot = V1bot;
		CSprite2d::AddToBuffer(rect, m_Color, U1lef, V1bot, U2rig, V2bot, U3lef, V3top, U4rig, V4top);
		rect.left += (fCharWidth / 4 + fSpacing) * m_vecFontScale.x;
	}
	CSprite2d::RenderVertexBuffer();
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, reinterpret_cast<void*>(0));
}