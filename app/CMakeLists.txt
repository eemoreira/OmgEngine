# app

set(SOURCES
  src/main.cpp

  src/app_layer.cpp
  src/app_layer.h

  src/imgui_layer.cpp
  src/imgui_layer.h

  src/overlay_layer.cpp
  src/overlay_layer.h
)

add_executable(app)

target_sources(app PRIVATE ${SOURCES})


add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(app core)

target_include_directories(app PRIVATE src)

add_custom_target(copy_shaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/app/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders"
)
add_dependencies(app copy_shaders)
