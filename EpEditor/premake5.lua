project "EpEditor"
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
		"%{IncludeDir.filewatch}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.spdlog}"
    }

    links {
        "EpEngine"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
    }

    dependson {
        "EpScriptCore"
    }

    filter "system:windows"
		systemversion "latest"

    filter "configurations:Debug"
		defines "EP_DEBUG"
		runtime "Debug"
		symbols "On"

        postbuildcommands {
		    '{COPY} "../EpEngine/vendor/mono/bin/Debug/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
	    }

	filter "configurations:Release"
		defines "EP_RELEASE"
		runtime "Release"
		optimize "On"

        postbuildcommands {
		    '{COPY} "../EpEngine/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
	    }

    filter "configurations:Dist"
        defines "EP_DIST"
        runtime "Release"
        optimize "On"

        postbuildcommands {
		    '{COPY} "../EpEngine/vendor/mono/bin/Release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
	    }
