#include "app_layer.h"
#include "core/application.h"
#include "core/logger.h"
#include <glad/gl.h>

AppLayer::AppLayer() {
  m_is_red = false;
  m_is_mouse_button_held = false;
  m_shader = std::make_unique<renderer::Shader>("app/shaders/vertex/shader.glsl", "app/shaders/fragment/shader.glsl");

  // clang-format off
  float vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.5f,  0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f
  };
  // clang-format on

  uint32_t faces[] = {0, 1, 2, 2, 3, 0};

  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  APP_LOG_INFO("CREATED LAYER!, shader.id = {}", m_shader->m_id);
}
AppLayer::~AppLayer() { APP_LOG_INFO("DESTROYED LAYER!"); }

void AppLayer::on_render() {
  glClear(GL_COLOR_BUFFER_BIT);
  m_shader->use();
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void AppLayer::on_event(core::Event &event) {
  core::EventDispatcher dispatcher(event);
  dispatcher.dispatch<core::MouseButtonReleasedEvent>([this](core::MouseButtonReleasedEvent &event) {
    return on_mouse_button_released(event);
  });
  dispatcher.dispatch<core::MouseButtonPressedEvent>([this](core::MouseButtonPressedEvent &event) {
    return on_mouse_button_pressed(event);
  });
}

void AppLayer::on_update() { }

bool AppLayer::on_mouse_button_released(core::MouseButtonReleasedEvent &event) {

  GLFWwindow *handle = core::Application::get_application().get_window()->get_handle();
  if (glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && m_is_mouse_button_held) {

    if (is_red()) glClearColor(0.f, 1.f, 0.f, 1.f);
    else glClearColor(1.f, 0.f, 0.f, 1.f);
    m_is_red ^= true;
    m_is_mouse_button_held = false;
  }

  return true;
}

bool AppLayer::on_mouse_button_pressed(core::MouseButtonPressedEvent &event) {

  GLFWwindow *handle = core::Application::get_application().get_window()->get_handle();
  if (glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) m_is_mouse_button_held = true;

  return true;
}
