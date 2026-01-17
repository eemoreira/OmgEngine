#pragma once

#include <glm/glm.hpp>

namespace renderer {

  struct Light {
  public:
    Light(
        const glm::vec3 &position,
        const glm::vec3 &ambient,
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float constant,
        float linear,
        float quadratic
    )
        : m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_constant(constant), m_linear(linear),
          m_quadratic(quadratic) { }

    const glm::vec3 &get_position() const { return m_position; }
    const glm::vec3 &get_ambient() const { return m_ambient; }
    const glm::vec3 &get_diffuse() const { return m_diffuse; }
    const glm::vec3 &get_specular() const { return m_specular; }
    float get_constant() const { return m_constant; }
    float get_linear() const { return m_linear; }
    float get_quadratic() const { return m_quadratic; }
    void move_to(const glm::vec3 &new_position) { m_position = new_position; }

    void set_ambient(const glm::vec3 &ambient) { m_ambient = ambient; }
    void set_diffuse(const glm::vec3 &diffuse) { m_diffuse = diffuse; }
    void set_specular(const glm::vec3 &specular) { m_specular = specular; }
    void set_constant(float constant) { m_constant = constant; }
    void set_linear(float linear) { m_linear = linear; }
    void set_quadratic(float quadratic) { m_quadratic = quadratic; }

    void kill_light() {
      m_ambient = glm::vec3(0.f);
      m_diffuse = glm::vec3(0.f);
      m_specular = glm::vec3(0.f);
    }

  private:
    glm::vec3 m_position;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_constant;
    float m_linear;
    float m_quadratic;
  };

  struct FlashLight : public Light {
  public:
    FlashLight(
        const glm::vec3 &position,
        const glm::vec3 &direction,
        float cut_off,
        float outer_cut_off,
        const glm::vec3 &ambient,
        const glm::vec3 &diffuse,
        const glm::vec3 &specular,
        float constant,
        float linear,
        float quadratic
    )
        : Light(position, ambient, diffuse, specular, constant, linear, quadratic), m_direction(direction), m_cut_off(cut_off),
          m_outer_cut_off(outer_cut_off) { }
    const glm::vec3 &get_direction() const { return m_direction; }
    float get_cut_off() const { return m_cut_off; }
    float get_outer_cut_off() const { return m_outer_cut_off; }

    void set_direction(const glm::vec3 &direction) { m_direction = direction; }

  private:
    glm::vec3 m_direction;
    float m_cut_off;
    float m_outer_cut_off;
  };

}
