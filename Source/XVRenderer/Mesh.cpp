#include "Mesh.h"
#include "IRenderer.h"
#include "RenderAPI.h"

#include <stdexcept>

namespace XV
{

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
  : m_vertices(vertices)
  , m_indices(indices)
{
}

void Mesh::UploadToGPU(IRenderer* renderer)
{
  if (m_uploaded)
    return;

  if (!renderer)
    throw std::runtime_error("[Mesh] Cannot upload to GPU: renderer is null");

  switch (renderer->GetAPI())
  {
    case RenderAPI::OpenGL:
    {
      m_gl_vbo = std::make_unique<VertexBuffer>(m_vertices);
      m_gl_ebo = std::make_unique<IndexBuffer>(m_indices);
      m_uploaded = true;
      break;
    }

    case RenderAPI::Vulkan:
    {
      // TODO: Implement Vulkan buffer upload
      throw std::runtime_error("[Mesh] Vulkan mesh upload not yet implemented");
      break;
    }

    default:
      throw std::runtime_error("[Mesh] Unknown render API");
  }
}

void Mesh::Bind(IRenderer* renderer)
{
  if (!m_uploaded)
    throw std::runtime_error("[Mesh] Cannot bind mesh: not uploaded to GPU");

  if (!renderer)
    throw std::runtime_error("[Mesh] Cannot bind mesh: renderer is null");

  switch (renderer->GetAPI())
  {
    case RenderAPI::OpenGL:
    {
      if (m_gl_vbo && m_gl_ebo)
      {
        renderer->BindVertexBuffer(*m_gl_vbo);
        renderer->BindIndexBuffer(*m_gl_ebo);
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

Mesh Mesh::CreateCube()
{
  return Mesh(Vertex::GetCubeVertices(), Vertex::GetCubeIndices());
}

Mesh Mesh::CreatePlane(float width, float height)
{
  float hw = width * 0.5f;
  float hh = height * 0.5f;

  std::vector<Vertex> vertices = {
    {{-hw, 0.0f, -hh}, {0.8f, 0.8f, 0.8f}}, // bottom-left
    {{ hw, 0.0f, -hh}, {0.9f, 0.9f, 0.9f}}, // bottom-right
    {{ hw, 0.0f,  hh}, {1.0f, 1.0f, 1.0f}}, // top-right
    {{-hw, 0.0f,  hh}, {0.7f, 0.7f, 0.7f}}, // top-left
  };

  std::vector<u32> indices = {
    0, 1, 2,
    2, 3, 0
  };

  return Mesh(vertices, indices);
}

}
