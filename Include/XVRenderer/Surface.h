#pragma once

#include <vulkan/vulkan.hpp>
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

struct GLFWwindow;

namespace XV
{

class Surface : NonCopyable
{
  vk::Instance m_instance;
  vk::SurfaceKHR m_surface;

public:
  Surface() = default;
  Surface(vk::Instance instance, Ptr<GLFWwindow> window);
  ~Surface();

  // Move semantics
  Surface(Surface&& other) noexcept;
  Surface& operator=(Surface&& other) noexcept;

  vk::SurfaceKHR GetSurface() const { return m_surface; }
  bool IsValid() const { return m_surface != nullptr; }
};

}
