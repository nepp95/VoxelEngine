#pragma once

#include <filesystem>

namespace EpEngine
{
    class Filesystem
    {
    public:
        static void Init();
        static void Shutdown();

        static char* ReadBytes(const std::filesystem::path& filepath);
        static void WriteBytes(const std::filesystem::path& filepath, const char* bytes, bool overwrite = true);
    };
}