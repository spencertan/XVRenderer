#include "Device.h"

#include <stdexcept>
#include <set>

namespace XV
{

Device::Device(const PhysicalDevice& physical_device, const DeviceConfig& config)
{
  m_queue_family_indices = physical_device.GetQueueFamilyIndices();

  // Create queue create infos
  std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
  std::set<u32> unique_queue_families = {
    m_queue_family_indices.graphics_family.value(),
    m_queue_family_indices.present_family.value()
  };

  if (m_queue_family_indices.compute_family.has_value())
  {
    unique_queue_families.insert(m_queue_family_indices.compute_family.value());
  }

  if (m_queue_family_indices.transfer_family.has_value())
  {
    unique_queue_families.insert(m_queue_family_indices.transfer_family.value());
  }

  float queue_priority = 1.0f;
  for (u32 queue_family : unique_queue_families)
  {
    vk::DeviceQueueCreateInfo queue_create_info{
      {},
      queue_family,
      1,
      &queue_priority
    };
    queue_create_infos.push_back(queue_create_info);
  }

  // Device features
  vk::PhysicalDeviceFeatures device_features = config.enabled_features;

  // Create logical device
  vk::DeviceCreateInfo create_info{
    {},
    static_cast<u32>(queue_create_infos.size()),
    queue_create_infos.data(),
    0,
    nullptr, // Validation layers are now instance-level only
    static_cast<u32>(config.required_extensions.size()),
    config.required_extensions.data(),
    &device_features
  };

  try
  {
    m_device = physical_device.GetDevice().createDevice(create_info);
  }
  catch (const vk::SystemError& e)
  {
    throw std::runtime_error("[Device] Failed to create logical device: " + std::string(e.what()));
  }

  // Retrieve queue handles
  m_graphics_queue = m_device.getQueue(m_queue_family_indices.graphics_family.value(), 0);
  m_present_queue = m_device.getQueue(m_queue_family_indices.present_family.value(), 0);

  if (m_queue_family_indices.compute_family.has_value())
  {
    m_compute_queue = m_device.getQueue(m_queue_family_indices.compute_family.value(), 0);
  }

  if (m_queue_family_indices.transfer_family.has_value())
  {
    m_transfer_queue = m_device.getQueue(m_queue_family_indices.transfer_family.value(), 0);
  }
}

Device::~Device()
{
  if (m_device)
  {
    m_device.destroy();
  }
}

Device::Device(Device&& other) noexcept
  : m_device(other.m_device)
  , m_graphics_queue(other.m_graphics_queue)
  , m_present_queue(other.m_present_queue)
  , m_compute_queue(other.m_compute_queue)
  , m_transfer_queue(other.m_transfer_queue)
  , m_queue_family_indices(other.m_queue_family_indices)
{
  other.m_device = nullptr;
  other.m_graphics_queue = nullptr;
  other.m_present_queue = nullptr;
  other.m_compute_queue = nullptr;
  other.m_transfer_queue = nullptr;
}

Device& Device::operator=(Device&& other) noexcept
{
  if (this != &other)
  {
    // Clean up existing resources
    if (m_device)
    {
      m_device.destroy();
    }

    // Move from other
    m_device = other.m_device;
    m_graphics_queue = other.m_graphics_queue;
    m_present_queue = other.m_present_queue;
    m_compute_queue = other.m_compute_queue;
    m_transfer_queue = other.m_transfer_queue;
    m_queue_family_indices = other.m_queue_family_indices;

    other.m_device = nullptr;
    other.m_graphics_queue = nullptr;
    other.m_present_queue = nullptr;
    other.m_compute_queue = nullptr;
    other.m_transfer_queue = nullptr;
  }
  return *this;
}

}
