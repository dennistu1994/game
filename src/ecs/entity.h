#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "absl/strings/string_view.h"
#include "math/math.h"

namespace dennistwo::ecs {

using dennistwo::math::float3;

typedef std::optional<absl::string_view> entity_id;

class Entity {
   private:
    std::optional<std::string> identifier;

   public:
    float3 position;        // local position
    float3 world_position;  // derived from position and parent world_position
    std::vector<Entity *> children;
    Entity *parent;

    std::optional<absl::string_view> ID();

    // constructor
    Entity(entity_id identifier = std::nullopt);

    ~Entity();

    // instantiate a new children and add it to this node
    Entity *AddChild(entity_id identifier = std::nullopt);

    void Render();
};
}  // namespace dennistwo::ecs