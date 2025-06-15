#pragma once
#include <vector>
#include <memory>

class Entity {
    public:
        std::vector<std::unique_ptr<Entity>> children;
};
