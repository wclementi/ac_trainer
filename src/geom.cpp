#include "stdafx.h"
#include "geom.h"

float GetDistance(const Vec3& src, const Vec3& dst) {
    Vec3 diff = dst - src;
    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
}

Vec3 CalcAngle(const Vec3& src, const Vec3& dst) {
    Vec3 angle;
    
    // Assault Cube specific angle calculations
    angle.x = -std::atan2(dst.x - src.x, dst.y - src.y) * (180.0f / std::numbers::pi_v<float>) + 180.0f;
    angle.y = std::asin((dst.z - src.z) / GetDistance(src, dst)) * (180.0f / std::numbers::pi_v<float>);
    angle.z = 0.0f;
    
    return angle;
}
