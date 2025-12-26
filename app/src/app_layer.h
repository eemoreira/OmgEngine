#pragma once

#include "core/event.h"
#include "core/input_events.h"
#include "core/layer.h"
#include "core/renderer/shader.h"
#include <cstdint>

struct AppLayer : core::Layer {
public:
  AppLayer();
  virtual ~AppLayer();
  virtual void on_render() override;
  virtual void on_update() override;
  virtual void on_event(core::Event &event) override;

  bool is_red() { return m_is_red; }

private:
  uint32_t vao, vbo, ebo;
  bool on_mouse_button_pressed(core::MouseButtonPressedEvent &event);
  bool on_mouse_button_released(core::MouseButtonReleasedEvent &event);
  bool m_is_red;
  bool m_is_mouse_button_held;
  std::unique_ptr<renderer::Shader> m_shader;
};
