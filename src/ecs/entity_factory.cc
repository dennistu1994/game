#include "entity_factory.h"

#include "entity.h"

namespace dennistwo::ecs {
Entity* TestEntity(glm::vec3 position) {
    Entity* entity = new Entity();
    return entity;
}

}  // namespace dennistwo::ecs