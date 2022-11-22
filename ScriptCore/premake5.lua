project "ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"

	targetdir("%{wks.location}/Editor/Resources/Scripts")
	objdir("%{wks.location}/Editor/Scripts/Intermediates")

	files {
		"Source/**.cs",
		"Properties/**.cs"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"