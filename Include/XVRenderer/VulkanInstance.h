#pragma once

#include "Platform.h"

#include <vulkan/vulkan.hpp>
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <vector>
#include <string>

namespace XV
{

struct InstanceConfig
{
  std::string application_name = "XVRenderer Application";
  std::string engine_name = "XVRenderer";
  u32 application_version = VK_MAKE_VERSION(1, 0, 0);
  u32 engine_version = VK_MAKE_VERSION(1, 0, 0);
  u32 api_version = VK_API_VERSION_1_3;
  bool enable_validation_layers = true;
  std::vector<const char*> required_extensions;
  std::vector<const char*> required_layers;
};

class VulkanInstance : NonCopyable
{
  vk::Instance m_instance;
  vk::DebugUtilsMessengerEXT m_debug_messenger;
  bool m_validation_enabled;

  // Validation layer support
  static constexpr const char* s_validation_layer_name = "VK_LAYER_KHRONOS_validation";

  bool CheckValidationLayerSupport() const;
  std::vector<const char*> GetRequiredExtensions(const std::vector<const char*>& additional_extensions) const;
  void SetupDebugMessenger();

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data);

public:
  VulkanInstance(const InstanceConfig& config = {});
  ~VulkanInstance();

  // Move semantics
  VulkanInstance(VulkanInstance&& other) noexcept;
  VulkanInstance& operator=(VulkanInstance&& other) noexcept;

  vk::Instance GetInstance() const { return m_instance; }
  bool IsValidationEnabled() const { return m_validation_enabled; }
};

}
