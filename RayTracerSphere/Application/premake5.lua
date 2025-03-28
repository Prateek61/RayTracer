project "RayTracerSphereApplication"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/RayTracerSphere/Core/src",
        "%{wks.location}/Engine/src",
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
        "RayTracerSphereCore",
        "Engine",
        "GLFW",
        "ImGui",
        "Lua"
    }

    filter "system:windows"
        systemversion "latest"

        -- If build system is Visual Studio
        if _ACTION and string.startswith(_ACTION, "vs") then
            -- Enable the /utf-8 flag for Visual Studio
            buildoptions { "/utf-8" }
        else
            links
            {
                "%{Library.Windows.User32}",
                "%{Library.Windows.GDI32}",
                "%{Library.Windows.WinSock}"
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