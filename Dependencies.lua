-- VoxelEngine Dependencies

IncludeDir = {}
IncludeDir["entt"] = "%{wks.location}/Engine/vendor/entt/single_include"
IncludeDir["glad"] = "%{wks.location}/Engine/vendor/glad/include"
IncludeDir["glfw"] = "%{wks.location}/Engine/vendor/glfw/include"
IncludeDir["glm"] = "%{wks.location}/Engine/vendor/glm"
IncludeDir["imgui"] = "%{wks.location}/Engine/vendor/imgui"
IncludeDir["mono"] = "%{wks.location}/Engine/vendor/mono/include"
IncludeDir["stb"] = "%{wks.location}/Engine/vendor/stb"
IncludeDir["yaml_cpp"] = "%{wks.location}/Engine/vendor/yaml-cpp/include"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Engine/vendor/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"