#pragma once

#include "draw_utilities.hpp"
#include "map.hpp"

class Ball
{
    public:
    int r;
    float x, y, dx, dy;
    bool active;
    unsigned int textureID;
    Ball(int x, int y, int r);

    void initializeTexture();

    void move(BreakableRectangle* rectangles, int num_rectangles);
    void draw(Graphics graphics);

};