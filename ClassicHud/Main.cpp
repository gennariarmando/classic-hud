#include "Main.hpp"

using namespace plugin;

void __declspec(naked) OpaqueRadarHack() {
	RwEngineInstance->dOpenDevice.fpRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	_asm
	{
		mov		ecx, 586420h
		mov		al, [esp + 140h - 12Dh]
		test	al, al
		jmp		ecx
	}
}

class ClassicHud {
public:
	static void Text() { 
		CFont::SetScale(SCREEN_MULTIPLIER(0.5f), SCREEN_MULTIPLIER(1.1f));
		CFont::SetColor(CRGBA(225, 255, 225, 150));
		CFont::SetFontStyle(FONT_SUBTITLES);
		CFont::SetCentreSize(1.0);
		CFont::SetRightJustifyWrap(1.0);
		CFont::SetOutlinePosition(0);
		CFont::SetProp(true);
		CFont::SetAlignment(ALIGN_RIGHT);
		CFont::PrintString(RsGlobal.maximumWidth - SCREEN_COORD(5.0f), SCREEN_COORD(0.0f), SetTextBasedOnStyle);
	}

	static CRGBA * __fastcall SetRadarDisc(CRGBA *rgba, int, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		rgba->red = 255;
		rgba->green = 255;
		rgba->blue = 255;
		rgba->alpha = 255;
		return rgba;
	}

	ClassicHud() {
		Ini();

		if (Enable == 1) {
			Events::drawMenuBackgroundEvent += Text;

			// Set Default Zoom
			patch::SetFloat(0x586C9B, RADAR_RANGE);
			patch::Set<const void*>(0x586C66, &RADAR_RANGE);

			// Transparent Radar
			patch::Set<DWORD>(0x586433, 0x8D * 0 / 0xFF);
			patch::Set<DWORD>(0x58647C, RADAR_COLOUR_A);
			patch::Set<DWORD>(0x5864BD, RADAR_COLOUR_A);
			patch::Set<DWORD>(0x58A789, RADAR_COLOUR_A);
			patch::Set<DWORD>(0x58A88F, RADAR_COLOUR_A);
			patch::Set<DWORD>(0x58A8D9, RADAR_COLOUR_A);
			patch::Set<DWORD>(0x58A98F, RADAR_COLOUR_A);
			patch::RedirectJump(0x58641A, OpaqueRadarHack);

			//Radar Disc RGBA
			patch::RedirectCall(0x58A8BD, &SetRadarDisc);
			patch::RedirectCall(0x58A813, &SetRadarDisc);
			patch::RedirectCall(0x58A967, &SetRadarDisc);
			patch::RedirectCall(0x58AA15, &SetRadarDisc);
		}
	}
} ClassicHud;