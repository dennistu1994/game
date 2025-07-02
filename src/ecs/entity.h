#pragma once
#include <absl/strings/string_view.h>

#include <glm/glm.hpp>
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
    void UpdateWorldTransform();

   public:
    Entity(entity_id identifier = std::nullopt);
    ~Entity();
    Entity *AddChild(entity_id identifier = std::nullopt);
    void AddChild(Entity *child);

    ::glm::vec3 position = ::glm::vec3(0.f);
    ::glm::vec3 scale = ::glm::vec3(1.f);
    ::glm::mat4 rotation = ::glm::mat4(1.f);
    ::glm::mat4 localTransform;
    ::glm::mat4 worldTransform;  // calculated recursively during pre-render
    std::vector<Entity *> children;
    Entity *parent = nullptr;

    std::optional<absl::string_view> ID();

    void Render();

    void UpdateLocalTransform();
    void UpdateWorldTransformRecursive();
};
}  // namespace dennistwo::ecs