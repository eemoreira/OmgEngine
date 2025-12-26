#include "window.h"
#include "input_events.h"
#include "logger.h"
#include "window_events.h"
#include <glad/gl.h>
#include <iostream>

namespace core {

  Window::Window(const WindowSpec &win_spec) : m_window_spec(win_spec) { }

  Window::~Window() { destroy(); }

  void Window::create() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    m_handle =
        glfwCreateWindow(m_window_spec.width, m_window_spec.height, m_window_spec.title.c_str(), nullptr, nullptr);

    if (!m_handle) assert(false && "ERROR CREATING WINDOW");

    glfwMakeContextCurrent(m_handle);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(m_window_spec.v_sync ? 1 : 0);
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetMouseButtonCallback(m_handle, [](GLFWwindow *win, int32_t button, int32_t action, int32_t mods) {
      Window *window = (Window *)glfwGetWindowUserPointer(win);

      switch (action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        window->raise_event(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        window->raise_event(event);
        break;
      }
      default: {
        CORE_LOG_WARN("UNSUPPORTED MOUSE BUTTON EVENT");
      }
      }
    });

    glfwSetKeyCallback(m_handle, [](GLFWwindow *win, int32_t keycode, int32_t scancode, int32_t action, int32_t mods) {
      Window *window = (Window *)glfwGetWindowUserPointer(win);
      switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(keycode, false);
        window->raise_event(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(keycode, true);
        window->raise_event(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(keycode);
        window->raise_event(event);
        break;
      }
      }
    });

    glfwSetScrollCallback(m_handle, [](GLFWwindow *win, double x_offset, double y_offset) {
      Window *window = (Window *)glfwGetWindowUserPointer(win);
      MouseScrollEvent event(x_offset, y_offset);
      window->raise_event(event);
    });

    glfwSetCursorPosCallback(m_handle, [](GLFWwindow *win, double x, double y) {
      Window *window = (Window *)glfwGetWindowUserPointer(win);
      MouseMovedEvent event(x, y);
      window->raise_event(event);
    });

    glfwSetWindowSizeCallback(m_handle, [](GLFWwindow *win, int32_t width, int32_t height) {
      Window *window = (Window *)glfwGetWindowUserPointer(win);
      WindowResizedEvent event(width, height);
      window->raise_event(event);
    });

    glfwSetWindowCloseCallback(m_handle, [](GLFWwindow *win) {
      Window *window = (Window *)glfwGetWindowUserPointer(win);
      WindowClosedEvent event;
      window->raise_event(event);
    });

    std::cout << "WINDOW BUILT" << std::endl;
  }

  void Window::update() { glfwSwapBuffers(m_handle); }

  void Window::destroy() {
    if (m_handle) {
      glfwDestroyWindow(m_handle);
      m_handle = nullptr;
    }
  }

  glm::vec2 Window::get_framebuffer_size() {
    int32_t width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);
    return glm::vec2(width, height);
  }

  glm::vec2 Window::get_mouse_pos() {
    double x, y;
    glfwGetCursorPos(m_handle, &x, &y);
    return glm::vec2(x, y);
  }

  bool Window::should_close() { return glfwWindowShouldClose(m_handle); }

  void Window::raise_event(Event &event) {
    if (m_window_spec.event_callback) {
      m_window_spec.event_callback(event);
    }
  }

  GLFWwindow* Window::get_handle() { return m_handle; }

}
