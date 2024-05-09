void drawRectangle(WindowCoords coord, int w, int h, Color color = {1.0f, 1.0f, 1.0f, 1.0f})
{
    glLoadIdentity();
    glTranslatef(2.0f * coord.x / screenWidth - 1.0f, (2.0f * coord.y / screenHeight - 1.0f), 0.0f);
    glScalef(2.0f * static_cast<float>(w) / screenWidth, 2.0f * static_cast<float>(h) / screenHeight, 1.0f);
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
void drawRectangle(WindowCoordsRectangle coords, Color color = {1.0f, 1.0f, 1.0f, 1.0f})
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
void drawRectangleWithTexture(WindowCoords coord, int w, int h, unsigned int textureID)
{
    glLoadIdentity();
    glTranslatef(2.0f * coord.x / screenWidth - 1.0f, -(2.0f * coord.y / screenHeight - 1.0f), 0.0f);
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
void drawRectangleWithTexture(WindowCoordsRectangle coords, unsigned int textureID)
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
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f); glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f); glTexCoord2f(1.0f, 1.0f);
    glVertex2f(0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f);
    glEnd();    
}