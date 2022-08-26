project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src"
    }

    filter "configurations:Debug"
        defines "V_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "V_RELEASE"
        runtime "Release"
        optimize "On"