# core

set(SOURCES
  vendor/stb/stb_image.h
  vendor/stb/stb_image.cpp
  src/core/window.h
  src/core/window.cpp
)

add_library(core STATIC)
target_sources(core PRIVATE ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(core glfw)
target_link_libraries(core glad)
target_link_libraries(core glm)

target_include_directories(core PUBLIC "src" "vendor/stb")
