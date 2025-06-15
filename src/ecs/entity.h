#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "absl/strings/string_view.h"

namespace dennistwo {
typedef std::optional<absl::string_view> entity_id;
class Entity {
   private:
    std::optional<std::string> identifier;

   public:
    std::vector<Entity *> children;
    Entity *parent;

    std::optional<absl::string_view> ID();

    // constructor
    Entity(entity_id identifier = std::nullopt);

    ~Entity();

    // instantiate a new children and add it to this node
    Entity *AddChild(entity_id identifier = std::nullopt);
};
}  // namespace dennistwo