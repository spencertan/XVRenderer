#include "Swapchain.h"

#include <stdexcept>
#include <algorithm>
#include <limits>

namespace XV
{

Swapchain::Swapchain(const PhysicalDevice& physical_device, const Device& device,
                     vk::SurfaceKHR surface, const SwapchainConfig& config)
  : m_device(device.GetDevice())
{
  auto capabilities = physical_device.GetDevice().getSurfaceCapabilitiesKHR(surface);
  auto formats = physical_device.GetDevice().getSurfaceFormatsKHR(surface);
  auto present_modes = physical_device.GetDevice().getSurfacePresentModesKHR(surface);

  // Choose swapchain settings
  auto surface_format = ChooseSurfaceFormat(formats, config.preferred_format);
  auto present_mode = ChoosePresentMode(present_modes, config.preferred_present_mode);
  auto extent = ChooseExtent(capabilities, config.width, config.height);

  // Determine image count
  u32 image_count = config.image_count;
  if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount)
  {
    image_count = capabilities.maxImageCount;
  }
  if (image_count < capabilities.minImageCount)
  {
    image_count = capabilities.minImageCount;
  }

  // Create swapchain
  vk::SwapchainCreateInfoKHR create_info{
    {},
    surface,
    image_count,
    surface_format.format,
    surface_format.colorSpace,
    extent,
    1, // imageArrayLayers
    vk::ImageUsageFlagBits::eColorAttachment
  };

  auto indices = device.GetQueueFamilyIndices();
  u32 queue_family_indices[] = {
    indices.graphics_family.value(),
    indices.present_family.value()
  };

  if (indices.graphics_family != indices.present_family)
  {
    create_info.imageSharingMode = vk::SharingMode::eConcurrent;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  }
  else
  {
    create_info.imageSharingMode = vk::SharingMode::eExclusive;
  }

  create_info.preTransform = capabilities.currentTransform;
  create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = nullptr;

  try
  {
    m_swapchain = m_device.createSwapchainKHR(create_info);
  }
  catch (const vk::SystemError& e)
  {
    throw std::runtime_error("[Swapchain] Failed to create swapchain: " + std::string(e.what()));
  }

  // Retrieve swapchain images
  m_images = m_device.getSwapchainImagesKHR(m_swapchain);
  m_image_format = surface_format.format;
  m_extent = extent;

  // Create image views
  CreateImageViews();
}

Swapchain::~Swapchain()
{
  for (auto image_view : m_image_views)
  {
    m_device.destroyImageView(image_view);
  }

  if (m_swapchain)
  {
    m_device.destroySwapchainKHR(m_swapchain);
  }
}

Swapchain::Swapchain(Swapchain&& other) noexcept
  : m_device(other.m_device)
  , m_swapchain(other.m_swapchain)
  , m_images(std::move(other.m_images))
  , m_image_views(std::move(other.m_image_views))
  , m_image_format(other.m_image_format)
  , m_extent(other.m_extent)
{
  other.m_device = nullptr;
  other.m_swapchain = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& other) noexcept
{
  if (this != &other)
  {
    // Clean up existing resources
    for (auto image_view : m_image_views)
    {
      m_device.destroyImageView(image_view);
    }
    if (m_swapchain)
    {
      m_device.destroySwapchainKHR(m_swapchain);
    }

    // Move from other
    m_device = other.m_device;
    m_swapchain = other.m_swapchain;
    m_images = std::move(other.m_images);
    m_image_views = std::move(other.m_image_views);
    m_image_format = other.m_image_format;
    m_extent = other.m_extent;

    other.m_device = nullptr;
    other.m_swapchain = nullptr;
  }
  return *this;
}

vk::SurfaceFormatKHR Swapchain::ChooseSurfaceFormat(
  const std::vector<vk::SurfaceFormatKHR>& available_formats,
  const vk::SurfaceFormatKHR& preferred) const
{
  // Check if preferred format is available
  for (const auto& format : available_formats)
  {
    if (format.format == preferred.format && format.colorSpace == preferred.colorSpace)
    {
      return format;
    }
  }

  // Fallback to first available format
  return available_formats[0];
}

vk::PresentModeKHR Swapchain::ChoosePresentMode(
  const std::vector<vk::PresentModeKHR>& available_modes,
  vk::PresentModeKHR preferred) const
{
  // Check if preferred mode is available
  for (const auto& mode : available_modes)
  {
    if (mode == preferred)
    {
      return mode;
    }
  }

  // FIFO is guaranteed to be available
  return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Swapchain::ChooseExtent(
  const vk::SurfaceCapabilitiesKHR& capabilities,
  u32 width, u32 height) const
{
  if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
  {
    return capabilities.currentExtent;
  }
  else
  {
    vk::Extent2D actual_extent = { width, height };

    actual_extent.width = std::clamp(
      actual_extent.width,
      capabilities.minImageExtent.width,
      capabilities.maxImageExtent.width
    );
    actual_extent.height = std::clamp(
      actual_extent.height,
      capabilities.minImageExtent.height,
      capabilities.maxImageExtent.height
    );

    return actual_extent;
  }
}

void Swapchain::CreateImageViews()
{
  m_image_views.resize(m_images.size());

  for (size_t i = 0; i < m_images.size(); ++i)
  {
    vk::ImageViewCreateInfo create_info{
      {},
      m_images[i],
      vk::ImageViewType::e2D,
      m_image_format,
      vk::ComponentMapping{},
      vk::ImageSubresourceRange{
        vk::ImageAspectFlagBits::eColor,
        0, 1, 0, 1
      }
    };

    m_image_views[i] = m_device.createImageView(create_info);
  }
}

}
