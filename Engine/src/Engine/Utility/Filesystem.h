#pragma once

#include <filesystem>

namespace VoxelEngine
{
    struct FilesystemData
    {
        std::filesystem::path RootPath;
        std::filesystem::path AssetPath;
        std::filesystem::path BinPath;
    }

    class Filesystem
    {
    public:
        static char* ReadBytes(const std::filesystem::path& filepath);
        static void WriteBytes(const std::filesystem::path& filepath, const char* bytes, bool overwrite = true);

    private:
        static FilesystemData s_data;    
    }
}