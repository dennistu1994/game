#include "entity.h"

#include <absl/strings/str_cat.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <glm/vec3.hpp>

namespace {
using Catch::Approx;
using dennistwo::ecs::Entity;
using ::glm::vec3;

TEST_CASE("Entity_SmokeTest") {
    Entity entity;
    REQUIRE(entity.children.size() == 0);
    entity.AddChild();
    REQUIRE(entity.children.size() == 1);
    REQUIRE(entity.position.x == 0);
}

TEST_CASE("Entity_Identifier") {
    Entity entity("parent");
    REQUIRE(entity.children.size() == 0);
    Entity* child = entity.AddChild("child");
    REQUIRE(entity.children.size() == 1);

    REQUIRE(entity.ID().has_value());
    REQUIRE(entity.ID().value() == "parent");
    REQUIRE(child->ID().has_value());
    REQUIRE(child->ID().value() == "child");
}

TEST_CASE("Entity_WorldPosition") {
    Entity entity("parent");
    Entity* child = entity.AddChild("child");
    entity.position.x = 3.0;
    child->position.y = 5.0;

    entity.Render();
    REQUIRE(Approx(entity.world_position.x) == 3.0);
    REQUIRE(Approx(child->world_position.x) == 3.0);
    REQUIRE(Approx(child->world_position.y) == 5.0);
}
}  // namespace