project "XVRenderer"
  kind "StaticLib"
  language "C++"
  cppdialect "C++latest"
  editandcontinue "Off"

  files
  {
    "Source/**.h",
    "Source/**.cpp",
    "Source/**.inl",
    "Include/**.h",
    "Include/**.cpp",
    "Include/**.inl",
  }

  includedirs
  {
    "Include/XVRenderer",
  }

  sysincludedirs
  {
    "Dep/XVUtilities/Include",
    "Dep/glfw-3.3.7/include",
    "Dep/glm/glm",
    "$(VULKAN_SDK)/include",
  }

  links 
  {
    "$(VULKAN_SDK)/lib/vulkan-1.lib",
    "Dep/glfw-3.3.7/lib-vc2022/glfw3_mt.lib",
  }

include "Dep/XVUtilities"