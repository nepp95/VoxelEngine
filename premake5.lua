include "Dependencies.lua"

workspace "VoxelEngine"
	architecture "x86_64"
	startproject "Editor"
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}
	
	group "Dependencies"
		include "Engine/vendor/glad" -- OpenGL abstraction
		include "Engine/vendor/glfw" -- Window management
		include "Engine/vendor/imgui" -- Graphical user interface]
	group ""

	group "Core"
		include "Engine"
		include "Editor"
	group ""
	
	-- group "Client"
	-- 	include "Sandbox"
	-- group ""