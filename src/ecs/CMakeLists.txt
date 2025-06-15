include_directories(../)

add_library(ecs entity.h entity.cc)
target_link_libraries(ecs PRIVATE absl::string_view)

add_executable(entity_test entity_test.cc)
add_test(NAME EntityTest COMMAND entity_test)
target_link_libraries(entity_test PRIVATE Catch2WithMain ecs absl::string_view absl::strings)
