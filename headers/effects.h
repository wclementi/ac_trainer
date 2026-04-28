#pragma once
#include "geom.h"
#include <vector>
#include <string>

struct FloatingText {
    Vec3 worldPos;
    int value;
    float startTime;
    float alpha = 1.0f;
};

class VisualEffects {
public:
    void AddDamage(Vec3 pos, int amount);
    void TriggerHitmarker();
    
    void Update(float deltaTime);
    void Render(const Matrix& viewMatrix, int screenWidth, int screenHeight);

private:
    std::vector<FloatingText> damageNumbers;
    float hitmarkerAlpha = 0.0f;
    const float hitmarkerDuration = 0.5f;
    const float floatingTextDuration = 1.5f;
};
