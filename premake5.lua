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
		include "Engine/vendor/glad"
		include "Engine/vendor/glfw"
		include "Engine/vendor/imgui"
		include "Engine/vendor/yaml-cpp"
	group ""

	group "Core"
		include "Engine"
		include "Editor"
	group ""
	
	-- group "Client"
	-- 	include "Sandbox"
	-- group ""