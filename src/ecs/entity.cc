#include "entity.h"

#include <iostream>
#include <memory>
#include <optional>

#include "absl/strings/string_view.h"
#include "glm/vec3.hpp"

namespace dennistwo::ecs {
using ::glm::vec3;
Entity *Entity::AddChild(entity_id identifier) {
    Entity *child = new Entity(identifier);
    child->parent = this;
    children.push_back(child);
    return child;
}

std::optional<absl::string_view> Entity::ID() {
    return identifier;
}

Entity::Entity(std::optional<absl::string_view> identifier) : identifier(identifier) {}

Entity::~Entity() {
    for (auto child : children) {
        delete child;
    }
}

// returns the entity's parent world position
static vec3 ParentPosition(Entity *entity) {
    Entity *parent = entity->parent;
    if (parent == nullptr) {
        return glm::vec3();
    }
    return parent->world_position;
}

void Entity::Render() {
    world_position = ParentPosition(this) + position;

    for (Entity *child : children) {
        child->Render();
    }
}

}  // namespace dennistwo::ecs