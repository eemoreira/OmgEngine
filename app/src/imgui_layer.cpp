#include "imgui_layer.h"
#include "app_layer.h"
#include "core/application.h"
#include "core/event.h"
#include "core/input_events.h"
#include "core/renderer/light.h"
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

  static int selected_light = -1;
  ImGui::Begin("Light Controller");
  ImGui::Text("Select Light:");
  core::Application &app = core::Application::get_application();
  auto app_layer = app.get_layer_single_instance<AppLayer>();
  const auto &lights = app_layer->get_lights();
  for (size_t i = 0; i < lights.size(); ++i) {
    std::string light_label = "Light " + std::to_string(i);
    ;
    if (ImGui::Selectable(light_label.c_str(), selected_light == static_cast<int>(i)))
      selected_light = static_cast<int>(i);
  }

  std::shared_ptr<renderer::Light> light_to_edit = nullptr;
  if (selected_light >= 0 && selected_light < static_cast<int>(lights.size()))
    light_to_edit = lights[selected_light];

  if (light_to_edit) {
    ImGui::Separator();
    ImGui::Text("Edit Light %d", selected_light);

    glm::vec3 position = light_to_edit->get_position();
    glm::vec3 ambient = light_to_edit->get_ambient();
    glm::vec3 diffuse = light_to_edit->get_diffuse();
    glm::vec3 specular = light_to_edit->get_specular();
    float constant = light_to_edit->get_constant();
    float linear = light_to_edit->get_linear();
    float quadratic = light_to_edit->get_quadratic();

    ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
    ImGui::ColorEdit3("Ambient", glm::value_ptr(ambient));
    ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuse));
    ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
    ImGui::DragFloat("Constant", &constant, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Linear", &linear, 0.001f, 0.0f, 1.0f);
    ImGui::DragFloat("Quadratic", &quadratic, 0.0001f, 0.0f, 1.0f);

    light_to_edit->move_to(position);
    light_to_edit->set_ambient(ambient);
    light_to_edit->set_diffuse(diffuse);
    light_to_edit->set_specular(specular);
    light_to_edit->set_constant(constant);
    light_to_edit->set_linear(linear);
    light_to_edit->set_quadratic(quadratic);
  } else {
    ImGui::Text("No light selected.");
  }

  ImGui::Separator();
  ImGui::BeginChild("Add New Light");
  static glm::vec3 new_light_position = glm::vec3(0.f);
  static glm::vec3 new_light_ambient = glm::vec3(0.1f);
  static glm::vec3 new_light_diffuse = glm::vec3(1.f);
  static glm::vec3 new_light_specular = glm::vec3(1.f);
  static float new_light_constant = 1.f;
  static float new_light_linear = 0.09f;
  static float new_light_quadratic = 0.032f;
  ImGui::Text("Add New Light");
  ImGui::DragFloat3("Position##new", glm::value_ptr(new_light_position), 0.1f);
  ImGui::ColorEdit3("Ambient##new", glm::value_ptr(new_light_ambient));
  ImGui::ColorEdit3("Diffuse##new", glm::value_ptr(new_light_diffuse));
  ImGui::ColorEdit3("Specular##new", glm::value_ptr(new_light_specular));
  ImGui::DragFloat("Constant##new", &new_light_constant, 0.01f, 0.0f, 10.0f);
  ImGui::DragFloat("Linear##new", &new_light_linear, 0.001f, 0.0f, 1.0f);
  ImGui::DragFloat("Quadratic##new", &new_light_quadratic, 0.0001f, 0.0f, 1.0f);
  if (ImGui::Button("Add Light")) {
    app_layer->create_light(
        new_light_position,
        new_light_ambient,
        new_light_diffuse,
        new_light_specular,
        new_light_constant,
        new_light_linear,
        new_light_quadratic
    );
  }
  ImGui::EndChild();

  ImGui::End();

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
