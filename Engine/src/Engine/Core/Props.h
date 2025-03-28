#pragma once

#include "Engine/Commons/Base.h"

#include <cstdint>
#include <string>
#include <filesystem>

namespace Engine
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            CORE_ASSERT(index < Count, "Command line Args Index out of range")
            return Args[index];
        }
    };

    struct WindowProps
    {
        std::string Title = "Application!!";
		uint32_t Width = 1280;
		uint32_t Height = 720;
        bool VSync = false;
    };

    struct ApplicationProps
    {
        std::string Name = "Application!!";
        std::filesystem::path AssetsDirectory = "Assets";
        ApplicationCommandLineArgs CommandLineArgs;
        WindowProps WindowProps;
    };
}