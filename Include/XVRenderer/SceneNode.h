#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include <XVUtilities/Types.h>
#include <XVUtilities/Helper/NonCopyable.h>

#include <string>
#include <vector>
#include <memory>

namespace XV
{

// Node in the scene graph hierarchy
class SceneNode : NonCopyable
{
  std::string m_name;
  bool m_active = true;

  Transform m_local_transform;
  mutable glm::mat4 m_world_matrix;
  mutable bool m_world_dirty = true;

  // Hierarchy
  SceneNode* m_parent = nullptr;
  std::vector<std::unique_ptr<SceneNode>> m_children;

  // Rendering components (not owned)
  Mesh* m_mesh = nullptr;
  Material* m_material = nullptr;

  void MarkWorldDirty();
  void UpdateWorldMatrix() const;

public:
  explicit SceneNode(const std::string& name = "Node");

  // === Hierarchy Management ===

  SceneNode* AddChild(const std::string& name = "Child");
  SceneNode* AddChild(std::unique_ptr<SceneNode> child);
  void RemoveChild(SceneNode* child);
  void RemoveAllChildren();

  SceneNode* GetChild(const std::string& name);
  const SceneNode* GetChild(const std::string& name) const;
  SceneNode* GetChild(size_t index);
  const SceneNode* GetChild(size_t index) const;

  SceneNode* GetParent() { return m_parent; }
  const SceneNode* GetParent() const { return m_parent; }

  size_t GetChildCount() const { return m_children.size(); }
  const std::vector<std::unique_ptr<SceneNode>>& GetChildren() const { return m_children; }

  // === Transform Management ===

  Transform& GetTransform() { MarkWorldDirty(); return m_local_transform; }
  const Transform& GetTransform() const { return m_local_transform; }

  const glm::mat4& GetWorldMatrix() const;
  glm::vec3 GetWorldPosition() const;

  // === Component Management ===

  void SetMesh(Mesh* mesh) { m_mesh = mesh; }
  void SetMaterial(Material* material) { m_material = material; }

  Mesh* GetMesh() { return m_mesh; }
  const Mesh* GetMesh() const { return m_mesh; }

  Material* GetMaterial() { return m_material; }
  const Material* GetMaterial() const { return m_material; }

  bool HasMesh() const { return m_mesh != nullptr; }
  bool HasMaterial() const { return m_material != nullptr; }
  bool IsRenderable() const { return m_mesh != nullptr && m_material != nullptr; }

  // === Activation ===

  void SetActive(bool active) { m_active = active; }
  bool IsActive() const { return m_active; }

  // === Accessors ===

  const std::string& GetName() const { return m_name; }
  void SetName(const std::string& name) { m_name = name; }

private:
  void SetParent(SceneNode* parent);
};

}
