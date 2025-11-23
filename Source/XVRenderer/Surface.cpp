#include "Surface.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <stdexcept>

namespace XV
{

Surface::Surface(vk::Instance instance, Ptr<GLFWwindow> window)
  : m_instance(instance)
{
  VkSurfaceKHR surface_c;
  VkResult result = glfwCreateWindowSurface(
    static_cast<VkInstance>(instance),
    window,
    nullptr,
    &surface_c
  );

  if (result != VK_SUCCESS)
  {
    throw std::runtime_error("[Surface] Failed to create window surface!");
  }

  m_surface = vk::SurfaceKHR(surface_c);
}

Surface::~Surface()
{
  if (m_surface && m_instance)
  {
    m_instance.destroySurfaceKHR(m_surface);
  }
}

Surface::Surface(Surface&& other) noexcept
  : m_instance(other.m_instance)
  , m_surface(other.m_surface)
{
  other.m_instance = nullptr;
  other.m_surface = nullptr;
}

Surface& Surface::operator=(Surface&& other) noexcept
{
  if (this != &other)
  {
    // Clean up existing resources
    if (m_surface && m_instance)
    {
      m_instance.destroySurfaceKHR(m_surface);
    }

    // Move from other
    m_instance = other.m_instance;
    m_surface = other.m_surface;

    other.m_instance = nullptr;
    other.m_surface = nullptr;
  }
  return *this;
}

}
