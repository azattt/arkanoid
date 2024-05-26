#pragma once

#include <vector>
#include "game_structs.hpp"

enum BonusType
{
    DoubleBalls,
    DoublePlatform,
    BallCapture,
    ClearPenalties,
    ClearAll,

    HalfPlatform,
    InvertedControls
};

struct Bonus
{
    float x, y;
    BonusType bonus_type;
};

struct BreakableRectangle
{
    WindowCoordsRectangle rect;
    int durability;
    Color color;
    BonusType bonus_type;
};

extern std::vector<Bonus> bonuses;
