#define GLM_ENABLE_EXPERIMENTAL
#include "entity.h"

#include <absl/strings/str_cat.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "utils.h"

namespace {
using Catch::Approx;
using dennistwo::Print;
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

TEST_CASE("Entity_WorldTransform") {
    Entity entity("parent");
    Entity* child = entity.AddChild("child");

    entity.position.x = 3.0;
    entity.UpdateLocalTransform();
    child->position.y = 5.0;
    child->UpdateLocalTransform();

    entity.UpdateWorldTransformRecursive();
    glm::vec3 childWorldTranslation = glm::vec3(child->worldTransform[3]);
    REQUIRE(
        glm::all(
            glm::epsilonEqual(childWorldTranslation, glm::vec3(3.f, 5.f, 0.f), glm::epsilon<float>())));
}
}  // namespace