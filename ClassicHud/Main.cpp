#include "Main.hpp"

using namespace plugin;

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

	ClassicHud() {
		Ini();

		if (Enable) {
			Events::drawMenuBackgroundEvent += Text;
		}
	}
} ClassicHud;