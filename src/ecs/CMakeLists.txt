
add_library(dennistwo_ecs STATIC entity.h entity.cc)
target_link_libraries(dennistwo_ecs PRIVATE absl::string_view)
target_link_libraries(dennistwo_ecs PUBLIC glm::glm)

add_executable(entity_test entity_test.cc)
add_test(NAME EntityTest COMMAND entity_test)
target_link_libraries(entity_test PRIVATE Catch2WithMain dennistwo_ecs absl::strings)
