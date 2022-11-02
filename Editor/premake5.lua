project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "%{wks.location}/Engine/src",
        "%{wks.location}/Engine/vendor",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.yaml_cpp}"
    }

    links {
        "Engine"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
		defines "V_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "V_RELEASE"
		runtime "Release"
		optimize "On"