#pragma once

#include "game_structs.hpp"

struct BreakableRectangle{
    WindowCoordsRectangle rect;
    int durability;
    Color color;
};