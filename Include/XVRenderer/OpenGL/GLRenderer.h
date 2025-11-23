#pragma once

#include "IRenderer.h"
#include "OpenGL/GLBuffer.h"
#include "OpenGL/GLShader.h"
#include <XVUtilities/Helper/NonCopyable.h>
#include <memory>

namespace XV
{

class Window;

class GLRenderer : public IRenderer, NonCopyable
{
  Ptr<Window> m_window;
  Ptr<const Shader> m_current_shader = nullptr;

  bool m_initialized = false;

  void InitializeOpenGL();

public:
  GLRenderer(Window& window);
  ~GLRenderer() override;

  RenderAPI GetAPI() const override { return RenderAPI::OpenGL; }

  // Frame lifecycle
  void BeginFrame() override;
  void EndFrame() override;
  void Clear(const glm::vec4& color) override;

  // Drawing
  void DrawIndexed(u32 index_count) override;

  // State management
  void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
  void EnableDepthTest(bool enable) override;

  // Resource binding
  void BindVertexBuffer(const VertexBuffer& buffer) override;
  void BindIndexBuffer(const IndexBuffer& buffer) override;
  void BindShader(const Shader& shader) override;

  // Uniform updates
  void SetUniformMat4(const char* name, const glm::mat4& matrix) override;

  // Synchronization
  void WaitIdle() override;
};

}
