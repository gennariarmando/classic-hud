#include "plugin.h"
#include "CFontNew.h"
#include "CHudNew.h"
#include "CRadarNew.h"
#include "Settings.h"

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		s.readIni();

		if (s.m_bEnable) {
			CHudNew::InjectPatches();
			CFontNew::InjectPatches();
			CRadarNew::InjectPatches();
		}
	}
	return TRUE;
}
