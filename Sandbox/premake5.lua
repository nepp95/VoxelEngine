project "Sandbox"
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
        "%{wks.location}/EpEngine/src",
        "%{wks.location}/EpEngine/vendor",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.yaml_cpp}"
    }

    links {
        "EpEngine"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
		defines "EP_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "EP_RELEASE"
		runtime "Release"
		optimize "On"

  	filter "configurations:Dist"
    	defines "EP_DIST"
		runtime "Release"
		optimize "On"