include_directories(../)

add_library(ecs entity.h)

add_executable(entity_test entity_test.cc)
add_test(NAME entity_test, COMMAND entity_test)
target_link_libraries(entity_test, Catch2WithMain)
