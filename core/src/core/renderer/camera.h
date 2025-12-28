#pragma

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace renderer {

  const float DEFAULT_YAW = -90.f;
  const float DEFAUTL_PITCH = 0.f;
  const float SENSITIVITY = 0.05f;
  const float VELOCITY = 25.f;
  const float SPRINT_MULTIPLIER = 2.f;
  const float MAX_FOV = 100.f;
  const float MIN_FOV = 20.f;
  const float ZOOM_SENSITIVITY = 10.f;
  const float MAX_PITCH = 89.f;
  const float MIN_PITCH = -89.f;

  struct Camera {
  public:
    Camera(glm::vec3 pos);
    Camera();
    ~Camera();

    void look_to(float x, float y);
    void zoom(float y_offset);
    void move_towards(glm::vec3 direction, float delta_time);
    void update_vectors();

    glm::mat4 view_matrix() { return glm::lookAt(m_pos, m_pos + m_front, m_up); }
    glm::vec3 get_posistion() { return m_pos; }
    float get_fov() { return m_fov; }
    glm::vec3 get_front() { return m_front; }
    glm::vec3 get_right() { return m_right; }
    glm::vec3 get_up() { return m_up; }
    void sprint();
    void walk();

  private:
    glm::vec3 m_pos, m_front, m_right, m_up, m_world_up;
    float m_yaw, m_pitch, m_fov;
    bool m_is_sprinting = false;
  };

}
