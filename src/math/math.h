#pragma once

#include <ostream>

namespace dennistwo::math {
struct float3 {
    float x;
    float y;
    float z;

    float3 operator+(float3 other);
};

std::ostream& operator<<(std::ostream& outs, const float3& value);

constexpr float3 ZERO3{};
}  // namespace dennistwo::math