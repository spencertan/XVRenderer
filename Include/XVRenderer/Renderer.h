#pragma once

#include "VulkanInstance.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "Surface.h"
#include "Swapchain.h"
#include "Window.h"

#include <XVUtilities/Helper/NonCopyable.h>
#include <memory>

namespace XV
{

struct RendererConfig
{
  std::string application_name = "XVRenderer Application";
  bool enable_validation = true;
};

class Renderer : NonCopyable
{
  std::unique_ptr<VulkanInstance> m_instance;
  std::unique_ptr<Surface> m_surface;
  std::unique_ptr<PhysicalDevice> m_physical_device;
  std::unique_ptr<Device> m_device;
  std::unique_ptr<Swapchain> m_swapchain;

  Ptr<Window> m_window;

public:
  Renderer(Window& window, const RendererConfig& config = {});
  ~Renderer();

  // Move semantics
  Renderer(Renderer&& other) noexcept = default;
  Renderer& operator=(Renderer&& other) noexcept = default;

  // Accessors
  VulkanInstance& GetInstance() { return *m_instance; }
  PhysicalDevice& GetPhysicalDevice() { return *m_physical_device; }
  Device& GetDevice() { return *m_device; }
  Surface& GetSurface() { return *m_surface; }
  Swapchain& GetSwapchain() { return *m_swapchain; }

  const VulkanInstance& GetInstance() const { return *m_instance; }
  const PhysicalDevice& GetPhysicalDevice() const { return *m_physical_device; }
  const Device& GetDevice() const { return *m_device; }
  const Surface& GetSurface() const { return *m_surface; }
  const Swapchain& GetSwapchain() const { return *m_swapchain; }

  // Utility methods
  void WaitIdle() const { m_device->WaitIdle(); }
  void RecreateSwapchain();
};

}
