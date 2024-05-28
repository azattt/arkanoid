#include "ball.hpp"

#include <algorithm> // std::min/max
#include <cmath>
#include <iostream>

#include <GL/glut.h>
#include <stb_image.h>

#include "vars.hpp"
#include "gui.hpp"

std::vector<Bonus> bonuses;

Ball::Ball(float x, float y, float r, float dx, float dy, int index) : x(x), y(y), r(r), dx(dx), dy(dy), index(index)
{
}
void Ball::initializeTexture()
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load("./resources/svaston2.png", &width, &height, &channels, 0);
    if (image == nullptr)
    {
        std::cout << "Не удалось загрузить текстуру с диска" << std::endl;
    }

    glGenTextures(1, &textureID);
    if (textureID == 0)
    {
        std::cout << "Не удалось создать текстуру" << std::endl;
    }

    // std::cout << width << " " << height << " " << channels << std::endl;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Ball::move(std::vector<BreakableRectangle> &rectangles)
{
    x += dx;
    y += dy;
    if (x - r < 0)
    {
        x = r;
        dx *= -1.0;
        // std::cout << "5" << std::endl;
    }
    else if (y + r > 600)
    {
        y = 600 - r;
        dy *= -1.0;
        // std::cout << "6" << std::endl;
    }
    else if (x + r > 800)
    {
        x = 800 - r;
        dx *= -1.0;
        // std::cout << 7 << std::endl;
    }

    else if (x - r <= r_x + r_w && y - r <= r_y + r_h && x + r >= r_x && y + r >= r_y)
    {
        dy *= -1.0;
        y = r_y + r_h + r;
        if (capturing_ball)
        {
            dy = 0;
            dx = 0;
            x = r_x + r_w / 2;
            captured_ball_index = index;
            capturing_ball = false;
        }
        // std::cout << 8 << std::endl;
    }

    // проверка столкновения с прямоугольниками

    for (int i = 0; i < rectangles.size(); i++)
    {
        if (rectangles[i].durability == 0)
            continue;
        if (x - r <= rectangles[i].rect.top_right.x && x + r >= rectangles[i].rect.bottom_left.x &&
            y - r <= rectangles[i].rect.top_right.y && y + r >= rectangles[i].rect.bottom_left.y)
        {
            // std::cout << "внутри\n";
            // лево
            // https://stackoverflow.com/questions/69997547/intersections-of-intervals
            // низ
            float future_x_left = (x - r) + (dx / dy) * (rectangles[i].rect.bottom_left.y - y - r);
            if (y + r - dy <= rectangles[i].rect.bottom_left.y && std::max(future_x_left, (float)rectangles[i].rect.bottom_left.x) <= std::min(future_x_left + 2 * r, (float)rectangles[i].rect.top_right.x))
            {
                dy *= -1.0;
                y = rectangles[i].rect.bottom_left.y - r;
                // std::cout << y << " низ\n";
            }
            else if (x - r - dx <= rectangles[i].rect.bottom_left.x)
            {
                x = rectangles[i].rect.bottom_left.x - r;
                dx *= -1.0;
                // std::cout << x << " лево\n";
            }
            // верх
            else if (x - dx >= rectangles[i].rect.bottom_left.x && x - dx <= rectangles[i].rect.top_right.x && y + r - dy >= rectangles[i].rect.top_right.y)
            {
                y = rectangles[i].rect.top_right.y + r;
                dy *= -1.0;
                // std::cout << i << " верх\n";
            }
            // право
            else if (x + r - dx >= rectangles[i].rect.top_right.x)
            {
                x = rectangles[i].rect.top_right.x + r;
                dx *= -1.0;
                // std::cout << y << " право\n";
            }
            if (rectangles[i].durability > 0)
            {
                rectangles[i].durability -= 1;
                if (rectangles[i].durability == 0)
                {
                    bonuses.push_back(Bonus{(rectangles[i].rect.top_right.x + rectangles[i].rect.bottom_left.x) / 2.0f, (rectangles[i].rect.top_right.y + rectangles[i].rect.bottom_left.y) / 2.0f,
                                            rectangles[i].bonus_type});
                }
            }
        }
    }
}
void Ball::draw(Graphics &graphics)
{
    graphics.drawCircle({x, y}, {1.0f, 0.0f, 0.0f, 1.0f}, r, 10);
}