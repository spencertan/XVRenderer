#include "Material.h"
#include "IRenderer.h"
#include "RenderAPI.h"

#include <stdexcept>

namespace XV
{

Material::Material(const std::string& name, const MaterialProperties& props)
  : m_name(name)
  , m_properties(props)
{
}

void Material::Compile(IRenderer* renderer)
{
  if (m_compiled)
    return;

  if (!renderer)
    throw std::runtime_error("[Material] Cannot compile: renderer is null");

  switch (renderer->GetAPI())
  {
    case RenderAPI::OpenGL:
    {
      // Use default shader for now
      m_gl_shader = std::make_unique<Shader>(Shader::CreateDefault());
      m_compiled = true;
      break;
    }

    case RenderAPI::Vulkan:
    {
      // TODO: Implement Vulkan pipeline compilation
      throw std::runtime_error("[Material] Vulkan material compilation not yet implemented");
      break;
    }

    default:
      throw std::runtime_error("[Material] Unknown render API");
  }
}

void Material::Bind(IRenderer* renderer)
{
  if (!m_compiled)
    throw std::runtime_error("[Material] Cannot bind material: not compiled");

  if (!renderer)
    throw std::runtime_error("[Material] Cannot bind material: renderer is null");

  switch (renderer->GetAPI())
  {
    case RenderAPI::OpenGL:
    {
      if (m_gl_shader)
      {
        renderer->BindShader(*m_gl_shader);
      }
      break;
    }

    case RenderAPI::Vulkan:
    {
      // TODO: Implement Vulkan binding
      break;
    }

    default:
      break;
  }
}

void Material::SetProperties(const MaterialProperties& props)
{
  m_properties = props;
  // TODO: Update GPU resources if already compiled
}

void Material::SetAlbedo(const glm::vec3& albedo)
{
  m_properties.albedo = albedo;
  // TODO: Update GPU resources if already compiled
}

Material Material::CreateDefault(const std::string& name)
{
  MaterialProperties props;
  props.albedo = glm::vec3(0.8f);
  props.metallic = 0.0f;
  props.roughness = 0.5f;
  props.ao = 1.0f;

  return Material(name, props);
}

Material Material::CreateColored(const glm::vec3& color, const std::string& name)
{
  MaterialProperties props;
  props.albedo = color;
  props.metallic = 0.0f;
  props.roughness = 0.5f;
  props.ao = 1.0f;

  return Material(name, props);
}

}
