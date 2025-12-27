#pragma once

#include "core/event.h"
#include "core/input_events.h"
#include "core/layer.h"
#include "core/renderer/camera.h"
#include "core/renderer/model.h"
#include "core/renderer/shader.h"
#include <array>
#include <cstdint>

struct AppLayer : core::Layer {
public:
  AppLayer();
  virtual ~AppLayer();
  virtual void on_render() override;
  virtual void on_update(float time_step) override;
  virtual void on_event(core::Event &event) override;

  bool is_key_pressed(int32_t key_code) { return m_keys_pressed[key_code]; }
  bool is_red() { return m_is_red; }
  void reset_first_mouse() { m_first_mouse = false; }

private:
  uint32_t vao, vbo, ebo;
  bool on_mouse_button_pressed(core::MouseButtonPressedEvent &event);
  bool on_mouse_button_released(core::MouseButtonReleasedEvent &event);
  bool on_mouse_moved(core::MouseMovedEvent &event);
  bool on_mouse_scrolled(core::MouseScrollEvent &event);
  bool on_key_pressed(core::KeyPressedEvent &event);
  bool on_key_released(core::KeyReleasedEvent &event);
  bool m_is_red = false;
  bool m_is_mouse_button_held = false;
  bool m_first_mouse = false;
  float m_delta_time;
  glm::vec2 m_mouse_pos = glm::vec2(0.f);
  std::array<bool, 1024> m_keys_pressed = {false};

  std::unique_ptr<renderer::Camera> m_camera;

  // shaders
  std::unique_ptr<renderer::Shader> m_shader;
  std::unique_ptr<renderer::Shader> m_textured_shader;

  // models
  std::unique_ptr<renderer::Model> m_backpack;
  std::unique_ptr<renderer::Model> m_sponza;
};
