#include "entity_factory.h"

#include "entity.h"

namespace dennistwo::ecs {
Entity* Square(glm::vec3 position, glm::vec2 size, std::optional<absl::string_view> identifier) {
    Entity* square = new Entity(identifier);
}
}