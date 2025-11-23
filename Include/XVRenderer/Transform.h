#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace XV
{

// Transform component for 3D objects
class Transform
{
  glm::vec3 m_position;
  glm::quat m_rotation;
  glm::vec3 m_scale;

  mutable glm::mat4 m_local_matrix;
  mutable bool m_dirty = true;

  void UpdateMatrix() const;

public:
  Transform();
  Transform(const glm::vec3& position, const glm::quat& rotation = glm::quat(1, 0, 0, 0), const glm::vec3& scale = glm::vec3(1));

  // Position
  void SetPosition(const glm::vec3& position);
  void Translate(const glm::vec3& offset);
  const glm::vec3& GetPosition() const { return m_position; }

  // Rotation
  void SetRotation(const glm::quat& rotation);
  void SetRotation(const glm::vec3& euler_angles); // in radians
  void Rotate(const glm::quat& rotation);
  void Rotate(float angle, const glm::vec3& axis);
  const glm::quat& GetRotation() const { return m_rotation; }
  glm::vec3 GetEulerAngles() const; // in radians

  // Scale
  void SetScale(const glm::vec3& scale);
  void SetScale(float uniform_scale);
  void Scale(const glm::vec3& scale);
  const glm::vec3& GetScale() const { return m_scale; }

  // Direction vectors
  glm::vec3 GetForward() const;
  glm::vec3 GetRight() const;
  glm::vec3 GetUp() const;

  // Matrix
  const glm::mat4& GetLocalMatrix() const;

  // Reset
  void Reset();
};

}
