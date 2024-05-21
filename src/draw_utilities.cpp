#include "draw_utilities.hpp"

#include <stdexcept>
#include <cmath>

#include "game_structs.hpp"

constexpr float PI = 3.14159265359;

Graphics::Graphics(int screenWidth, int screenHeight): screenWidth{screenWidth}, screenHeight{screenHeight}{};

float Graphics::toNDC_x(int x){
    return 2.0f * x / screenWidth - 1.0f;
}

float Graphics::toNDC_y(int y){
    return 2.0f * y / screenHeight - 1.0f;
}

// coords: координаты прямоугольника (смотрите WindowCoordsRectangle в game_structs.hpp)
// color (необязательный): цвет прямоугольника
void Graphics::drawRectangle(WindowCoordsRectangle coords, Color color, int angle)
{
    if (coords.top_right.x < coords.bottom_left.x){
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался правее правого верхнего (ознакомтесь с WindowCoordsRectangle (game_structs.hpp))");
    }
    if (coords.top_right.y < coords.bottom_left.y){
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался выше правого верхнего (ознакомтесь с WindowCoordsRectangle(game_structs.hpp)");
    }
    glLoadIdentity();
    glTranslatef(2.0f * coords.bottom_left.x / screenWidth - 1.0f, (2.0f * coords.bottom_left.y / screenHeight - 1.0f), 0.0f);
    glScalef(2.0f * static_cast<float>(coords.top_right.x-coords.bottom_left.x) / screenWidth, 2.0f * static_cast<float>(coords.top_right.y-coords.bottom_left.y) / screenHeight, 1.0f);
    glRotatef(angle, 0, 0, 1);
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();    
}

// coord: координата левого нижнего угла прямоугольника
// w: ширина прямоугольника
// h: высота прямоугольника
// color (необязательный): цвет прямоугольника
void Graphics::drawRectangle(WindowCoords coord, int w, int h, Color color)
{
    drawRectangle({coord.x, coord.y, coord.x+w, coord.y+h}, color);
}

void Graphics::drawRectangleWithTexture(WindowCoordsRectangle coords, unsigned int textureID)
{
    if (coords.top_right.x < coords.bottom_left.x){
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался правее правого верхнего (ознакомтесь с WindowCoordsRectangle (game_structs.hpp))");
    }
    if (coords.top_right.y < coords.bottom_left.y){
        throw std::invalid_argument("Недопустимые координаты прямоугольника: левый нижний угол оказался выше правого верхнего (ознакомтесь с WindowCoordsRectangle(game_structs.hpp)");
    }
    glLoadIdentity();
    glTranslatef(2.0f * coords.bottom_left.x / screenWidth - 1.0f, (2.0f * coords.bottom_left.y / screenHeight - 1.0f), 0.0f);
    glScalef(2.0f * static_cast<float>(coords.top_right.x-coords.bottom_left.x) / screenWidth, 2.0f * static_cast<float>(coords.top_right.y-coords.bottom_left.y) / screenHeight, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glVertex2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f);
    glEnd();    
}

void Graphics::drawRectangleWithTexture(WindowCoords coord, int w, int h, unsigned int textureID)
{
    glLoadIdentity();
    glTranslatef(2.0f * coord.x / screenWidth - 1.0f, (2.0f * coord.y / screenHeight - 1.0f), 0.0f);
    glScalef(2.0f * static_cast<float>(w) / screenWidth, 2.0f * static_cast<float>(h) / screenHeight, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f);
    glEnd();
}

void Graphics::drawCircle(WindowCoords coord, const Color color, unsigned int radius, unsigned int vert_count)
{
    glLoadIdentity();
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLES);
    float cx = toNDC_x(coord.x), cy = toNDC_y(coord.y);
    float NDC_radius_x = 2.0f * static_cast<float>(radius)/screenWidth;
    float NDC_radius_y = 2.0f * static_cast<float>(radius)/screenHeight;
    for (int v = 0; v < vert_count; v++)
    {
        glVertex2f(cx, cy);
        glVertex2f(cx + NDC_radius_x * std::cos(2*PI/vert_count * v), cy + NDC_radius_y * std::sin(2*PI/vert_count * v));
        glVertex2f(cx + NDC_radius_x * std::cos(2*PI/vert_count * (v+1)), cy + NDC_radius_y * std::sin(2*PI/vert_count * (v+1)));
    }
    glEnd();
}