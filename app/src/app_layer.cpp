#include "app_layer.h"
#include "core/application.h"
#include "core/logger.h"
#include "imgui.h"
#include "imgui_layer.h"
#include <glad/gl.h>

AppLayer::AppLayer() {

  m_shader = std::make_unique<renderer::Shader>("app/shaders/vertex/shader.glsl", "app/shaders/fragment/shader.glsl");
  m_textured_shader =
      std::make_unique<renderer::Shader>("app/shaders/vertex/textured_shader.glsl", "app/shaders/fragment/textured_shader.glsl");

  m_camera = std::make_unique<renderer::Camera>();

  create_light(glm::vec3(0.f), glm::vec3(0.9f), glm::vec3(1.0f), glm::vec3(1.0f), 1.f, 0.007f, 0.0002f);

  m_backpack = std::make_unique<renderer::Model>("app/assets/backpack/backpack.obj", true);
  m_sponza = std::make_unique<renderer::Model>("app/assets/sponza/sponza.obj", false);
  m_light_sphere = std::make_unique<renderer::Model>("app/assets/sphere/sphere.obj", false);

  APP_LOG_INFO("CREATED LAYER!, shader.id = {}", m_shader->m_id);
}

AppLayer::~AppLayer() { APP_LOG_INFO("DESTROYED APP LAYER!"); }

void AppLayer::on_render() {
  glEnable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_shader->use();

  glm::vec2 fb = core::Application::get_application().get_framebuffer_size();
  glViewport(0, 0, static_cast<uint32_t>(fb.x), static_cast<uint32_t>(fb.y));
  glm::mat4 model = glm::mat4(1.f);
  glm::mat4 view = m_camera->view_matrix();
  glm::mat4 projection = glm::perspective(glm::radians(m_camera->get_fov()), fb.x / fb.y, 0.1f, 1000.f);
  m_shader->setMatrix4f("view", view);
  m_shader->setMatrix4f("projection", projection);

  float time = core::Application::get_application().get_time();

  m_shader->use();

  m_shader->setMatrix4f("view", view);
  m_shader->setMatrix4f("projection", projection);
  for (size_t i = 0; i < m_lights.size(); ++i) {
    model = glm::translate(glm::mat4(1.f), m_lights[i]->get_position());
    m_shader->setMatrix4f("model", model);
    m_shader->setVec3("color", m_lights[i]->get_diffuse());
    m_light_sphere->draw(m_shader);
  }

  m_textured_shader->use();
  m_textured_shader->setVec3("camera_pos", m_camera->get_posistion());
  m_textured_shader->setInt("light_count", static_cast<int>(m_lights.size()));
  for (size_t i = 0; i < m_lights.size(); ++i) {
    const std::string light_name = "lights[" + std::to_string(i) + "]";
    m_textured_shader->setLight(light_name, m_lights[i]);
  }
  m_textured_shader->setMatrix4f("projection", projection);
  m_textured_shader->setMatrix4f("view", view);

  model = glm::translate(glm::mat4(1.f), glm::vec3(5.f, 2.f, -5.f));
  glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
  m_textured_shader->setMatrix4f("model", model);
  m_textured_shader->setMatrix3f("normal_matrix", normal_matrix);
  m_backpack->draw(m_textured_shader);

  m_textured_shader->use();
  model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -2.f));
  model = glm::scale(model, glm::vec3(0.1f));
  normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
  m_textured_shader->setMatrix4f("model", model);
  m_textured_shader->setMatrix3f("normal_matrix", normal_matrix);
  m_sponza->draw(m_textured_shader);
}

void AppLayer::on_event(core::Event &event) {
  core::EventDispatcher dispatcher(event);

  dispatcher.dispatch<core::MouseButtonReleasedEvent>([this](core::MouseButtonReleasedEvent &event) {
    return on_mouse_button_released(event);
  });

  dispatcher.dispatch<core::MouseButtonPressedEvent>([this](core::MouseButtonPressedEvent &event) {
    return on_mouse_button_pressed(event);
  });

  dispatcher.dispatch<core::MouseMovedEvent>([this](core::MouseMovedEvent &event) { return on_mouse_moved(event); });

  dispatcher.dispatch<core::MouseScrollEvent>([this](core::MouseScrollEvent &event) { return on_mouse_scrolled(event); });

  dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &event) { return on_key_pressed(event); });
  dispatcher.dispatch<core::KeyReleasedEvent>([this](core::KeyReleasedEvent &event) { return on_key_released(event); });
}

void AppLayer::on_update(float time_step) {
  ImGuiIO io = ImGui::GetIO();
  ImGuiLayer *imgui = core::Application::get_application().get_layer_single_instance<ImGuiLayer>();
  if (io.WantCaptureMouse && !imgui->is_suspended())
    return;

  m_delta_time = time_step;
  GLFWwindow *handle = core::Application::get_application().get_window()->get_handle();
  if (m_keys_pressed[GLFW_KEY_W])
    m_camera->move_towards(m_camera->get_front(), m_delta_time);
  if (m_keys_pressed[GLFW_KEY_S])
    m_camera->move_towards(-m_camera->get_front(), m_delta_time);
  if (m_keys_pressed[GLFW_KEY_A])
    m_camera->move_towards(-m_camera->get_right(), m_delta_time);
  if (m_keys_pressed[GLFW_KEY_D])
    m_camera->move_towards(m_camera->get_right(), m_delta_time);
  if (m_keys_pressed[GLFW_KEY_SPACE])
    m_camera->move_towards(m_camera->get_up(), m_delta_time);
  if (m_keys_pressed[GLFW_KEY_LEFT_SHIFT])
    m_camera->sprint();
  if (m_keys_pressed[GLFW_KEY_LEFT_SHIFT] == 0)
    m_camera->walk();
}

bool AppLayer::on_key_pressed(core::KeyPressedEvent &event) {
  m_keys_pressed[event.get_key_code()] = true;
  return true;
}

bool AppLayer::on_key_released(core::KeyReleasedEvent &event) {
  m_keys_pressed[event.get_key_code()] = false;
  return true;
}

bool AppLayer::on_mouse_scrolled(core::MouseScrollEvent &event) {
  m_camera->zoom(event.get_y_offset());
  return true;
}

bool AppLayer::on_mouse_button_released(core::MouseButtonReleasedEvent &event) {

  GLFWwindow *handle = core::Application::get_application().get_window()->get_handle();
  if (glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && m_is_mouse_button_held) {

    if (is_red())
      glClearColor(0.f, 0.3f, 0.f, 1.f);
    else
      glClearColor(0.3f, 0.f, 0.f, 1.f);
    m_is_red ^= true;
    m_is_mouse_button_held = false;
    return true;
  }

  return false;
}

bool AppLayer::on_mouse_button_pressed(core::MouseButtonPressedEvent &event) {
  GLFWwindow *handle = core::Application::get_application().get_window()->get_handle();
  if (glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
    m_is_mouse_button_held = true;
    return true;
  }
  return false;
}

bool AppLayer::on_mouse_moved(core::MouseMovedEvent &event) {

  if (!m_first_mouse) {
    m_mouse_pos.x = event.get_x();
    m_mouse_pos.y = event.get_y();
    m_first_mouse = true;
  }

  m_camera->look_to(event.get_x() - m_mouse_pos.x, m_mouse_pos.y - event.get_y());

  m_mouse_pos.x = event.get_x();
  m_mouse_pos.y = event.get_y();

  return true;
}
