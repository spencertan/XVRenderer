#include "Transform.h"
#include <glm/gtx/euler_angles.hpp>

namespace XV
{

Transform::Transform()
  : m_position(0.0f)
  , m_rotation(1, 0, 0, 0) // Identity quaternion
  , m_scale(1.0f)
{
}

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
  : m_position(position)
  , m_rotation(rotation)
  , m_scale(scale)
{
}

void Transform::SetPosition(const glm::vec3& position)
{
  m_position = position;
  m_dirty = true;
}

void Transform::Translate(const glm::vec3& offset)
{
  m_position += offset;
  m_dirty = true;
}

void Transform::SetRotation(const glm::quat& rotation)
{
  m_rotation = rotation;
  m_dirty = true;
}

void Transform::SetRotation(const glm::vec3& euler_angles)
{
  m_rotation = glm::quat(euler_angles);
  m_dirty = true;
}

void Transform::Rotate(const glm::quat& rotation)
{
  m_rotation = rotation * m_rotation;
  m_dirty = true;
}

void Transform::Rotate(float angle, const glm::vec3& axis)
{
  m_rotation = glm::angleAxis(angle, axis) * m_rotation;
  m_dirty = true;
}

glm::vec3 Transform::GetEulerAngles() const
{
  return glm::eulerAngles(m_rotation);
}

void Transform::SetScale(const glm::vec3& scale)
{
  m_scale = scale;
  m_dirty = true;
}

void Transform::SetScale(float uniform_scale)
{
  m_scale = glm::vec3(uniform_scale);
  m_dirty = true;
}

void Transform::Scale(const glm::vec3& scale)
{
  m_scale *= scale;
  m_dirty = true;
}

glm::vec3 Transform::GetForward() const
{
  return m_rotation * glm::vec3(0, 0, -1);
}

glm::vec3 Transform::GetRight() const
{
  return m_rotation * glm::vec3(1, 0, 0);
}

glm::vec3 Transform::GetUp() const
{
  return m_rotation * glm::vec3(0, 1, 0);
}

const glm::mat4& Transform::GetLocalMatrix() const
{
  if (m_dirty)
  {
    UpdateMatrix();
  }
  return m_local_matrix;
}

void Transform::Reset()
{
  m_position = glm::vec3(0.0f);
  m_rotation = glm::quat(1, 0, 0, 0);
  m_scale = glm::vec3(1.0f);
  m_dirty = true;
}

void Transform::UpdateMatrix() const
{
  m_local_matrix = glm::translate(glm::mat4(1.0f), m_position);
  m_local_matrix *= glm::mat4_cast(m_rotation);
  m_local_matrix = glm::scale(m_local_matrix, m_scale);
  m_dirty = false;
}

}
