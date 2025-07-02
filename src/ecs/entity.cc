#include "entity.h"

#include <absl/strings/string_view.h>

#include <glm/ext.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <optional>

namespace dennistwo::ecs {
using ::glm::vec3;
Entity *Entity::AddChild(entity_id identifier) {
    Entity *child = new Entity(identifier);
    child->parent = this;
    children.push_back(child);
    return child;
}

void Entity::AddChild(Entity *child) {
    child->parent = this;
    children.push_back(child);
}

std::optional<absl::string_view> Entity::ID() {
    return identifier;
}

Entity::Entity(std::optional<absl::string_view> identifier) : identifier(identifier) {
    UpdateLocalTransform();
};

Entity::~Entity() {
    for (auto child : children) {
        delete child;
    }
}

// returns the entity's parent world position
static glm::mat4 ParentWorldTransform(Entity *entity) {
    Entity *parent = entity->parent;
    if (parent == nullptr) {
        return glm::mat4(1.f);
    }
    return parent->worldTransform;
}

void Entity::UpdateWorldTransform() {
    worldTransform = ParentWorldTransform(this) * localTransform;
}

void Entity::UpdateWorldTransformRecursive() {
    UpdateWorldTransform();
    for (Entity *child : children) {
        child->UpdateWorldTransform();
    }
}

void Entity::Render() {
    // push mesh to VBO
    // push uniform for world projection
    for (Entity *child : children) {
        child->Render();
    }
}

void Entity::UpdateLocalTransform() {
    localTransform = glm::translate(rotation * glm::scale(glm::mat4(1.f), scale), position);
}

}  // namespace dennistwo::ecs