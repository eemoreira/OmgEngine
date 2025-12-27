#include "imgui_layer.h"
#include "core/application.h"
#include "core/event.h"
#include "core/input_events.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/gl.h>

ImGuiLayer::ImGuiLayer() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(core::Application::get_application().get_window()->get_handle(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

ImGuiLayer::~ImGuiLayer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  APP_LOG_INFO("DESTROYED IMGUI LAYER!");
}

void ImGuiLayer::on_render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::on_event(core::Event &event) {

  if (is_suspended())
    return;

  core::EventDispatcher dispatcher(event);
  dispatcher.dispatch<core::MouseButtonPressedEvent>([this](core::MouseButtonPressedEvent &e) { return true; });
  dispatcher.dispatch<core::MouseButtonReleasedEvent>([this](core::MouseButtonReleasedEvent &e) { return true; });
  dispatcher.dispatch<core::MouseMovedEvent>([this](core::MouseMovedEvent &e) { return true; });
  dispatcher.dispatch<core::MouseScrollEvent>([this](core::MouseScrollEvent &e) { return true; });
  dispatcher.dispatch<core::KeyPressedEvent>([this](core::KeyPressedEvent &e) { return true; });
  dispatcher.dispatch<core::KeyReleasedEvent>([this](core::KeyReleasedEvent &e) { return true; });
}

void ImGuiLayer::suspend() {
  m_is_suspended = true;
  GLFWwindow *win = core::Application::get_application().get_window()->get_handle();
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ImGuiLayer::unsuspend() {
  m_is_suspended = false;
  GLFWwindow *win = core::Application::get_application().get_window()->get_handle();
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool ImGuiLayer::is_suspended() { return m_is_suspended; }
