-- Premake 5 build configuration script to compile Lua as a static library.
project "Lua"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "*.h",
        "*.c",
    }
    excludes
    {
        "onelua.c"
    }

    includedirs
    {
        "*"
    }

    filter "system:windows"
        systemversion "latest"

        filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
    
    filter {} -- Reset filter