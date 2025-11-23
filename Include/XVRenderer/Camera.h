#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <XVUtilities/Types.h>

namespace XV
{

class Camera
{
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_world_up;

  float m_yaw;
  float m_pitch;

  float m_fov;
  float m_aspect_ratio;
  float m_near_plane;
  float m_far_plane;

  void UpdateCameraVectors();

public:
  Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = -90.0f,
    float pitch = 0.0f
  );

  // Accessors
  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix() const;
  glm::vec3 GetPosition() const { return m_position; }
  glm::vec3 GetFront() const { return m_front; }

  // Setters
  void SetPosition(const glm::vec3& position) { m_position = position; }
  void SetAspectRatio(float aspect_ratio) { m_aspect_ratio = aspect_ratio; }
  void SetFOV(float fov) { m_fov = fov; }

  // Movement
  void MoveForward(float distance);
  void MoveRight(float distance);
  void MoveUp(float distance);

  // Rotation
  void Rotate(float yaw_offset, float pitch_offset);
};

}
