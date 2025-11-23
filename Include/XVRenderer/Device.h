#pragma once

#include "PhysicalDevice.h"

#include <vulkan/vulkan.hpp>
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <vector>

namespace XV
{

struct DeviceConfig
{
  std::vector<const char*> required_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  vk::PhysicalDeviceFeatures enabled_features{};
};

class Device : NonCopyable
{
  vk::Device m_device;
  vk::Queue m_graphics_queue;
  vk::Queue m_present_queue;
  vk::Queue m_compute_queue;
  vk::Queue m_transfer_queue;

  QueueFamilyIndices m_queue_family_indices;

public:
  Device(const PhysicalDevice& physical_device, const DeviceConfig& config = {});
  ~Device();

  // Move semantics
  Device(Device&& other) noexcept;
  Device& operator=(Device&& other) noexcept;

  vk::Device GetDevice() const { return m_device; }
  vk::Queue GetGraphicsQueue() const { return m_graphics_queue; }
  vk::Queue GetPresentQueue() const { return m_present_queue; }
  vk::Queue GetComputeQueue() const { return m_compute_queue; }
  vk::Queue GetTransferQueue() const { return m_transfer_queue; }

  const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_queue_family_indices; }

  void WaitIdle() const { m_device.waitIdle(); }
};

}
