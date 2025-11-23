#pragma once

#include "PhysicalDevice.h"
#include "Device.h"

#include <vulkan/vulkan.hpp>
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <vector>

namespace XV
{

struct SwapchainConfig
{
  u32 width = 800;
  u32 height = 600;
  vk::PresentModeKHR preferred_present_mode = vk::PresentModeKHR::eMailbox;
  vk::SurfaceFormatKHR preferred_format = {
    vk::Format::eB8G8R8A8Srgb,
    vk::ColorSpaceKHR::eSrgbNonlinear
  };
  u32 image_count = 3; // Triple buffering by default
};

class Swapchain : NonCopyable
{
  vk::Device m_device;
  vk::SwapchainKHR m_swapchain;

  std::vector<vk::Image> m_images;
  std::vector<vk::ImageView> m_image_views;

  vk::Format m_image_format;
  vk::Extent2D m_extent;

  vk::SurfaceFormatKHR ChooseSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR>& available_formats,
    const vk::SurfaceFormatKHR& preferred) const;

  vk::PresentModeKHR ChoosePresentMode(
    const std::vector<vk::PresentModeKHR>& available_modes,
    vk::PresentModeKHR preferred) const;

  vk::Extent2D ChooseExtent(
    const vk::SurfaceCapabilitiesKHR& capabilities,
    u32 width, u32 height) const;

  void CreateImageViews();

public:
  Swapchain() = default;
  Swapchain(const PhysicalDevice& physical_device, const Device& device,
            vk::SurfaceKHR surface, const SwapchainConfig& config = {});
  ~Swapchain();

  // Move semantics
  Swapchain(Swapchain&& other) noexcept;
  Swapchain& operator=(Swapchain&& other) noexcept;

  vk::SwapchainKHR GetSwapchain() const { return m_swapchain; }
  vk::Format GetImageFormat() const { return m_image_format; }
  vk::Extent2D GetExtent() const { return m_extent; }
  const std::vector<vk::Image>& GetImages() const { return m_images; }
  const std::vector<vk::ImageView>& GetImageViews() const { return m_image_views; }

  u32 GetImageCount() const { return static_cast<u32>(m_images.size()); }
};

}
