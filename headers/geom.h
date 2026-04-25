#pragma once
#include <cmath>
#include <numbers>

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;

    Vec3 operator+(const Vec3& src) const { return {x + src.x, y + src.y, z + src.z}; }
    Vec3 operator-(const Vec3& src) const { return {x - src.x, y - src.y, z - src.z}; }
    Vec3 operator*(float src) const { return {x * src, y * src, z * src}; }
    Vec3 operator/(float src) const { return {x / src, y / src, z / src}; }
};

struct Vec4 {
    float x, y, z, w;
};

struct Matrix {
    float matrix[16];
};

float GetDistance(const Vec3& src, const Vec3& dst);
Vec3 CalcAngle(const Vec3& src, const Vec3& dst);
bool WorldToScreen(const Vec3& pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight);
