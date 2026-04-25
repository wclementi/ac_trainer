#pragma once

#include <cstddef>

namespace offsets {
    // Base pointers (Relative to ac_client.exe base address)
    // Updated for Assault Cube v1.3.0.2
    constexpr std::ptrdiff_t localPlayer = 0x17E0A8;
    constexpr std::ptrdiff_t entityList = 0x18AC04;
    constexpr std::ptrdiff_t playerCount = 0x18AC0C;
    constexpr std::ptrdiff_t fov = 0x18A7CC;

    // Player Entity Offsets (Relative to LocalPlayer or Entity Base)
    constexpr std::ptrdiff_t headPos = 0x4;        // Vec3
    constexpr std::ptrdiff_t feetPos = 0x28;       // Vec3 (Z: 0x28, X: 0x2C, Y: 0x30)
    constexpr std::ptrdiff_t viewAngles = 0x34;    // Vec2 (X: 0x34, Y: 0x38)
    constexpr std::ptrdiff_t health = 0xEC;        // int
    constexpr std::ptrdiff_t armor = 0xF0;         // int
    constexpr std::ptrdiff_t autoShoot = 0x204;    // int/bool
    constexpr std::ptrdiff_t name = 0x205;         // char[16]
    constexpr std::ptrdiff_t team = 0x30C;         // int

    // Weapon / Ammo Offsets (Relative to LocalPlayer)
    constexpr std::ptrdiff_t pistolAmmo = 0x12C;
    constexpr std::ptrdiff_t carbineAmmo = 0x130;
    constexpr std::ptrdiff_t shotgunAmmo = 0x134;
    constexpr std::ptrdiff_t smgAmmo = 0x138;
    constexpr std::ptrdiff_t sniperAmmo = 0x13C;
    constexpr std::ptrdiff_t arAmmo = 0x140;
    constexpr std::ptrdiff_t grenadeAmmo = 0x144;

    // Weapon Fast Fire (Delay/Cooldown) Offsets
    constexpr std::ptrdiff_t shotgunFastFire = 0x158;
    constexpr std::ptrdiff_t sniperFastFire = 0x160;
    constexpr std::ptrdiff_t arFastFire = 0x164;
}
