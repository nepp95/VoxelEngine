--workspace "Sandbox"
--	architecture "x86_64"
--	startproject "Sandbox"
--
--	configurations {
--		"Debug",
--		"Release",
--		"Dist"
--	}
--
--	flags {
--		"MultiProcessorCompile"
--	}

project "Sandbox"
	location ""
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files {
		"Source/**.cs",
		"Properties/**.cs"
	}

	links {
		"EpScriptCore"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
	
	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "EpEngine"
	include "../../../../EpScriptCore"
group ""