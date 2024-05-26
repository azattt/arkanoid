#pragma once

#include <vector>

#include "draw_utilities.hpp"
#include "map.hpp"



class Ball
{
    public:
    int index;
    float x, y, r, dx, dy;
    unsigned int textureID;
    Ball(float x, float y, float r, float dx, float dy);

    void initializeTexture();

    void move(std::vector<BreakableRectangle>& rectangles);
    void draw(Graphics& graphics);

};