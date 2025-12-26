#pragma once
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

    void setInt(const std::string &uniform_name, int val) {
      glUniform1i(glGetUniformLocation(m_id, uniform_name.c_str()), val);
    }

    void setFloat(const std::string &uniform_name, float val) {
      glUniform1f(glGetUniformLocation(m_id, uniform_name.c_str()), val);
    }

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
  };

}
