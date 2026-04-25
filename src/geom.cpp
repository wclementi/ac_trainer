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

bool WorldToScreen(const Vec3& pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight) {
    // Matrix-vector multiplication (OpenGL style)
    Vec4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    // If clipCoords.w < 0.1f, the entity is behind us
    if (clipCoords.w < 0.1f)
        return false;

    // Perspective division to get Normalized Device Coordinates (NDC)
    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    // Transform NDC to window coordinates
    screen.x = (windowWidth / 2 * NDC.x) + (windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (windowHeight / 2);
    
    return true;
}
