#pragma once
#include "light.h"
#include <cstdint>
#include <filesystem>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace renderer {

  struct Shader {
    uint32_t m_id;

    Shader(const std::filesystem::path &vertex_shader_path, const std::filesystem::path &fragment_shader_path);
    ~Shader();

    bool check_shader_compile_status(uint32_t shader_handle);
    bool check_program_link_status();

    void use() { glUseProgram(m_id); }

    void setInt(const std::string &uniform_name, int val) { glUniform1i(glGetUniformLocation(m_id, uniform_name.c_str()), val); }

    void setFloat(const std::string &uniform_name, float val) { glUniform1f(glGetUniformLocation(m_id, uniform_name.c_str()), val); }

    void setFloat2(const std::string &uniform_name, float val1, float val2) {
      glUniform2f(glGetUniformLocation(m_id, uniform_name.c_str()), val1, val2);
    }

    void setFloat3(const std::string &uniform_name, float val1, float val2, float val3) {
      glUniform3f(glGetUniformLocation(m_id, uniform_name.c_str()), val1, val2, val3);
    }
    void setVec3(const std::string &uniform_name, glm::vec3 vec) {
      glUniform3f(glGetUniformLocation(m_id, uniform_name.c_str()), vec.x, vec.y, vec.z);
    }
    void setMatrix4f(const std::string &uniform_name, glm::mat4 mat) {
      glUniformMatrix4fv(glGetUniformLocation(m_id, uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setMatrix3f(const std::string &uniform_name, glm::mat3 mat) {
      glUniformMatrix3fv(glGetUniformLocation(m_id, uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setLight(const std::string &uniform_name, std::shared_ptr<Light> light) {
      setVec3(uniform_name + ".position", light->get_position());
      setVec3(uniform_name + ".ambient", light->get_ambient());
      setVec3(uniform_name + ".diffuse", light->get_diffuse());
      setVec3(uniform_name + ".specular", light->get_specular());
      setFloat(uniform_name + ".constant", light->get_constant());
      setFloat(uniform_name + ".linear", light->get_linear());
      setFloat(uniform_name + ".quadratic", light->get_quadratic());
    }
    void setFlashLight(const std::string &uniform_name, std::shared_ptr<FlashLight> flashlight) {
      setVec3(uniform_name + ".light.position", flashlight->get_position());
      setVec3(uniform_name + ".light.ambient", flashlight->get_ambient());
      setVec3(uniform_name + ".light.diffuse", flashlight->get_diffuse());
      setVec3(uniform_name + ".light.specular", flashlight->get_specular());
      setFloat(uniform_name + ".light.constant", flashlight->get_constant());
      setFloat(uniform_name + ".light.linear", flashlight->get_linear());
      setFloat(uniform_name + ".light.quadratic", flashlight->get_quadratic());
      setVec3(uniform_name + ".direction", flashlight->get_direction());
      setFloat(uniform_name + ".cut_off", flashlight->get_cut_off());
      setFloat(uniform_name + ".outer_cut_off", flashlight->get_outer_cut_off());
    }
  };

}
