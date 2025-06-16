#include "entity.h"

#include <iostream>
#include <memory>
#include <optional>

#include "absl/strings/string_view.h"

namespace dennistwo::ecs {
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
static float3 ParentPosition(Entity *entity) {
    Entity *parent = entity->parent;
    if (parent == nullptr) {
        return dennistwo::math::ZERO3;
    }
    return parent->world_position;
}

void Entity::Render() {
    std::cout << "render " << identifier.value_or("") << std::endl;
    std::cout << "parent position" << ParentPosition(this) << std::endl;

    world_position = ParentPosition(this) + position;

    std::cout << "my position" << world_position << std::endl;
    for (Entity *child : children) {
        child->Render();
    }
}

}  // namespace dennistwo::ecs