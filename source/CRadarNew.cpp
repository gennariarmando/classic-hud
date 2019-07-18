#include "plugin.h"
#include "CHud.h"
#include "CRadar.h"
#include "CTxdStore.h"
#include "CHudNew.h"
#include "CRadarNew.h"
#include "Settings.h"
#include "ScreenAddition.h"

using namespace plugin;

bool CRadarNew::ms_bSpritesLoaded;

char *hudTextureNames2[] = {
	"fist",
	"fistm",
	"sitem16",
	"sitem16m",
	"siterocket",
	"siterocketm",
	"radardisc",
	"radardisca",
	"radarRingPlane",
	"radarRingPlaneA",
	"SkipIcon",
	"SkipIconA"
};

char *radarIconList[] = {
	"radar_centre",
	"arrow",
	"radar_north",
	"radar_airYard",
	"radar_ammugun",
	"radar_barbers",
	"radar_BIGSMOKE",
	"radar_boatyard",
	"radar_burgerShot",
	"radar_bulldozer",
	"radar_CATALINAPINK",
	"radar_CESARVIAPANDO",
	"radar_chicken" ,
	"radar_CJ",
	"radar_CRASH1",
	"radar_diner",
	"radar_emmetGun",
	"radar_enemyAttack",
	"radar_fire",
	"radar_girlfriend",
	"radar_hostpitaL",
	"radar_LocoSyndicate",
	"radar_MADDOG",
	"radar_mafiaCasino",
	"radar_MCSTRAP",
	"radar_modGarage",
	"radar_OGLOC",
	"radar_pizza",
	"radar_police",
	"radar_propertyG",
	"radar_propertyR",
	"radar_race",
	"radar_RYDER",
	"radar_saveGame",
	"radar_school",
	"radar_qmark",
	"radar_SWEET",
	"radar_tattoo",
	"radar_THETRUTH",
	"radar_waypoint",
	"radar_TorenoRanch"	,
	"radar_triads",
	"radar_triadsCasino",
	"radar_tshirt",
	"radar_WOOZIE",
	"radar_ZERO",
	"radar_dateDisco",
	"radar_dateDrink",
	"radar_dateFood",
	"radar_truck",
	"radar_cash",
	"radar_flag",
	"radar_gym",
	"radar_impound",
	"radar_light",
	"radar_runway",
	"radar_gangB",
	"radar_gangP",
	"radar_gangY",
	"radar_gangN",
	"radar_gangG",
	"radar_spray"
};

void CRadarNew::Initialise() {
	if (!ms_bSpritesLoaded) {

		ms_bSpritesLoaded = true;
	}
}

void CRadarNew::Shutdown() {
	if (ms_bSpritesLoaded) {

		ms_bSpritesLoaded = false;
	}
}

void CRadarNew::DrawRadarCircle() {
	if (CHudNew::GetGameMode() == GAMEMODE_III) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_VC) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_SA) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_LCS) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_VCS) {

	}
	else if (CHudNew::GetGameMode() == GAMEMODE_ADVANCE) {

	}
}

void CRadarNew::InjectPatches() {
#if GTASA
	//plugin::patch::Nop(0x58A818, 16);
	//plugin::patch::Nop(0x58A8C2, 16);
	//plugin::patch::Nop(0x58A96C, 16);
	//plugin::patch::Nop(0x58AA1A, 16); 
#endif
}
