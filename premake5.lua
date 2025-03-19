include "Dependencies.lua"

workspace "OpenGL"
    startproject "App"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Deps/GLAD"
    include "Deps/GLFW"
    include "Deps/imgui"
    include "Deps/lua"
group ""

group "Core"
    include "Engine"
group ""

group "App"
    include "App"
group ""