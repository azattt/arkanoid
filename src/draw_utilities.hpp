#pragma once
#include "game_structs.hpp"


class Graphics{
public:
    Graphics(int screenWidth, int screenHeight);
    float toNDC_x(int x);
    float toNDC_y(int y);
    int fromNDC_x(float x);
    int fromNDC_y(float y);
    void drawRectangle(WindowCoords coord, int w, int h, Color color = {1.0f, 1.0f, 1.0f, 1.0f}, int angle = 0);
    void drawRectangle(WindowCoordsRectangle coords, Color color = {1.0f, 1.0f, 1.0f, 1.0f}, int angle = 0);
    void drawRectangleWithTexture(WindowCoords coord, int w, int h, unsigned int textureID, int angle = 0);
    void drawRectangleWithTexture(WindowCoordsRectangle coords, unsigned int textureID, int angle = 0);
    void drawCircle(WindowCoords coord, const Color color, unsigned int radius, unsigned int vert_count);

    int screenWidth, screenHeight;
};