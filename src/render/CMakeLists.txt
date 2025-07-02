
add_library(dennistwo_render STATIC render_system.h render_system.cc)
target_link_libraries(dennistwo_render PRIVATE absl::strings)
target_link_libraries(dennistwo_render PRIVATE glad_gl_core_41 glm::glm)
target_link_libraries(dennistwo_render PRIVATE dennistwo_utils)

