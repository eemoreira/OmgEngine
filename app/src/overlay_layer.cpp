#include "overlay_layer.h"
#include "app_layer.h"
#include "core/application.h"
#include "imgui_layer.h"

OverlayLayer::OverlayLayer() { }
OverlayLayer::~OverlayLayer() { APP_LOG_INFO("DESTROYED OVERLAY LAYER!"); }

void OverlayLayer::on_event(core::Event &event) {
  core::EventDispatcher dispatcher(event);
  dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &event) { return on_key_pressed(event); });
  dispatcher.dispatch<core::KeyReleasedEvent>([this](core::KeyReleasedEvent &event) { return on_key_released(event); });
}

bool OverlayLayer::on_key_pressed(core::KeyPressedEvent &event) {
  if (event.get_key_code() == GLFW_KEY_F1) {
    m_is_suspend_key_pressed = true;
    return true;
  }
  return false;
}

bool OverlayLayer::on_key_released(core::KeyReleasedEvent &event) {
  if (event.get_key_code() == GLFW_KEY_F1 && m_is_suspend_key_pressed) {
    m_is_suspend_key_pressed = false;

    ImGuiLayer *imgui_layer = core::Application::get_application().get_layer_single_instance<ImGuiLayer>();
    AppLayer *app_layer = core::Application::get_application().get_layer_single_instance<AppLayer>();

    if (!imgui_layer->is_suspended()) {
      APP_LOG_INFO("SUSPENDING IMGUI LAYER");
      imgui_layer->suspend();
      app_layer->reset_first_mouse();
    } else {
      imgui_layer->unsuspend();
    }

    return true;
  }
  return false;
}
