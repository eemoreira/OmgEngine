#include "application.h"
#include "logger.h"

namespace core {

  static Application *s_application = nullptr;

  void glfw_error_callback(int error, const char *msg) { CORE_LOG_ERROR("[GLFW]" + std::string(msg)); }

  Application::Application(const ApplicationSpec &app_spec) : m_app_spec(app_spec) {
    s_application = this;

    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();

    if (m_app_spec.window_spec.title.empty()) m_app_spec.window_spec.title = m_app_spec.name;
    m_app_spec.window_spec.event_callback = [this](Event &event) { raise_event(event); };

    m_window = std::make_shared<Window>(m_app_spec.window_spec);
    m_window->create();
  }

  Application::~Application() {
    m_layer_stack.clear();
    m_window->destroy();
    glfwTerminate();
    s_application = nullptr;
  }

  void Application::run() {

    float last_time = get_time();

    while (m_running) {
      glfwPollEvents();

      if (m_window->should_close()) {
        stop();
        break;
      }

      float now_time = get_time();
      float time_step = now_time - last_time;
      time_step = glm::clamp(time_step, 0.001f, 0.1f);
      last_time = now_time;

      for (const std::unique_ptr<Layer> &layer : m_layer_stack)
        if (!layer->is_suspended()) layer->on_update(time_step);

      for (const std::unique_ptr<Layer> &layer : m_layer_stack)
        if (!layer->is_suspended()) layer->on_render();

      m_window->update();
    }
  }

  void Application::raise_event(Event &event) {
    for (int i = int(m_layer_stack.size() - 1); i >= 0; i--) {
      m_layer_stack[i]->on_event(event);
      if (event.handled) {
        CORE_LOG_WARN("EVENT {} HANDLED BY INDEX {}", event.to_string(), i+1);
        event.handled = true;
        break;
      }
    }
  }

  void Application::stop() { m_running = false; }

  float Application::get_time() { return glfwGetTime(); }

  Application &Application::get_application() { return *s_application; }

  glm::vec2 Application::get_framebuffer_size() const { return m_window->get_framebuffer_size(); }

}
