#include "entity.h"

#include <memory>
#include <optional>

#include "absl/strings/string_view.h"

namespace dennistwo {
Entity *Entity::AddChild(entity_id identifier) {
    Entity *child = new Entity(identifier);
    child->parent = this;
    children.push_back(child);
    return child;
}

std::optional<absl::string_view> Entity::ID() {
    return identifier;
}

Entity::Entity(std::optional<absl::string_view> identifier) {
    this->identifier = identifier;
}

Entity::~Entity() {
    for (auto child : children) {
        delete child;
    }
}
}  // namespace dennistwo