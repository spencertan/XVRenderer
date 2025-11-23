#include "SceneNode.h"

#include <algorithm>
#include <stdexcept>

namespace XV
{

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

// === Hierarchy Management ===

SceneNode* SceneNode::AddChild(const std::string& name)
{
  auto child = std::make_unique<SceneNode>(name);
  child->SetParent(this);
  SceneNode* child_ptr = child.get();
  m_children.push_back(std::move(child));
  return child_ptr;
}

SceneNode* SceneNode::AddChild(std::unique_ptr<SceneNode> child)
{
  if (!child)
    return nullptr;

  child->SetParent(this);
  SceneNode* child_ptr = child.get();
  m_children.push_back(std::move(child));
  return child_ptr;
}

void SceneNode::RemoveChild(SceneNode* child)
{
  auto it = std::find_if(m_children.begin(), m_children.end(),
    [child](const std::unique_ptr<SceneNode>& node) { return node.get() == child; });

  if (it != m_children.end())
  {
    (*it)->SetParent(nullptr);
    m_children.erase(it);
  }
}

void SceneNode::RemoveAllChildren()
{
  for (auto& child : m_children)
  {
    child->SetParent(nullptr);
  }
  m_children.clear();
}

SceneNode* SceneNode::GetChild(const std::string& name)
{
  for (auto& child : m_children)
  {
    if (child->GetName() == name)
    {
      return child.get();
    }
  }
  return nullptr;
}

const SceneNode* SceneNode::GetChild(const std::string& name) const
{
  for (const auto& child : m_children)
  {
    if (child->GetName() == name)
    {
      return child.get();
    }
  }
  return nullptr;
}

SceneNode* SceneNode::GetChild(size_t index)
{
  if (index < m_children.size())
  {
    return m_children[index].get();
  }
  return nullptr;
}

const SceneNode* SceneNode::GetChild(size_t index) const
{
  if (index < m_children.size())
  {
    return m_children[index].get();
  }
  return nullptr;
}

// === Transform Management ===

const glm::mat4& SceneNode::GetWorldMatrix() const
{
  if (m_world_dirty)
  {
    UpdateWorldMatrix();
  }
  return m_world_matrix;
}

glm::vec3 SceneNode::GetWorldPosition() const
{
  const glm::mat4& world = GetWorldMatrix();
  return glm::vec3(world[3]);
}

// === Private Methods ===

void SceneNode::SetParent(SceneNode* parent)
{
  m_parent = parent;
  MarkWorldDirty();
}

void SceneNode::MarkWorldDirty()
{
  m_world_dirty = true;

  // Propagate to children
  for (auto& child : m_children)
  {
    child->MarkWorldDirty();
  }
}

void SceneNode::UpdateWorldMatrix() const
{
  if (m_parent)
  {
    m_world_matrix = m_parent->GetWorldMatrix() * m_local_transform.GetLocalMatrix();
  }
  else
  {
    m_world_matrix = m_local_transform.GetLocalMatrix();
  }

  m_world_dirty = false;
}

}
