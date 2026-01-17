#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace physics {

  const float PI = 3.14159265359f;
  const float G = 0.0007; // gravitational constant

  struct Sphere {
  public:
    Sphere(float radius, float density, const glm::vec3 &position, const glm::vec3 &velocity);
    ~Sphere() = default;

    float get_radius() const { return m_radius; }
    float get_mass() const { return mass; }
    glm::vec3 get_position() const { return m_position; }
    glm::vec3 get_velocity() const { return m_velocity; }
    glm::vec3 get_acceleration() const { return m_acceleration; }

    void apply_gravity(const std::unique_ptr<Sphere> &oth);
    void step(float time_step) {
      m_velocity += m_acceleration * time_step;
      m_position += m_velocity * time_step;
      m_acceleration = glm::vec3(0.f);
    }

  private:
    float m_radius, mass;
    glm::vec3 m_position, m_velocity, m_acceleration;
  };

}
