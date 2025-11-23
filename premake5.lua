workspace "XVRenderer"
  architecture "x64"
  configurations { "Debug", "Release" }
  startproject "ColoredCubeExample"

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

  filter {}

-- XVRenderer Library
project "XVRenderer"
  kind "StaticLib"
  language "C++"
  cppdialect "C++latest"
  editandcontinue "Off"

  targetdir "build/%{cfg.buildcfg}/lib"
  objdir "build/%{cfg.buildcfg}/obj/%{prj.name}"

  files
  {
    "Source/**.h",
    "Source/**.cpp",
    "Source/**.inl",
    "Include/**.h",
    "Include/**.cpp",
    "Include/**.inl",
    "Dep/glad/src/glad.c",  -- GLAD implementation
  }

  includedirs
  {
    "Include",
    "Include/XVRenderer",
  }

  sysincludedirs
  {
    "Dep/XVUtilities/Include",
    "Dep/glfw-3.3.7/include",
    "Dep/glm/glm",
    "Dep/glad/include",  -- GLAD headers
    "$(VULKAN_SDK)/include",
  }

  links
  {
    "$(VULKAN_SDK)/lib/vulkan-1.lib",
    "Dep/glfw-3.3.7/lib-vc2022/glfw3_mt.lib",
  }

  -- Platform-specific OpenGL linking
  filter "system:windows"
    links { "opengl32" }

  filter "system:linux"
    links { "GL" }

  filter "system:macosx"
    links { "OpenGL.framework" }

  filter {}

-- Colored Cube Example
project "ColoredCubeExample"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++latest"

  targetdir "build/%{cfg.buildcfg}/bin"
  objdir "build/%{cfg.buildcfg}/obj/%{prj.name}"

  files
  {
    "Examples/ColoredCube.cpp"
  }

  includedirs
  {
    "Include",
  }

  sysincludedirs
  {
    "Dep/XVUtilities/Include",
    "Dep/glfw-3.3.7/include",
    "Dep/glm/glm",
    "Dep/glad/include",
    "$(VULKAN_SDK)/include",
  }

  links
  {
    "XVRenderer",
  }

  -- Make sure library is built first
  dependson { "XVRenderer" }

  -- Link library path
  libdirs
  {
    "build/%{cfg.buildcfg}/lib"
  }

include "Dep/XVUtilities"
