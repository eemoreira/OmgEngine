#include "shader.h"
#include "core/logger.h"
#include <fstream>

namespace renderer {

  Shader::~Shader() { glDeleteProgram(m_id); }

  Shader::Shader(const std::filesystem::path &vertex_shader_path, const std::filesystem::path &fragment_shader_path) {
    std::string vertex_code, fragment_code;
    std::ifstream vertex_file, fragment_file;

    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vertex_file.open(vertex_shader_path);
      fragment_file.open(fragment_shader_path);

      std::stringstream vertex_stream, fragment_stream;
      vertex_stream << vertex_file.rdbuf();
      fragment_stream << fragment_file.rdbuf();

      vertex_file.close();
      fragment_file.close();

      vertex_code = vertex_stream.str();
      fragment_code = fragment_stream.str();

    } catch (std::ifstream::failure e) {
      CORE_LOG_ERROR(
          "SHADER::READING_FILES: {}\nvertex: {}, fragment: {}",
          e.what(),
          vertex_shader_path.c_str(),
          fragment_shader_path.c_str()
      );
    }

    const char *vertexCode = vertex_code.c_str();
    const char *fragmentCode = fragment_code.c_str();

    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexCode, NULL);
    glCompileShader(vertex_shader);
    if (!check_shader_compile_status(vertex_shader)) {
      glDeleteShader(vertex_shader);
      return;
    }

    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentCode, NULL);
    glCompileShader(fragment_shader);
    if (!check_shader_compile_status(fragment_shader)) {
      glDeleteShader(fragment_shader);
      return;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex_shader);
    glAttachShader(m_id, fragment_shader);
    glLinkProgram(m_id);

    if (!check_program_link_status()) {
      glDeleteProgram(m_id);
      return;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }

  bool Shader::check_shader_compile_status(uint32_t shader_handle) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader_handle, 512, NULL, infoLog);
      CORE_LOG_ERROR("ERROR::SHADER::COMPILATION_FAILED\n{0}", infoLog);
      return false;
    }
    return true;
  }

  bool Shader::check_program_link_status() {
    int success;
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(m_id, 512, NULL, infoLog);
      CORE_LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{0}", infoLog);
      return false;
    }
    return true;
  }

}
