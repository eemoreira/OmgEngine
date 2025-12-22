# Dependecies

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw3
  DOWNLOAD_EXTRACT_TIMESTAMP OFF
  URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
)

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
FetchContent_Declare(
    glad
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/Dav1dde/glad/archive/refs/tags/v2.0.8.zip
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6)
endif()
set_target_properties(glad PROPERTIES FOLDER "dependencies")
  
# GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_GetProperties(glm)
if (NOT glm_POPULATED) 
  set(FETCHCONTENT_QUIET NO)
  FetchContent_Populate(glm)
add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

set_target_properties(glm PROPERTIES FOLDER "dependencies")
