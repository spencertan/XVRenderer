#pragma once

#include "SceneNode.h"
#include "ResourceManager.h"
#include "Camera.h"
#include <XVUtilities/Helper/NonCopyable.h>

#include <memory>
#include <string>

namespace XV
{

// Forward declarations
class IRenderer;

// Scene graph manager
class Scene : NonCopyable
{
  std::string m_name;
  std::unique_ptr<SceneNode> m_root;
  ResourceManager m_resource_manager;

  Camera m_main_camera;

public:
  explicit Scene(const std::string& name = "Scene", IRenderer* renderer = nullptr);

  // === Root Node Access ===

  SceneNode* GetRoot() { return m_root.get(); }
  const SceneNode* GetRoot() const { return m_root.get(); }

  // === Resource Management ===

  ResourceManager& GetResourceManager() { return m_resource_manager; }
  const ResourceManager& GetResourceManager() const { return m_resource_manager; }

  // === Camera ===

  Camera& GetMainCamera() { return m_main_camera; }
  const Camera& GetMainCamera() const { return m_main_camera; }

  void SetMainCamera(const Camera& camera) { m_main_camera = camera; }

  // === Scene Helpers ===

  // Create a renderable node with mesh and material
  SceneNode* CreateNode(const std::string& name, Mesh* mesh = nullptr, Material* material = nullptr);

  // === Accessors ===

  const std::string& GetName() const { return m_name; }
  void SetName(const std::string& name) { m_name = name; }
};

}
