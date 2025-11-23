#include "VulkanInstance.h"

#include <iostream>
#include <stdexcept>
#include <cstring>

namespace XV
{

VulkanInstance::VulkanInstance(const InstanceConfig& config)
  : m_validation_enabled(false)
{
  // Check validation layer support
#ifdef XV_DEBUG
  if (config.enable_validation_layers && !CheckValidationLayerSupport())
  {
    std::cerr << "[VulkanInstance] Validation layers requested but not available. Continuing without validation.\n";
  }
  else if (config.enable_validation_layers)
  {
    m_validation_enabled = true;
  }
#endif

  // Application info
  vk::ApplicationInfo app_info{
    config.application_name.c_str(),
    config.application_version,
    config.engine_name.c_str(),
    config.engine_version,
    config.api_version
  };

  // Get required extensions
  auto extensions = GetRequiredExtensions(config.required_extensions);

  // Setup layers
  std::vector<const char*> layers = config.required_layers;
  if (m_validation_enabled)
  {
    layers.push_back(s_validation_layer_name);
  }

  // Create instance
  vk::InstanceCreateInfo create_info{
    {},
    &app_info,
    static_cast<u32>(layers.size()),
    layers.data(),
    static_cast<u32>(extensions.size()),
    extensions.data()
  };

  // Debug messenger for instance creation/destruction
  vk::DebugUtilsMessengerCreateInfoEXT debug_create_info;
  if (m_validation_enabled)
  {
    debug_create_info.messageSeverity =
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    debug_create_info.messageType =
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    debug_create_info.pfnUserCallback = DebugCallback;
    create_info.pNext = &debug_create_info;
  }

  try
  {
    m_instance = vk::createInstance(create_info);
  }
  catch (const vk::SystemError& e)
  {
    throw std::runtime_error("[VulkanInstance] Failed to create Vulkan instance: " + std::string(e.what()));
  }

  // Setup debug messenger for the instance
  if (m_validation_enabled)
  {
    SetupDebugMessenger();
  }
}

VulkanInstance::~VulkanInstance()
{
  if (m_debug_messenger)
  {
    m_instance.destroyDebugUtilsMessengerEXT(m_debug_messenger);
  }

  if (m_instance)
  {
    m_instance.destroy();
  }
}

VulkanInstance::VulkanInstance(VulkanInstance&& other) noexcept
  : m_instance(other.m_instance)
  , m_debug_messenger(other.m_debug_messenger)
  , m_validation_enabled(other.m_validation_enabled)
{
  other.m_instance = nullptr;
  other.m_debug_messenger = nullptr;
  other.m_validation_enabled = false;
}

VulkanInstance& VulkanInstance::operator=(VulkanInstance&& other) noexcept
{
  if (this != &other)
  {
    // Clean up existing resources
    if (m_debug_messenger)
    {
      m_instance.destroyDebugUtilsMessengerEXT(m_debug_messenger);
    }
    if (m_instance)
    {
      m_instance.destroy();
    }

    // Move from other
    m_instance = other.m_instance;
    m_debug_messenger = other.m_debug_messenger;
    m_validation_enabled = other.m_validation_enabled;

    other.m_instance = nullptr;
    other.m_debug_messenger = nullptr;
    other.m_validation_enabled = false;
  }
  return *this;
}

bool VulkanInstance::CheckValidationLayerSupport() const
{
  auto available_layers = vk::enumerateInstanceLayerProperties();

  for (const auto& layer : available_layers)
  {
    if (std::strcmp(s_validation_layer_name, layer.layerName) == 0)
    {
      return true;
    }
  }

  return false;
}

std::vector<const char*> VulkanInstance::GetRequiredExtensions(
  const std::vector<const char*>& additional_extensions) const
{
  std::vector<const char*> extensions = additional_extensions;

  // Add debug extension if validation is enabled
  if (m_validation_enabled)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // Platform-specific extensions are typically provided by GLFW
  // via glfwGetRequiredInstanceExtensions, which the user should pass
  // in the config.required_extensions

  return extensions;
}

void VulkanInstance::SetupDebugMessenger()
{
  vk::DebugUtilsMessengerCreateInfoEXT create_info{
    {},
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
    DebugCallback
  };

  m_debug_messenger = m_instance.createDebugUtilsMessengerEXT(create_info);
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void* user_data)
{
  std::cerr << "[Vulkan Validation] " << callback_data->pMessage << "\n";
  return VK_FALSE;
}

}
