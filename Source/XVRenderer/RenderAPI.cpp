#include "RenderAPI.h"

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include <iostream>

namespace XV
{

RenderAPI RenderAPIInfo::DetectBestAPI()
{
  // Try Vulkan first
  if (IsAPISupported(RenderAPI::Vulkan))
  {
    std::cout << "[RenderAPI] Vulkan is supported\n";
    return RenderAPI::Vulkan;
  }

  // Fallback to OpenGL
  if (IsAPISupported(RenderAPI::OpenGL))
  {
    std::cout << "[RenderAPI] Falling back to OpenGL\n";
    return RenderAPI::OpenGL;
  }

  std::cerr << "[RenderAPI] No supported rendering API found!\n";
  return RenderAPI::None;
}

bool RenderAPIInfo::IsAPISupported(RenderAPI api)
{
  switch (api)
  {
    case RenderAPI::Vulkan:
    {
      // Check if Vulkan is available
      return glfwVulkanSupported() == GLFW_TRUE;
    }

    case RenderAPI::OpenGL:
    {
      // OpenGL is generally always available on desktop platforms
      // A more thorough check would try to create a context
      return true;
    }

    case RenderAPI::None:
    default:
      return false;
  }
}

const char* RenderAPIInfo::GetAPIName(RenderAPI api)
{
  switch (api)
  {
    case RenderAPI::Vulkan: return "Vulkan";
    case RenderAPI::OpenGL: return "OpenGL";
    case RenderAPI::None:   return "None";
    default:                return "Unknown";
  }
}

}
