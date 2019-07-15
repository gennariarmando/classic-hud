#include "plugin.h"
#include "CFontNew.h"
#include "CHudNew.h"

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CHudNew::InjectPatches();
		CFontNew::InjectPatches();
	}
	return TRUE;
}
