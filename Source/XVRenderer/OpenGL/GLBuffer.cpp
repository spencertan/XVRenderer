#include "OpenGL/GLBuffer.h"

#include <glad/glad.h>
#include <iostream>

namespace XV
{

// ===== VertexBuffer =====

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
  : m_vertex_count(static_cast<u32>(vertices.size()))
{
  // Generate and bind VAO
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  // Generate and bind VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  // Position attribute (location = 0)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

  // Color attribute (location = 1)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

  glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer()
{
  if (m_vbo)
  {
    glDeleteBuffers(1, &m_vbo);
  }
  if (m_vao)
  {
    glDeleteVertexArrays(1, &m_vao);
  }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
  : m_vbo(other.m_vbo)
  , m_vao(other.m_vao)
  , m_vertex_count(other.m_vertex_count)
{
  other.m_vbo = 0;
  other.m_vao = 0;
  other.m_vertex_count = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
  if (this != &other)
  {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);

    m_vbo = other.m_vbo;
    m_vao = other.m_vao;
    m_vertex_count = other.m_vertex_count;

    other.m_vbo = 0;
    other.m_vao = 0;
    other.m_vertex_count = 0;
  }
  return *this;
}

void VertexBuffer::Bind() const
{
  glBindVertexArray(m_vao);
}

void VertexBuffer::Unbind() const
{
  glBindVertexArray(0);
}

// ===== IndexBuffer =====

IndexBuffer::IndexBuffer(const std::vector<u32>& indices)
  : m_index_count(static_cast<u32>(indices.size()))
{
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
  if (m_ebo)
  {
    glDeleteBuffers(1, &m_ebo);
  }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
  : m_ebo(other.m_ebo)
  , m_index_count(other.m_index_count)
{
  other.m_ebo = 0;
  other.m_index_count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
  if (this != &other)
  {
    if (m_ebo) glDeleteBuffers(1, &m_ebo);

    m_ebo = other.m_ebo;
    m_index_count = other.m_index_count;

    other.m_ebo = 0;
    other.m_index_count = 0;
  }
  return *this;
}

void IndexBuffer::Bind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void IndexBuffer::Unbind() const
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
