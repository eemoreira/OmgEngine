#include "window.h"
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

  bool Window::should_close() {
    return glfwWindowShouldClose(m_handle);
  }

}
