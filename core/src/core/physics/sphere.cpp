#include "sphere.h"
#include "core/logger.h"
#include <glm/glm.hpp>

namespace physics {

  Sphere::Sphere(float radius, float density, const glm::vec3 &position, const glm::vec3 &velocity)
      : m_radius(radius), m_position(position), m_velocity(velocity) {
    mass = (4.f / 3.f) * PI * glm::pow(radius, 3) * density;
    m_acceleration = glm::vec3(0.f);
  }

  void Sphere::apply_gravity(const std::unique_ptr<Sphere> &oth) {
    glm::vec3 direction = oth->m_position - m_position;
    float distance = glm::length(direction);
    distance = glm::clamp(distance, 0.1f, FLT_MAX);
    direction = glm::normalize(direction);
    float force_magnitude = (G * mass * oth->mass) / (distance * distance);
    glm::vec3 force = force_magnitude * direction;
    m_acceleration += force / mass;
  }

}
