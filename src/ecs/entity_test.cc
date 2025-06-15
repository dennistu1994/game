#include "entity.h"

#include <catch2/catch_test_macros.hpp>

#include "absl/strings/str_cat.h"

using dennistwo::Entity;

TEST_CASE("Entity_SmokeTest") {
    Entity entity;
    REQUIRE(entity.children.size() == 0);
    entity.AddChild();
    REQUIRE(entity.children.size() == 1);
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