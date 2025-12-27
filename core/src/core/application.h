#pragma once

#include "event.h"
#include "layer.h"
#include "window.h"
#include <memory>
#include <string>
#include <vector>

namespace core {
  struct ApplicationSpec {
    std::string name;
    core::WindowSpec window_spec;
  };

  struct Application {
  public:
    Application(const ApplicationSpec &application_spec = ApplicationSpec());
    ~Application();

    void run();
    void stop();

    template <typename T>
      requires(std::is_base_of<Layer, T>::value)
    void push_layer() {
      m_layer_stack.emplace_back(std::make_unique<T>());
    }

    template <typename T>
      requires(std::is_base_of<Layer, T>::value)
    T *get_layer_single_instance() {
      for (const auto &layer : m_layer_stack)
        if (T* casted = dynamic_cast<T*>(layer.get())) return casted;
      return nullptr;
    }

    void raise_event(Event &event);

    glm::vec2 get_framebuffer_size() const;

    std::shared_ptr<Window> get_window() const { return m_window; }

    static Application &get_application();
    static float get_time();

  private:
    ApplicationSpec m_app_spec;
    bool m_running;
    std::shared_ptr<Window> m_window;
    std::vector<std::unique_ptr<Layer>> m_layer_stack;
  };
}
