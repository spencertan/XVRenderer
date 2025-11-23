#include "PhysicalDevice.h"

#include <stdexcept>
#include <set>
#include <iostream>

namespace XV
{

PhysicalDevice::PhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface,
                               const PhysicalDeviceRequirements& requirements)
{
  auto devices = instance.enumeratePhysicalDevices();

  if (devices.empty())
  {
    throw std::runtime_error("[PhysicalDevice] Failed to find GPUs with Vulkan support!");
  }

  // Rate all devices and pick the best one
  u32 best_score = 0;
  vk::PhysicalDevice best_device;

  for (const auto& device : devices)
  {
    if (IsDeviceSuitable(device, surface, requirements))
    {
      u32 score = RateDeviceSuitability(device, requirements);
      if (score > best_score)
      {
        best_score = score;
        best_device = device;
      }
    }
  }

  if (!best_device)
  {
    throw std::runtime_error("[PhysicalDevice] Failed to find a suitable GPU!");
  }

  m_physical_device = best_device;
  m_properties = m_physical_device.getProperties();
  m_features = m_physical_device.getFeatures();
  m_queue_family_indices = FindQueueFamilies(m_physical_device, surface);

  std::cout << "[PhysicalDevice] Selected GPU: " << m_properties.deviceName << "\n";
}

QueueFamilyIndices PhysicalDevice::FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
  QueueFamilyIndices indices;
  auto queue_families = device.getQueueFamilyProperties();

  u32 i = 0;
  for (const auto& queue_family : queue_families)
  {
    // Graphics queue
    if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics)
    {
      indices.graphics_family = i;
    }

    // Present queue
    if (surface && device.getSurfaceSupportKHR(i, surface))
    {
      indices.present_family = i;
    }

    // Compute queue (prefer dedicated)
    if (queue_family.queueFlags & vk::QueueFlagBits::eCompute)
    {
      if (!indices.compute_family.has_value() ||
          !(queue_family.queueFlags & vk::QueueFlagBits::eGraphics))
      {
        indices.compute_family = i;
      }
    }

    // Transfer queue (prefer dedicated)
    if (queue_family.queueFlags & vk::QueueFlagBits::eTransfer)
    {
      if (!indices.transfer_family.has_value() ||
          (!(queue_family.queueFlags & vk::QueueFlagBits::eGraphics) &&
           !(queue_family.queueFlags & vk::QueueFlagBits::eCompute)))
      {
        indices.transfer_family = i;
      }
    }

    if (indices.IsComplete())
    {
      // Continue searching to find dedicated queues if available
    }

    ++i;
  }

  return indices;
}

bool PhysicalDevice::CheckDeviceExtensionSupport(vk::PhysicalDevice device,
                                                  const std::vector<const char*>& required_extensions)
{
  auto available_extensions = device.enumerateDeviceExtensionProperties();

  std::set<std::string> required_set(required_extensions.begin(), required_extensions.end());

  for (const auto& extension : available_extensions)
  {
    required_set.erase(extension.extensionName);
  }

  return required_set.empty();
}

bool PhysicalDevice::IsDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface,
                                       const PhysicalDeviceRequirements& requirements)
{
  auto properties = device.getProperties();
  auto features = device.getFeatures();

  // Check queue families
  auto indices = FindQueueFamilies(device, surface);
  if (!indices.IsComplete())
  {
    return false;
  }

  // Check extensions
  if (!CheckDeviceExtensionSupport(device, requirements.required_extensions))
  {
    return false;
  }

  // Check swapchain support (if surface is provided)
  if (surface)
  {
    auto formats = device.getSurfaceFormatsKHR(surface);
    auto present_modes = device.getSurfacePresentModesKHR(surface);

    if (formats.empty() || present_modes.empty())
    {
      return false;
    }
  }

  // Check discrete GPU requirement
  if (requirements.require_discrete_gpu &&
      properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
  {
    return false;
  }

  // Check geometry shader requirement
  if (requirements.require_geometry_shader && !features.geometryShader)
  {
    return false;
  }

  return true;
}

u32 PhysicalDevice::RateDeviceSuitability(vk::PhysicalDevice device,
                                          const PhysicalDeviceRequirements& requirements)
{
  auto properties = device.getProperties();
  auto features = device.getFeatures();

  u32 score = 0;

  // Discrete GPUs have a significant performance advantage
  if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
  {
    score += 1000;
  }

  // Maximum possible size of textures affects graphics quality
  score += properties.limits.maxImageDimension2D;

  // Prefer devices with geometry shader support
  if (features.geometryShader)
  {
    score += 100;
  }

  // Additional features can add to the score
  if (features.tessellationShader)
  {
    score += 50;
  }

  if (features.samplerAnisotropy)
  {
    score += 50;
  }

  return score;
}

}
