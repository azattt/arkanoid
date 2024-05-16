#pragma once
#include "game_structs.hpp"


class Graphics{
public:
    Graphics(int screenWidth, int screenHeight);
    void drawRectangle(WindowCoords coord, int w, int h, Color color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawRectangle(WindowCoordsRectangle coords, Color color = {1.0f, 1.0f, 1.0f, 1.0f});
    void drawRectangleWithTexture(WindowCoords coord, int w, int h, unsigned int textureID);
    void drawRectangleWithTexture(WindowCoordsRectangle coords, unsigned int textureID);
    void drawCircle(WindowCoords coord);

    int screenWidth, screenHeight;
};