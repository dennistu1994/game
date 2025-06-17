#pragma once
#include <absl/strings/string_view.h>

#include <glm/vec3.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace dennistwo::ecs {

typedef std::optional<absl::string_view>
    entity_id;

class Entity {
   private:
    std::optional<std::string> identifier;

   public:
    ::glm::vec3 position;        // local position
    ::glm::vec3 world_position;  // derived from position and parent world_position
    std::vector<Entity *> children;
    Entity *parent = nullptr;

    std::optional<absl::string_view> ID();

    // constructor
    Entity(entity_id identifier = std::nullopt);

    ~Entity();

    // instantiate a new children and add it to fthis node
    Entity *AddChild(entity_id identifier = std::nullopt);

    void Render();
};
}  // namespace dennistwo::ecs