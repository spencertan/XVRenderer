#pragma once

#include "OpenGL/GLShader.h"
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace XV
{

// Forward declarations
class IRenderer;

// Material properties
struct MaterialProperties
{
  glm::vec3 albedo = glm::vec3(1.0f);
  float metallic = 0.0f;
  float roughness = 0.5f;
  float ao = 1.0f; // ambient occlusion

  // Future: texture handles
  // u32 albedo_texture = 0;
  // u32 normal_texture = 0;
  // u32 metallic_roughness_texture = 0;
};

// Material resource that works with both OpenGL and Vulkan
class Material : NonCopyable
{
  std::string m_name;
  MaterialProperties m_properties;

  // OpenGL resources
  std::unique_ptr<Shader> m_gl_shader;

  // Vulkan resources (future)
  // std::unique_ptr<VulkanPipeline> m_vk_pipeline;
  // std::unique_ptr<VulkanDescriptorSet> m_vk_descriptor_set;

  bool m_compiled = false;

public:
  Material() = default;
  Material(const std::string& name, const MaterialProperties& props = {});

  // Move semantics
  Material(Material&& other) noexcept = default;
  Material& operator=(Material&& other) noexcept = default;

  // Compile material for specific renderer
  void Compile(IRenderer* renderer);

  // Bind material for rendering (API-specific)
  void Bind(IRenderer* renderer);

  // Update material properties
  void SetProperties(const MaterialProperties& props);
  void SetAlbedo(const glm::vec3& albedo);

  // Accessors
  const std::string& GetName() const { return m_name; }
  const MaterialProperties& GetProperties() const { return m_properties; }
  bool IsCompiled() const { return m_compiled; }

  // Predefined materials
  static Material CreateDefault(const std::string& name = "Default");
  static Material CreateColored(const glm::vec3& color, const std::string& name = "Colored");
};

}
