#pragma once

#include "core/layer.h"

struct ImGuiLayer : core::Layer {
public:
  ImGuiLayer();
  virtual ~ImGuiLayer();
  virtual void on_render() override;
  virtual void suspend() override;
  virtual void unsuspend() override;
  virtual bool is_suspended() override;
  //  virtual void on_update(float time_step) override;
  virtual void on_event(core::Event &event) override;

private:
  bool m_is_suspended = false;
};
