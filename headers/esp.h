#pragma once
#include "game.h"
#include "overlay.h"
#include "imgui.h"
#include <vector>

class ESP {
public:
    ESP(Game& game, Overlay& overlay);
    
    void Render();

private:
    Game& game;
    Overlay& overlay;

    void DrawBox(const Entity& ent, int localTeam, const Matrix& matrix);
    void DrawHealthBar(const Vec2& screenTop, float height, float width, int health);
};
