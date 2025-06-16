# SETUP
cmake_minimum_required(VERSION 3.16)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# EXTERNAL LIBRARIES
add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)
add_subdirectory(libs/abslcpp EXCLUDE_FROM_ALL)
add_subdirectory(libs/glad/cmake EXCLUDE_FROM_ALL)
add_subdirectory(libs/glm EXCLUDE_FROM_ALL)
glad_add_library(glad_gl_core_41 SHARED EXCLUDE_FROM_ALL API gl:core=4.1)
# EXTERNAL DEV LIBS
add_subdirectory(libs/Catch2 EXCLUDE_FROM_ALL)

# ENABLE TESTING FOR INTERNAL CODE
enable_testing()

# INTERNAL LIBRARIES
include_directories(src)
add_subdirectory(src)
add_subdirectory(src/ecs)

add_library(utils src/utils.h src/utils.cc)
target_link_libraries(utils PRIVATE absl::string_view)

# EXECUTABLES
add_executable(${PROJECT_NAME} src/main.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 glad_gl_core_41)
target_link_libraries(${PROJECT_NAME} PRIVATE absl::statusor absl::strings)
target_link_libraries(${PROJECT_NAME} PRIVATE utils dennistwo_ecs)
