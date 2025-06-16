#include "math.h"

#include <ostream>

namespace dennistwo::math {
float3 float3::operator+(float3 other) {
    return {x + other.x, y + other.y, z + other.z};
}

std::ostream& operator<<(std::ostream& outs, const float3& value) {
    return outs << value.x << ", " << value.y << ", " << value.z;
}

}  // namespace dennistwo::math