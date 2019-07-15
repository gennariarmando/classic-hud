workspace "ClassicHUD"
	configurations { 
	"ReleaseIII",
	"ReleaseVC",
	"ReleaseSA",
	"DebugIII",
	"DebugVC",
	"DebugSA"
	}
	location "project_files"
	objdir ("output/obj")
	buildlog ("output/log/%{prj.name}.log")
	buildoptions "/std:c++latest"
	
	systemversion "latest"
	
	kind "SharedLib"
	language "C++"
	targetname "ClassicHUD"
	targetdir "output/asi"
	targetextension ".asi"
    characterset ("MBCS")
	linkoptions "/SAFESEH:NO"
	staticruntime "On"
	defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NON_CONFORMING_SWPRINTFS" }
    buildoptions { "/sdl-" }
    disablewarnings { "4073", "4221" }

	files { 
	"include/screen_addition/*.*",
	"source/*.*"
	}

	includedirs { 
	"include/screen_addition/",
	"$(PLUGIN_SDK_DIR)/shared/",
	"$(PLUGIN_SDK_DIR)/shared/game/",
	"source"
	}
	
	libdirs { 
	"$(PLUGIN_SDK_DIR)/output/lib/"
	}
	
	filter "DebugIII"
		symbols "On"
	filter "DebugVC"
		symbols "On"
	filter "DebugSA"
		symbols "On"

	filter "ReleaseIII"
		flags "LinkTimeOptimization"
		optimize "Off"
	filter "ReleaseVC"
		flags "LinkTimeOptimization"
		optimize "Off"
	filter "ReleaseSA"
		flags "LinkTimeOptimization"
		optimize "Off"
	filter {}
	  
project "ClassicHUD"
filter "configurations:DebugIII"
    defines { "GTA3", "PLUGIN_SGV_10EN" }
	links { "plugin_iii_d" }
	includedirs { 
	"$(PLUGIN_SDK_DIR)/plugin_iii/",
	"$(PLUGIN_SDK_DIR)/plugin_iii/game_iii/"
	}
	debugdir "$(GTA_III_DIR)"
	debugcommand "$(GTA_III_DIR)/gta3.exe"
	postbuildcommands "copy /y \"$(TargetPath)\" \"$(GTA_III_DIR)\\scripts\\ClassicHUD.asi\""
	
filter "configurations:DebugVC"
	defines { "GTAVC", "PLUGIN_SGV_10EN" }
	links { "plugin_vc_d" }
	includedirs {
	"$(PLUGIN_SDK_DIR)/plugin_vc/",
	"$(PLUGIN_SDK_DIR)/plugin_vc/game_vc/"
	}
	debugdir "$(GTA_VC_DIR)"
	debugcommand "$(GTA_VC_DIR)/gta-vc.exe"
	postbuildcommands "copy /y \"$(TargetPath)\" \"$(GTA_VC_DIR)\\scripts\\ClassicHUD.asi\""
	
filter "configurations:DebugSA"
	defines { "GTASA", "PLUGIN_SGV_10US" }
	links { "plugin_d" }
	includedirs {
	"$(PLUGIN_SDK_DIR)/plugin_sa/",
	"$(PLUGIN_SDK_DIR)/plugin_sa/game_sa/"
	}
	debugdir "$(GTA_SA_DIR)"
	debugcommand "$(GTA_SA_DIR)/gta-sa.exe"
	postbuildcommands "copy /y \"$(TargetPath)\" \"$(GTA_SA_DIR)\\scripts\\ClassicHUD.asi\""
	
filter "configurations:ReleaseIII"
	defines { "GTA3", "PLUGIN_SGV_10EN" }
	links { "plugin_iii" }
	includedirs { 
	"$(PLUGIN_SDK_DIR)/plugin_iii/",
	"$(PLUGIN_SDK_DIR)/plugin_iii/game_iii/"
	}
	
filter "configurations:ReleaseVC"
	defines { "GTAVC", "PLUGIN_SGV_10EN" }
	links { "plugin_vc" }
	includedirs {
	"$(PLUGIN_SDK_DIR)/plugin_vc/",
	"$(PLUGIN_SDK_DIR)/plugin_vc/game_vc/"
	}
	
filter "configurations:ReleaseSA"
	defines { "GTASA", "PLUGIN_SGV_10US" }
	links { "plugin" }
	includedirs {
	"$(PLUGIN_SDK_DIR)/plugin_sa/",
	"$(PLUGIN_SDK_DIR)/plugin_sa/game_sa/"
	}
