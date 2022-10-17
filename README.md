[![CI Develop Debug](https://github.com/nepp95/VoxelEngine/actions/workflows/build-dev-debug.yml/badge.svg)](https://github.com/nepp95/VoxelEngine/actions/workflows/build-dev-debug.yml)
[![CI Develop Release](https://github.com/nepp95/VoxelEngine/actions/workflows/build-dev-release.yml/badge.svg)](https://github.com/nepp95/VoxelEngine/actions/workflows/build-dev-release.yml)
[![CI Master Debug](https://github.com/nepp95/VoxelEngine/actions/workflows/build-master-debug.yml/badge.svg)](https://github.com/nepp95/VoxelEngine/actions/workflows/build-master-debug.yml)
[![CI Master Release](https://github.com/nepp95/VoxelEngine/actions/workflows/build-master-release.yml/badge.svg)](https://github.com/nepp95/VoxelEngine/actions/workflows/build-master-release.yml)

# VoxelEngine

VoxelEngine is an engine built with C++ and OpenGL 4.6. It is a personal project in which I learn (a lot) more about graphics programming and programming in general. A lot of this is based on Hazel Engine ([Website](https://hazelengine.com/), [Github](https://github.com/TheCherno/Hazel)), but there is a lot that wasn't covered in Hazel.

Current features:
- Reference Counting*
- 3D Cube Rendering
- Asset Management*
- Scene Management
- Basic logging*
- Debug UI

Planned features:
- Sound
- World generation
- User UI

*explicitly done without a library to learn more about it.

## Installation

*Note: If you do not have git in your PATH, make sure you do so before trying below steps.*

1. `git clone https://github.com/nepp95/VoxelEngine.git`
2. Run `setup.bat` from the scripts folder. This will require you to have Visual Studio 2022 installed. If you have a different version of Visual Studio installed, please edit `Win-GenerateProjects.bat` and change `vs2022` to one of the options on [this page](https://premake.github.io/docs/Using-Premake)
3. After running `setup.bat`, you might need to run it again, based on what was installed on your computer when it first ran. You know it's all right when you see "Done".
4. Open the solution, build and run!