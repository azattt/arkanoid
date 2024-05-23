#include "ball.hpp"

#include <iostream>
#include <cmath>

#include <GL/glut.h>
#include <stb_image.h>

#include "vars.hpp"

Ball::Ball(int x, int y, int r): x(x), y(y), r(r), dx(3), dy(4), active(false){}
void Ball::initializeTexture(){
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load("./resources/svaston2.png", &width, &height, &channels, 0);
    if (image == nullptr){
        std::cout << "Не удалось загрузить текстуру с диска" << std::endl;
    }

    glGenTextures(1, &textureID);
    if (textureID == 0) {
        std::cout << "Не удалось создать текстуру" << std::endl;
    }

    std::cout << width << " " << height << " " << channels << std::endl;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Ball::move(BreakableRectangle* rectangles, int num_rectangles)
{
    x += dx;
    y += dy;
    if (x - r < 0)
    {
        x = r;
        dx *= -1;
        // std::cout << "5" << std::endl;
    }
    else if (y + r >= 600)
    {
        y = 600-r;
        dy *= -1;
        // std::cout << "6" << std::endl;
    }
    else if (x + r >= 800)
    {
        x = 800-r;
        dx *= -1;
        // std::cout << 7 << std::endl;
    }
    
    else if (y + r >= r_y && y + r <= r_y + r_h && x > r_x && x < r_x + r_w)
    {
        dy *= -1;
        y = r_y + r_h + r;
        // std::cout << 8 << std::endl;
    }
    else
    {
    // проверка столкновения с прямоугольниками
    
    for (int i = 0; i < num_rectangles; i++)
    {
        if (rectangles[i].durability == 0) continue;
        if (x - r <= rectangles[i].rect.top_right.x &&
            x + r >= rectangles[i].rect.bottom_left.x &&
            y - r <= rectangles[i].rect.top_right.y &&
            y + r >= rectangles[i].rect.bottom_left.y)
        {
            // std::cout << "внутри\n";
            // лево
            if (x + r - dx < rectangles[i].rect.bottom_left.x){
                x = rectangles[i].rect.bottom_left.x - r;
                dx *= -1;
                // std::cout << x << " 1\n";
            }
            // верх
            else if (y - r - dy > rectangles[i].rect.top_right.y){
                y = rectangles[i].rect.top_right.y + r;
                dy *= -1;
                // std::cout << y << " 2\n";
            }
            // право
            else if (x - r - dx > rectangles[i].rect.top_right.x){
                x = rectangles[i].rect.top_right.x + r;
                dx *= -1;
                // std::cout << y << " 3\n";
            }
            // низ
            else if (y + r - dy > rectangles[i].rect.bottom_left.y){
                dy *= -1;
                y = rectangles[i].rect.bottom_left.y - r;
                // std::cout << y << " 4\n";
            }else{
                continue;
            }
            rectangles[i].durability -= 1;
            break;
        }
        
    }
    }
}
void Ball::draw(Graphics graphics){
    graphics.drawCircle({x, y}, {1.0f, 0.0f, 0.0f, 1.0f}, r, 10);
}