project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "vendor/stb/*.h",
        "vendor/stb/*.cpp"
    }

    includedirs {
        "src",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.yaml_cpp}"
    }

    links {
        "glad",
        "glfw",
        "imgui",
        "opengl32.lib",
        "yaml-cpp"
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