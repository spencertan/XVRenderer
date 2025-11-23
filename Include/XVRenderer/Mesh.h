#pragma once

#include "Vertex.h"
#include "OpenGL/GLBuffer.h"
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <vector>
#include <memory>

namespace XV
{

// Forward declarations
class IRenderer;

// Mesh resource that works with both OpenGL and Vulkan
class Mesh : NonCopyable
{
  std::vector<Vertex> m_vertices;
  std::vector<u32> m_indices;

  // OpenGL resources
  std::unique_ptr<VertexBuffer> m_gl_vbo;
  std::unique_ptr<IndexBuffer> m_gl_ebo;

  // Vulkan resources (future)
  // std::unique_ptr<VulkanBuffer> m_vk_vertex_buffer;
  // std::unique_ptr<VulkanBuffer> m_vk_index_buffer;

  bool m_uploaded = false;

public:
  Mesh() = default;
  Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices);

  // Move semantics
  Mesh(Mesh&& other) noexcept = default;
  Mesh& operator=(Mesh&& other) noexcept = default;

  // Upload mesh data to GPU for specific renderer
  void UploadToGPU(IRenderer* renderer);

  // Bind mesh for rendering (API-specific)
  void Bind(IRenderer* renderer);

  // Accessors
  const std::vector<Vertex>& GetVertices() const { return m_vertices; }
  const std::vector<u32>& GetIndices() const { return m_indices; }
  u32 GetVertexCount() const { return static_cast<u32>(m_vertices.size()); }
  u32 GetIndexCount() const { return static_cast<u32>(m_indices.size()); }
  bool IsUploaded() const { return m_uploaded; }

  // Predefined meshes
  static Mesh CreateCube();
  static Mesh CreatePlane(float width = 1.0f, float height = 1.0f);
};

}
