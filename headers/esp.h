#pragma once
#include "proc.h"
#include "entity.h"
#include "overlay.h"
#include "geom.h"
#include <vector>

class ESP {
public:
    ESP(Process& target, Overlay& overlay);
    
    void Render();

private:
    Process& target;
    Overlay& overlay;

    void DrawBox(const Entity& ent, int localTeam, float matrix[16]);
    void DrawHealthBar(const Vec2& screenTop, float height, float width, int health);
};
