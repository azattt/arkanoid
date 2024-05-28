#include "draw_utilities.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

#include <GL/gl.h>

#include "game_structs.hpp"

constexpr float PI = 3.14159265359;

Graphics::Graphics(int screenWidth, int screenHeight) : screenWidth{screenWidth}, screenHeight{screenHeight} {};

float Graphics::toNDC_x(int x)
{
    return 2.0f * x / screenWidth - 1.0f;
}

float Graphics::toNDC_y(int y)
{
    return 2.0f * y / screenHeight - 1.0f;
}

// coords: координаты прямоугольника (смотрите WindowCoordsRectangle в game_structs.hpp)
// color (необязательный): цвет прямоугольника
void Graphics::drawRectangle(WindowCoordsRectangle coords, Color color, int angle)
{
    if (coords.top_right.x < coords.bottom_left.x)
    {
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался правее правого верхнего (ознакомтесь с WindowCoordsRectangle (game_structs.hpp))");
    }
    if (coords.top_right.y < coords.bottom_left.y)
    {
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался выше правого верхнего (ознакомтесь с WindowCoordsRectangle(game_structs.hpp)");
    }
    glPushMatrix();
    if (angle)
    {
        glTranslatef((coords.top_right.x - coords.bottom_left.x) / 2, (coords.top_right.y - coords.bottom_left.y) / 2, 0.0f);
        glTranslatef(coords.bottom_left.x, coords.bottom_left.y, 0.0f);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(-coords.bottom_left.x, -coords.bottom_left.y, 0.0f);
        glTranslatef(-(coords.top_right.x - coords.bottom_left.x) / 2, -(coords.top_right.y - coords.bottom_left.y) / 2, 0.0f);
    }

    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLES);
    glVertex2f(coords.bottom_left.x, coords.bottom_left.y);
    glVertex2f(coords.top_right.x, coords.bottom_left.y);
    glVertex2f(coords.top_right.x, coords.top_right.y);
    glVertex2f(coords.top_right.x, coords.top_right.y);
    glVertex2f(coords.bottom_left.x, coords.bottom_left.y);
    glVertex2f(coords.bottom_left.x, coords.top_right.y);
    glEnd();
    glPopMatrix();
}

// coord: координата левого нижнего угла прямоугольника
// w: ширина прямоугольника
// h: высота прямоугольника
// color (необязательный): цвет прямоугольника
void Graphics::drawRectangle(WindowCoords coord, int w, int h, Color color, int angle)
{
    drawRectangle({coord.x, coord.y, coord.x + w, coord.y + h}, color, angle);
}

void Graphics::drawRectangleWithTexture(WindowCoordsRectangle coords, unsigned int textureID, bool flipTexture, Color color, int angle)
{
    if (coords.top_right.x < coords.bottom_left.x)
    {
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался правее правого верхнего (ознакомтесь с WindowCoordsRectangle (game_structs.hpp))");
    }
    if (coords.top_right.y < coords.bottom_left.y)
    {
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался выше правого верхнего (ознакомтесь с WindowCoordsRectangle(game_structs.hpp)");
    }
    glPushMatrix();
    if (angle)
    {
        glTranslatef((coords.top_right.x - coords.bottom_left.x) / 2, (coords.top_right.y - coords.bottom_left.y) / 2, 0.0f);
        glTranslatef(coords.bottom_left.x, coords.bottom_left.y, 0.0f);
        glRotatef(angle, 0, 0, 1);
        glTranslatef(-coords.bottom_left.x, -coords.bottom_left.y, 0.0f);
        glTranslatef(-(coords.top_right.x - coords.bottom_left.x) / 2, -(coords.top_right.y - coords.bottom_left.y) / 2, 0.0f);
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(coords.bottom_left.x, coords.bottom_left.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(coords.top_right.x, coords.bottom_left.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(coords.top_right.x, coords.top_right.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(coords.top_right.x, coords.top_right.y);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(coords.bottom_left.x, coords.bottom_left.y);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(coords.bottom_left.x, coords.top_right.y);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::drawRectangleWithTexture(WindowCoords coord, int w, int h, unsigned int textureID, bool flipTexture, Color color, int angle)
{
    drawRectangleWithTexture({coord.x, coord.y, coord.x + w, coord.y + h}, textureID, flipTexture, color, angle);
}

void Graphics::drawCircle(WindowCoords coord, const Color color, unsigned int radius, unsigned int vert_count)
{
    glColor4f(color.r, color.g, color.b, color.a);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    float cx = coord.x, cy = coord.y;
    for (int v = 0; v < vert_count; v++)
    {
        glVertex2f(cx, cy);
        glVertex2f(cx + radius * std::cos(2 * PI / vert_count * v), cy + radius * std::sin(2 * PI / vert_count * v));
        glVertex2f(cx + radius * std::cos(2 * PI / vert_count * (v + 1)), cy + radius * std::sin(2 * PI / vert_count * (v + 1)));
    }
    glEnd();
    glPopMatrix();
}

void Graphics::setClearColor(Color color){
    glClearColor(color.r, color.g, color.b, color.a);
    clearColor = color;
}

Color Graphics::getClearColor(Color color){
    return clearColor;
}