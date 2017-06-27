#include "Main.hpp"

using namespace plugin;

void __declspec(naked) RadarAlpha() {
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, reinterpret_cast<void *>(TRUE));
    _asm {
        mov     ecx, 586420h
        mov     al, [esp + 140h - 12Dh]
        test    al, al
        jmp     ecx
    }
}

class CRadarNew {
public:
    static CRGBA * __fastcall SetRadarDisc(CRGBA *rgba, int, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        rgba->red = 255;
        rgba->green = 255;
        rgba->blue = 255;
        rgba->alpha = 255;
        return rgba;
    }

    CRadarNew() {
        Ini();

        if (Enable == 1) {
            // Set Radar Range
            patch::SetFloat(0x586C9B, RADAR_RANGE);
            patch::Set<const void*>(0x586C66, &RADAR_RANGE);

            // Transparent Radar
            patch::Set<DWORD>(0x5864BD, RADAR_COLOUR_A);
            plugin::patch::RedirectJump(0x58641A, RadarAlpha);

            //Radar Disc RGBA
            patch::RedirectCall(0x58A8BD, &SetRadarDisc);
            patch::RedirectCall(0x58A813, &SetRadarDisc);
            patch::RedirectCall(0x58A967, &SetRadarDisc);
            patch::RedirectCall(0x58AA15, &SetRadarDisc);
        }
    }
} CRadarNew;