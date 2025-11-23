#pragma once

#include <vulkan/vulkan.hpp>
#include <XVUtilities/Types.h>

#include <vector>
#include <optional>

namespace XV
{

struct QueueFamilyIndices
{
  std::optional<u32> graphics_family;
  std::optional<u32> present_family;
  std::optional<u32> compute_family;
  std::optional<u32> transfer_family;

  bool IsComplete() const
  {
    return graphics_family.has_value() && present_family.has_value();
  }

  bool HasDedicatedComputeQueue() const
  {
    return compute_family.has_value() &&
           compute_family.value() != graphics_family.value();
  }

  bool HasDedicatedTransferQueue() const
  {
    return transfer_family.has_value() &&
           transfer_family.value() != graphics_family.value();
  }
};

struct PhysicalDeviceRequirements
{
  bool require_discrete_gpu = false;
  bool require_geometry_shader = true;
  std::vector<const char*> required_extensions;
};

class PhysicalDevice
{
  vk::PhysicalDevice m_physical_device;
  vk::PhysicalDeviceProperties m_properties;
  vk::PhysicalDeviceFeatures m_features;
  QueueFamilyIndices m_queue_family_indices;

  static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);
  static bool CheckDeviceExtensionSupport(vk::PhysicalDevice device, const std::vector<const char*>& required_extensions);
  static bool IsDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface, const PhysicalDeviceRequirements& requirements);
  static u32 RateDeviceSuitability(vk::PhysicalDevice device, const PhysicalDeviceRequirements& requirements);

public:
  PhysicalDevice() = default;
  PhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface, const PhysicalDeviceRequirements& requirements = {});

  vk::PhysicalDevice GetDevice() const { return m_physical_device; }
  const vk::PhysicalDeviceProperties& GetProperties() const { return m_properties; }
  const vk::PhysicalDeviceFeatures& GetFeatures() const { return m_features; }
  const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_queue_family_indices; }

  bool IsValid() const { return m_physical_device != nullptr; }
};

}
