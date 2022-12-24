project "EpEngine"
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
        "vendor/stb/*.cpp",

        -- Yaml is a pain in the ass to compile and link, so we compile it directly into this library
        "vendor/yaml-cpp/src/**.cpp",
        "vendor/yaml-cpp/src/**.h",
        "vendor/yaml-cpp/include/**.h"
    }

    includedirs {
        "src",
        "%{IncludeDir.box2d}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glfw}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.mono}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.yaml_cpp}"
    }

    links {
        "%{Library.mono}",
        "box2d",
        "glad",
        "glfw",
        "imgui",
        "opengl32.lib"
    }

    filter "files:vendor/yaml-cpp/src/**.cpp"
        flags { "NoPCH" }

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