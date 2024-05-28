#pragma once
#include "game_structs.hpp"

class Graphics
{
public:
    Graphics(int screenWidth, int screenHeight);
    void drawRectangle(WindowCoords coord, int w, int h, Color color = {1.0f, 1.0f, 1.0f, 1.0f}, int angle = 0) const;
    void drawRectangle(WindowCoordsRectangle coords, Color color = {1.0f, 1.0f, 1.0f, 1.0f}, int angle = 0) const;
    void drawRectangleWithTexture(WindowCoords coord, int w, int h, unsigned int textureID, bool flipTexture = false, Color color = {1.0f, 1.0f, 1.0f, 1.0f}, int angle = 0) const;
    void drawRectangleWithTexture(WindowCoordsRectangle coords, unsigned int textureID, bool flipTexture = false, Color color = {1.0f, 1.0f, 1.0f, 1.0f}, int angle = 0) const;
    void drawCircle(WindowCoords coord, const Color color, unsigned int radius, unsigned int vert_count) const;
    void setClearColor(Color color);
    Color getClearColor(Color color) const;

    int screenWidth, screenHeight;
    private:
    Color clearColor;
};