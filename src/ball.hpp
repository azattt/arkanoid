#pragma once

#include "draw_utilities.hpp"
#include "map.hpp"

class Ball
{
    public:
    int x, y, dx, dy, r;
    bool active;
    Ball(int x, int y, int r): x(x), y(y), r(r), dx(3), dy(4), active(false){}

    void move(BreakableRectangle* rectangles, int num_rectangles);
    void draw(Graphics graphics);

};