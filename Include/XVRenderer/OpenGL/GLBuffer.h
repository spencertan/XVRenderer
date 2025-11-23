#pragma once

#include "Vertex.h"
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>
#include <vector>

namespace XV
{

class VertexBuffer : NonCopyable
{
  u32 m_vbo = 0;
  u32 m_vao = 0;
  u32 m_vertex_count = 0;

public:
  VertexBuffer() = default;
  VertexBuffer(const std::vector<Vertex>& vertices);
  ~VertexBuffer();

  // Move semantics
  VertexBuffer(VertexBuffer&& other) noexcept;
  VertexBuffer& operator=(VertexBuffer&& other) noexcept;

  void Bind() const;
  void Unbind() const;

  u32 GetVertexCount() const { return m_vertex_count; }
  u32 GetVAO() const { return m_vao; }
};

class IndexBuffer : NonCopyable
{
  u32 m_ebo = 0;
  u32 m_index_count = 0;

public:
  IndexBuffer() = default;
  IndexBuffer(const std::vector<u32>& indices);
  ~IndexBuffer();

  // Move semantics
  IndexBuffer(IndexBuffer&& other) noexcept;
  IndexBuffer& operator=(IndexBuffer&& other) noexcept;

  void Bind() const;
  void Unbind() const;

  u32 GetIndexCount() const { return m_index_count; }
};

}
