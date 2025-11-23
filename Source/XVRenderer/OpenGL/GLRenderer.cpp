#include "OpenGL/GLRenderer.h"
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

namespace XV
{

GLRenderer::GLRenderer(Window& window)
  : m_window(&window)
{
  InitializeOpenGL();
}

GLRenderer::~GLRenderer()
{
  // OpenGL context cleanup handled by GLFW/Window
}

void GLRenderer::InitializeOpenGL()
{
  // GLFW window hint for OpenGL should already be set by Window class
  // But we need to load OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    throw std::runtime_error("[GLRenderer] Failed to initialize GLAD!");
  }

  std::cout << "[GLRenderer] OpenGL " << glGetString(GL_VERSION) << "\n";
  std::cout << "[GLRenderer] GPU: " << glGetString(GL_RENDERER) << "\n";

  // Set up initial OpenGL state
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  m_initialized = true;
}

void GLRenderer::BeginFrame()
{
  // Nothing specific needed for OpenGL begin frame
}

void GLRenderer::EndFrame()
{
  m_window->PollEvents();
  glfwSwapBuffers(m_window->GetGLFWWindow());
}

void GLRenderer::Clear(const glm::vec4& color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::DrawIndexed(u32 index_count)
{
  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
}

void GLRenderer::SetViewport(u32 x, u32 y, u32 width, u32 height)
{
  glViewport(x, y, width, height);
}

void GLRenderer::EnableDepthTest(bool enable)
{
  if (enable)
  {
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }
}

void GLRenderer::BindVertexBuffer(const VertexBuffer& buffer)
{
  buffer.Bind();
}

void GLRenderer::BindIndexBuffer(const IndexBuffer& buffer)
{
  buffer.Bind();
}

void GLRenderer::BindShader(const Shader& shader)
{
  shader.Bind();
  m_current_shader = &shader;
}

void GLRenderer::SetUniformMat4(const char* name, const glm::mat4& matrix)
{
  if (m_current_shader)
  {
    const_cast<Shader*>(m_current_shader)->SetUniformMat4(name, matrix);
  }
}

void GLRenderer::WaitIdle()
{
  glFinish();
}

}
