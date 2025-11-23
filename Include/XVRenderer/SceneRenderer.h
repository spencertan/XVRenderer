#pragma once

#include "Scene.h"
#include "IRenderer.h"
#include <XVUtilities/Helper/NonCopyable.h>

namespace XV
{

// Scene renderer - traverses and renders the scene graph
class SceneRenderer : NonCopyable
{
  IRenderer* m_renderer = nullptr;

  void RenderNode(const SceneNode* node, const Camera& camera);

public:
  SceneRenderer() = default;
  explicit SceneRenderer(IRenderer* renderer);

  void SetRenderer(IRenderer* renderer) { m_renderer = renderer; }

  // Render the entire scene
  void Render(const Scene& scene);

  // Render from a specific node (useful for rendering sub-trees)
  void RenderSubtree(const SceneNode* node, const Camera& camera);
};

}
