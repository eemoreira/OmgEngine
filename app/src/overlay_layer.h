#pragma once

#include "core/event.h"
#include "core/input_events.h"
#include "core/layer.h"
#include <glm/glm.hpp>

struct OverlayLayer : core::Layer {
public:
  OverlayLayer();
  virtual ~OverlayLayer();
  virtual void on_event(core::Event &event) override;

private:
  bool on_key_pressed(core::KeyPressedEvent &event);
  bool on_key_released(core::KeyReleasedEvent &event);
  bool m_is_suspend_key_pressed = false;
};
