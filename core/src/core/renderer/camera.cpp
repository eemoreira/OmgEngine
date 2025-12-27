#include "camera.h"
#include "core/logger.h"

namespace renderer {

  Camera::Camera(glm::vec3 pos)
      : m_pos(pos), m_front(0.f), m_world_up(0.f, 1.f, 0.f), m_yaw(DEFAULT_YAW), m_pitch(DEFAUTL_PITCH),
        m_fov(MAX_FOV) {
    update_vectors();
  }

  Camera::Camera()
      : m_pos(0.f), m_front(0.f), m_world_up(0.f, 1.f, 0.f), m_yaw(DEFAULT_YAW), m_pitch(DEFAUTL_PITCH),
        m_fov(MAX_FOV) {
    update_vectors();
  }

  Camera::~Camera() { }

  void Camera::look_to(float x_offset, float y_offset) {
    m_yaw += x_offset * SENSITIVITY;
    m_yaw = glm::mod(m_yaw, 360.f);

    m_pitch += y_offset * SENSITIVITY;
    m_pitch = glm::clamp(m_pitch, MIN_PITCH, MAX_PITCH);

    update_vectors();
  }

  void Camera::zoom(float y_offset) {
    m_fov -= ZOOM_SENSITIVITY * y_offset;
    m_fov = glm::clamp(m_fov, MIN_FOV, MAX_FOV);
  }

  void Camera::move_towards(glm::vec3 direction, float delta_time) {
    const float velocity = (m_is_sprinting ? SPRINT_MULTIPLIER : 1.f) * VELOCITY * delta_time;
    m_pos += direction * velocity;
  }

  void Camera::sprint() { m_is_sprinting = true; }
  void Camera::walk() { m_is_sprinting = false; }

  void Camera::update_vectors() {
    const float &yaw_radians = glm::radians(m_yaw);
    const float &pitch_radians = glm::radians(m_pitch);

    m_front.x = glm::cos(yaw_radians) * glm::cos(pitch_radians);
    m_front.y = glm::sin(pitch_radians);
    m_front.z = glm::sin(yaw_radians) * glm::cos(pitch_radians);
    m_front = glm::normalize(m_front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));

  }

}
