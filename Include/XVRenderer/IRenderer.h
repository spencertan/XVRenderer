#pragma once

#include "RenderAPI.h"
#include <XVUtilities/Types.h>
#include <glm/glm.hpp>

namespace XV
{

// Forward declarations
class Window;
struct Vertex;
class VertexBuffer;
class IndexBuffer;
class Shader;

// Abstract renderer interface
class IRenderer
{
public:
  virtual ~IRenderer() = default;

  virtual RenderAPI GetAPI() const = 0;

  // Frame lifecycle
  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void Clear(const glm::vec4& color) = 0;

  // Drawing
  virtual void DrawIndexed(u32 index_count) = 0;

  // State management
  virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) = 0;
  virtual void EnableDepthTest(bool enable) = 0;

  // Resource binding
  virtual void BindVertexBuffer(const VertexBuffer& buffer) = 0;
  virtual void BindIndexBuffer(const IndexBuffer& buffer) = 0;
  virtual void BindShader(const Shader& shader) = 0;

  // Uniform updates
  virtual void SetUniformMat4(const char* name, const glm::mat4& matrix) = 0;

  // Synchronization
  virtual void WaitIdle() = 0;
};

}
