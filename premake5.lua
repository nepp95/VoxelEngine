include "Dependencies.lua"

workspace "VoxelEngine"
	architecture "x86_64"
	startproject "Editor"
	
	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	group "Dependencies"
	group ""

	group "Core"
		include "Engine"
		include "Editor"
	group ""
	
	-- group "Client"
	-- 	include "Sandbox"
	-- group ""