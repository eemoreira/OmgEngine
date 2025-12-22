#include "core/window.h"

int main() {
  glfwInit();
  core::Window *window = new core::Window();
  window->create();
  while (!window->should_close()) glfwPollEvents();
  return 0;
}
