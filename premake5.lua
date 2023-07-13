workspace "ShadowEngine"
	architecture "x64"
	startproject "Test"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "%{wks.location}/%{cfg.architecture}/%{cfg.longname}"

	IncludeDir = {}
	IncludeDir["GLM"] = "deps/GLM"
	IncludeDir["GLEW"] = "deps/GLEW/include"
	IncludeDir["GLFW"] = "deps/GLFW/include"
	IncludeDir["SOIL"] = "deps/SOIL/include"
	IncludeDir["Assimp"] = "deps/Assimp/include"
	IncludeDir["ColorConsole"] = "deps/ColorConsole/include"
	IncludeDir["ReactPhysics3D"] = "deps/ReactPhysics3D/include"
	IncludeDir["ImGUI"] = "deps/ImGUI/include"

	project "ShadowEngine"
		location "ShadowEngine"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"

		targetdir ("%{wks.location}/%{cfg.architecture}/%{cfg.longname}")
		objdir ("%{wks.location}/%{cfg.architecture}/%{cfg.longname}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
		}

		defines
		{
			"GLEW_STATIC"
		}

		includedirs
		{
			"%{IncludeDir.GLM}",
			"%{IncludeDir.GLEW}",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.SOIL}",
			"%{IncludeDir.Assimp}",
			"%{IncludeDir.ColorConsole}",
			"%{IncludeDir.ReactPhysics3D}",
			"%{IncludeDir.ImGUI}",
			"%{prj.name}/src"
		}

		syslibdirs
		{
			"deps/GLEW/lib",
			"deps/GLFW/lib",
			"deps/SOIL/lib",
			"deps/Assimp/lib",
			"deps/ReactPhysics3D/lib",
			"deps/ImGUI/lib"
		}

		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			links 
			{ 
				"assimp.lib",
				"glew32s.lib",
				"glfw3.lib",
				"SOIL.lib",
				"reactphysics3dDebug.lib",
				"ImGuiDebug.lib",
				"opengl32.lib"
			}

		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			links 
			{ 
				"assimp.lib",
				"glew32s.lib",
				"glfw3.lib",
				"SOIL.lib",
				"reactphysics3dRelease.lib",
				"ImGuiDebug.lib",
				"opengl32.lib"
			}

	project "Test"
		location "Test"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "off"
		
		dependson { "ShadowEngine" }

		targetdir ("%{wks.location}/%{cfg.architecture}/%{cfg.longname}")
		objdir ("%{wks.location}/%{cfg.architecture}/%{cfg.longname}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}
		
		defines
		{
			"GLEW_STATIC"
		}

		includedirs
		{
			"%{IncludeDir.GLM}",
			"%{IncludeDir.GLEW}",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.SOIL}",
			"%{IncludeDir.Assimp}",
			"%{IncludeDir.ColorConsole}",
			"%{IncludeDir.ReactPhysics3D}",
			"%{IncludeDir.ImGUI}",
			"ShadowEngine/src"
		}

		syslibdirs
		{
			"deps/GLEW/lib",
			"deps/GLFW/lib",
			"deps/SOIL/lib",
			"deps/Assimp/lib",
			"deps/ReactPhysics3D/lib",
			"deps/ImGUI/lib",
			"%{wks.location}/%{cfg.architecture}/%{cfg.longname}"
		}
		
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"
			links
			{
				"assimp.lib",
				"glew32s.lib",
				"glfw3.lib",
				"SOIL.lib",
				"ShadowEngine.lib",
				"reactphysics3dDebug.lib",
				"ImGuiDebug.lib",
				"opengl32.lib"
			}

		filter "configurations:Release"
			runtime "Release"
			optimize "on"
			links
			{
				"assimp.lib",
				"glew32s.lib",
				"glfw3.lib",
				"SOIL.lib",
				"ShadowEngine.lib",
				"reactphysics3dRelease.lib",
				"ImGuiRelease.lib",
				"opengl32.lib"
			}