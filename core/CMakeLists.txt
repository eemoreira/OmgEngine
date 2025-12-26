# core

set(SOURCES

  vendor/stb/stb_image.h
  vendor/stb/stb_image.cpp

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

target_include_directories(core PUBLIC "src" "vendor/stb")
