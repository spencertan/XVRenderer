#include "Scene.h"

namespace XV
{

Scene::Scene(const std::string& name, IRenderer* renderer)
  : m_name(name)
  , m_root(std::make_unique<SceneNode>("Root"))
  , m_resource_manager(renderer)
{
}

SceneNode* Scene::CreateNode(const std::string& name, Mesh* mesh, Material* material)
{
  auto node = m_root->AddChild(name);
  if (mesh)
  {
    node->SetMesh(mesh);
  }
  if (material)
  {
    node->SetMaterial(material);
  }
  return node;
}

}
