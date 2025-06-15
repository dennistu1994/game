# SETUP
cmake_minimum_required(VERSION 3.16)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")

# EXTERNAL LIBRARIES
add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)
add_subdirectory(libs/abslcpp EXCLUDE_FROM_ALL)
add_subdirectory(libs/glad/cmake EXCLUDE_FROM_ALL)
glad_add_library(glad_gl_core_41 SHARED EXCLUDE_FROM_ALL API gl:core=4.1)

# INTERNAL LIBRARIES
add_library(utils src/utils.h src/utils.cc)
target_link_libraries(utils PRIVATE absl::string_view)

# EXECUTABLES
add_executable(${PROJECT_NAME} src/main.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE glad_gl_core_41)
target_link_libraries(${PROJECT_NAME} PRIVATE absl::statusor absl::status absl::strings)
target_link_libraries(${PROJECT_NAME} PRIVATE utils)

# TESTS
add_subdirectory(libs/Catch2 EXCLUDE_FROM_ALL)
add_executable(entity_test src/ecs/entity_test.cc)
target_link_libraries(entity_test, Catch2WithMain)