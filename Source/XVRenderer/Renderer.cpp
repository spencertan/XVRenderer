#include "Renderer.h"

#include <iostream>

namespace XV
{

Renderer::Renderer(Window& window, const RendererConfig& config)
  : m_window(&window)
{
  std::cout << "[Renderer] Initializing Vulkan renderer...\n";

  // Create Vulkan instance
  InstanceConfig instance_config;
  instance_config.application_name = config.application_name;
  instance_config.enable_validation_layers = config.enable_validation;
  instance_config.required_extensions = Window::GetRequiredInstanceExtensions();

  m_instance = std::make_unique<VulkanInstance>(instance_config);
  std::cout << "[Renderer] Vulkan instance created\n";

  // Create window surface
  m_surface = std::make_unique<Surface>(m_instance->GetInstance(), window.GetGLFWWindow());
  std::cout << "[Renderer] Window surface created\n";

  // Select physical device
  PhysicalDeviceRequirements physical_device_requirements;
  physical_device_requirements.required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  physical_device_requirements.require_discrete_gpu = false;

  m_physical_device = std::make_unique<PhysicalDevice>(
    m_instance->GetInstance(),
    m_surface->GetSurface(),
    physical_device_requirements
  );
  std::cout << "[Renderer] Physical device selected\n";

  // Create logical device
  DeviceConfig device_config;
  device_config.required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  m_device = std::make_unique<Device>(*m_physical_device, device_config);
  std::cout << "[Renderer] Logical device created\n";

  // Create swapchain
  SwapchainConfig swapchain_config;
  swapchain_config.width = window.GetWidth();
  swapchain_config.height = window.GetHeight();

  m_swapchain = std::make_unique<Swapchain>(
    *m_physical_device,
    *m_device,
    m_surface->GetSurface(),
    swapchain_config
  );
  std::cout << "[Renderer] Swapchain created ("
            << m_swapchain->GetExtent().width << "x"
            << m_swapchain->GetExtent().height << ")\n";

  std::cout << "[Renderer] Initialization complete!\n";
}

Renderer::~Renderer()
{
  if (m_device)
  {
    m_device->WaitIdle();
  }

  std::cout << "[Renderer] Shutting down...\n";
}

void Renderer::RecreateSwapchain()
{
  // Wait for device to finish current operations
  m_device->WaitIdle();

  // Recreate swapchain
  SwapchainConfig swapchain_config;
  swapchain_config.width = m_window->GetWidth();
  swapchain_config.height = m_window->GetHeight();

  m_swapchain = std::make_unique<Swapchain>(
    *m_physical_device,
    *m_device,
    m_surface->GetSurface(),
    swapchain_config
  );

  std::cout << "[Renderer] Swapchain recreated ("
            << m_swapchain->GetExtent().width << "x"
            << m_swapchain->GetExtent().height << ")\n";
}

}
