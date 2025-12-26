#pragma once

#include "event.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <functional>
#include <glm/glm.hpp>
#include <string>

namespace core {
  struct WindowSpec {
    std::string title;
    uint32_t width = 720;
    uint32_t height = 1280;
    bool is_resizeable = true;
    bool v_sync = true;
    std::function<void(Event &)> event_callback;

    // TODO: add window events callback
  };

  struct Window {
  public:
    Window(const WindowSpec &win_spec = WindowSpec());
    ~Window();

    void create();
    void destroy();
    void update();
    glm::vec2 get_mouse_pos();
    glm::vec2 get_framebuffer_size();
    bool should_close();
    void raise_event(Event &event);
    GLFWwindow* get_handle();

  private:
    WindowSpec m_window_spec;
    GLFWwindow *m_handle = nullptr;
  };

}
