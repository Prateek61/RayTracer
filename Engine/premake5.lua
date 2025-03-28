project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/int/" .. outputdir .. "/%{prj.name}")

    pchheader "PCH.h"
    pchsource "src/PCH.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        -- ImGUIZMO
        "%{IncludeDir.IMGUIZMO}/ImGuizmo.cpp",
        "%{IncludeDir.IMGUIZMO}/ImGuizmo.h",
        -- STB Image
        "%{IncludeDir.STB_IMAGE}/stb_image.cpp",
        "%{IncludeDir.STB_IMAGE}/stb_image.h",
        "%{IncludeDir.STB_IMAGE}/stb_image_write.h"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src", -- Include the root directory
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.LUA}",
        "%{IncludeDir.STB_IMAGE}",
        "%{IncludeDir.SOL}",
        "%{IncludeDir.FILEWATCH}",
        "%{IncludeDir.IMGUIZMO}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "Lua"
    }

    filter "system:windows"
        systemversion "latest"

        links
        {
            "%{Library.Windows.OpenGL32Lib}"
        }

        -- If build system is Visual Studio
        if _ACTION and string.startswith(_ACTION, "vs") then
            links
            {
                "%{Library.Windows.WinSockLib}",
                "%{Library.Windows.WinMMLib}",
                "%{Library.Windows.WinVersionLib}",
                "%{Library.Windows.BCryptLib}"
            }

            -- Enable the /utf-8 flag for Visual Studio
            buildoptions { "/utf-8" }
        else
            links
            {
                "%{Library.Windows.User32}",
                "%{Library.Windows.GDI32}"
            }
        end


    filter "configurations:Debug"
        defines "BUILD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BUILD_RELEASE"
        optimize "on"
        symbols "on"

    filter "configurations:Dist"
        defines "BUILD_DIST"
        runtime "Release"
        optimize "on"
        symbols "off"

    -- Clear the filter
    filter {}