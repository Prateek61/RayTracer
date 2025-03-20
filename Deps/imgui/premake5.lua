project "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "./*.h",
        "./*.cpp",
        "misc/cpp/imgui_stdlib.h",
        "misc/cpp/imgui_stdlib.cpp"

    }

    includedirs
    {
        "."
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        -- staticruntime "On"

    filter "system:linux"
        systemversion "latest"
        cppdialect "C++17"
        -- staticruntime "On"
    
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