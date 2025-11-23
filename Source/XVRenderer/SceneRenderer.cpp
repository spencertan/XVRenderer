#include "SceneRenderer.h"

#include <stdexcept>
#include <iostream>

namespace XV
{

SceneRenderer::SceneRenderer(IRenderer* renderer)
  : m_renderer(renderer)
{
}

void SceneRenderer::Render(const Scene& scene)
{
  if (!m_renderer)
  {
    throw std::runtime_error("[SceneRenderer] Cannot render: renderer is null");
  }

  const Camera& camera = scene.GetMainCamera();
  const SceneNode* root = scene.GetRoot();

  if (root)
  {
    RenderSubtree(root, camera);
  }
}

void SceneRenderer::RenderSubtree(const SceneNode* node, const Camera& camera)
{
  if (!node || !node->IsActive())
  {
    return;
  }

  // Render this node if it's renderable
  if (node->IsRenderable())
  {
    RenderNode(node, camera);
  }

  // Recursively render children
  for (const auto& child : node->GetChildren())
  {
    RenderSubtree(child.get(), camera);
  }
}

void SceneRenderer::RenderNode(const SceneNode* node, const Camera& camera)
{
  Mesh* mesh = node->GetMesh();
  Material* material = node->GetMaterial();

  if (!mesh || !material)
  {
    return;
  }

  // Bind material (which binds the shader)
  material->Bind(m_renderer);

  // Set uniforms
  m_renderer->SetUniformMat4("uModel", node->GetWorldMatrix());
  m_renderer->SetUniformMat4("uView", camera.GetViewMatrix());
  m_renderer->SetUniformMat4("uProjection", camera.GetProjectionMatrix());

  // Bind mesh and draw
  mesh->Bind(m_renderer);
  m_renderer->DrawIndexed(mesh->GetIndexCount());
}

}
