include "Dependencies.lua"

workspace "RayTracer"
    startproject "SandBox"
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
    include "RayTracerSphere/Core"
group ""

group "App"
    include "SandBox"
    include "RayTracerSphere/Application"
group ""