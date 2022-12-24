include "Dependencies.lua"

workspace "EpEngine"
	architecture "x86_64"
	startproject "Editor"
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}
	
	group "Dependencies"
		include "EpEngine/vendor/box2d" -- Physics
		include "EpEngine/vendor/glad" -- OpenGL abstraction
		include "EpEngine/vendor/glfw" -- Window management
		include "EpEngine/vendor/imgui" -- UI
	group ""

	group "Core"
		include "EpEngine"
		include "EpScriptCore"
	group ""

	group "Tools"
		include "EpEditor"
	group ""

	group "Client"
		include "Sandbox"
	group ""