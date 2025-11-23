#include "Camera.h"

namespace XV
{

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
  : m_position(position)
  , m_world_up(up)
  , m_yaw(yaw)
  , m_pitch(pitch)
  , m_fov(45.0f)
  , m_aspect_ratio(16.0f / 9.0f)
  , m_near_plane(0.1f)
  , m_far_plane(100.0f)
{
  UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
  return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
  return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
}

void Camera::MoveForward(float distance)
{
  m_position += m_front * distance;
}

void Camera::MoveRight(float distance)
{
  m_position += m_right * distance;
}

void Camera::MoveUp(float distance)
{
  m_position += m_up * distance;
}

void Camera::Rotate(float yaw_offset, float pitch_offset)
{
  m_yaw += yaw_offset;
  m_pitch += pitch_offset;

  // Constrain pitch
  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f)
    m_pitch = -89.0f;

  UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
  // Calculate new front vector
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(front);

  // Re-calculate right and up vectors
  m_right = glm::normalize(glm::cross(m_front, m_world_up));
  m_up = glm::normalize(glm::cross(m_right, m_front));
}

}
