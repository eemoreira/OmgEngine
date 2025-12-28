# core

set(SOURCES

  vendor/stb/stb_image.h
  vendor/stb/stb_image.cpp

  vendor/imgui/imconfig.h
  vendor/imgui/imgui.cpp
  vendor/imgui/imgui.h
  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_internal.h
  vendor/imgui/imgui_tables.cpp
  vendor/imgui/imgui_widgets.cpp
  vendor/imgui/imstb_rectpack.h
  vendor/imgui/imstb_textedit.h
  vendor/imgui/imstb_truetype.h
  vendor/imgui/backends/imgui_impl_opengl3.cpp
  vendor/imgui/backends/imgui_impl_glfw.cpp

  src/core/window.h
  src/core/window.cpp

  src/core/logger.h
  src/core/logger.cpp

  src/core/layer.h
  src/core/layer.cpp

  src/core/application.h
  src/core/application.cpp

  src/core/event.h
  src/core/window_events.h
  src/core/input_events.h

  src/core/renderer/shader.h
  src/core/renderer/shader.cpp

  src/core/renderer/camera.h
  src/core/renderer/camera.cpp

  src/core/renderer/model.h
  src/core/renderer/model.cpp

  src/core/renderer/mesh.h
  src/core/renderer/mesh.cpp

  src/core/renderer/light.h

)

add_library(core STATIC)
target_sources(core PRIVATE ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(core glfw)
target_link_libraries(core glad)
target_link_libraries(core glm)
target_link_libraries(core spdlog)
target_link_libraries(core assimp)

target_include_directories(core PUBLIC "src" "vendor/stb" "vendor/imgui" "vendor/imgui/backends")
