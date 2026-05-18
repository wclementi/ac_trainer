#pragma once
#include <cmath>
#include <numbers>

/**
 * @brief Represents a 2D vector.
 */
struct Vec2 {
    float x, y;
};

/**
 * @brief Represents a 3D vector and provides basic vector arithmetic.
 */
struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3& src) const { return {x + src.x, y + src.y, z + src.z}; }
    Vec3 operator-(const Vec3& src) const { return {x - src.x, y - src.y, z - src.z}; }
    Vec3 operator*(float src) const { return {x * src, y * src, z * src}; }
    Vec3 operator/(float src) const { return {x / src, y / src, z / src}; }
};

/**
 * @brief Represents a 4D vector, often used in matrices.
 */
struct Vec4 {
    float x, y, z, w;
};

/**
 * @brief Represents a 4x4 matrix, typically used for the View-Projection matrix.
 */
struct Matrix {
    float matrix[16];
};

/**
 * @brief Calculates the Euclidean distance between two 3D points.
 * @param src The source point.
 * @param dst The destination point.
 * @return Distance in game units.
 */
float GetDistance(const Vec3& src, const Vec3& dst);

/**
 * @brief Calculates the pitch and yaw angles required to aim from src to dst.
 * @param src The source point (usually the local player's camera position).
 * @param dst The destination point (usually the target's head).
 * @return A Vec3 containing the calculated Pitch (x) and Yaw (y) in degrees.
 */
Vec3 CalcAngle(const Vec3& src, const Vec3& dst);

/**
 * @brief Projects a 3D world coordinate to a 2D screen coordinate.
 * @param pos The 3D world position.
 * @param screen Output 2D screen coordinate if the function returns true.
 * @param matrix The 4x4 View-Projection matrix from the game.
 * @param windowWidth Width of the screen/window.
 * @param windowHeight Height of the screen/window.
 * @return True if the 3D point is visible on the screen, false otherwise.
 */
bool WorldToScreen(const Vec3& pos, Vec2& screen, const Matrix& matrix, int windowWidth, int windowHeight);
