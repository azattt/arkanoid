#pragma once

#include "draw_utilities.hpp"

class Ball
{
    public:
    int x, y, dx, dy, r;
    bool active;
    Ball(int x, int y, int r): x(x), y(y), r(r), dx(2), dy(3), active(false){}

    void move(WindowCoordsRectangle* collision_rectangles, int num_rectangles);
    void draw(Graphics graphics);

};