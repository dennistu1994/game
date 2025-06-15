# SETUP
cmake_minimum_required(VERSION 3.16)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# EXTERNAL LIBRARIES
add_subdirectory(libs/SDL EXCLUDE_FROM_ALL)
add_subdirectory(libs/abseil-cpp EXCLUDE_FROM_ALL)

# INTERNAL LIBRARIES
add_library(utils src/utils.h src/utils.cc)
target_link_libraries(utils absl::string_view)


# EXECUTABLES
add_executable(${PROJECT_NAME} src/main.cc)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} absl::statusor absl::status absl::strings)
target_link_libraries(${PROJECT_NAME} utils)
