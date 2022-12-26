-- VoxelEngine Dependencies

IncludeDir = {}
IncludeDir["box2d"] = "%{wks.location}/EpEngine/vendor/box2d/include"
IncludeDir["entt"] = "%{wks.location}/EpEngine/vendor/entt/single_include"
IncludeDir["glad"] = "%{wks.location}/EpEngine/vendor/glad/include"
IncludeDir["glfw"] = "%{wks.location}/EpEngine/vendor/glfw/include"
IncludeDir["glm"] = "%{wks.location}/EpEngine/vendor/glm"
IncludeDir["imgui"] = "%{wks.location}/EpEngine/vendor/imgui"
IncludeDir["mono"] = "%{wks.location}/EpEngine/vendor/mono/include"
IncludeDir["spdlog"] = "%{wks.location}/EpEngine/vendor/spdlog/include"
IncludeDir["stb"] = "%{wks.location}/EpEngine/vendor/stb"
IncludeDir["yaml_cpp"] = "%{wks.location}/EpEngine/vendor/yaml-cpp/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/EpEngine/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"