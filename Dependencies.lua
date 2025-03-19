-- Project Dependencies

IncludeDir = {}
IncludeDir["SPDLOG"] = "%{wks.location}/Deps/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Deps/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Deps/GLAD/include"
IncludeDir["GLM"] = "%{wks.location}/Deps/GLM"
IncludeDir["IMGUI"] = "%{wks.location}/Deps/imgui"
IncludeDir["STB_IMAGE"] = "%{wks.location}/Deps/stb_image"
IncludeDir["LUA"] = "%{wks.location}/Deps/lua"
IncludeDir["SOL"] = "%{wks.location}/Deps/sol/include"
IncludeDir["FILEWATCH"] = "%{wks.location}/Deps/filewatch"
IncludeDir["IMGUIZMO"] = "%{wks.location}/Deps/ImGuizmo"

Library = {}

-- Windows
Library["Windows"] = {}
Library["Windows"]["WinSockLib"] = "Ws2_32.lib"
Library["Windows"]["WinMMLib"] = "Winmm.lib"
Library["Windows"]["WinVersionLib"] = "version.lib"
Library["Windows"]["BCryptLib"] = "Bcrypt.lib"
Library["Windows"]["OpenGL32Lib"] = "opengl32.lib"
Library["Windows"]["GDI32"] = "gdi32"
Library["Windows"]["User32"] = "user32"
Library["Windows"]["WinSock"] = "Ws2_32"

