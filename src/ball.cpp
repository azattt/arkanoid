#include "ball.hpp"

#include <iostream>
#include <cmath>

#include <GL/glut.h>

#include "vars.hpp"

void Ball::move(BreakableRectangle* rectangles, int num_rectangles)
{
    x += dx;
    y += dy;
    if (x - r < 0)
    {
        x = r;
        dx *= -1;
        std::cout << "5" << std::endl;
    }
    else if (y + r >= 600)
    {
        y = 600-r;
        dy *= -1;
        std::cout << "6" << std::endl;
    }
    else if (x + r >= 800)
    {
        x = 800-r;
        dx *= -1;
        std::cout << 7 << std::endl;
    }
    
    else if (y + r >= r_y && y + r <= r_y + r_h && x > r_x && x < r_x + r_w)
    {
        dy *= -1;
        y = r_y + r_h + r;
        std::cout << 8 << std::endl;
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
            std::cout << "внутри\n";
            // лево
            if (x + r - dx < rectangles[i].rect.bottom_left.x){
                x = rectangles[i].rect.bottom_left.x - r;
                dx *= -1;
                std::cout << x << " 1\n";
            }
            // верх
            else if (y - r - dy > rectangles[i].rect.top_right.y){
                y = rectangles[i].rect.top_right.y + r;
                dy *= -1;
                std::cout << y << " 2\n";
            }
            // право
            else if (x - r - dx > rectangles[i].rect.top_right.x){
                x = rectangles[i].rect.top_right.x + r;
                dx *= -1;
                std::cout << y << " 3\n";
            }
            // низ
            else if (y + r - dy > rectangles[i].rect.bottom_left.y){
                dy *= -1;
                y = rectangles[i].rect.bottom_left.y - r;
                std::cout << y << " 4\n";
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
    int angle = glutGet(GLUT_ELAPSED_TIME);
    graphics.drawRectangle({x-10, y-2, x+10, y+2}, {1.0f, 1.0f, 1.0f, 1.0f}, angle);
    graphics.drawRectangle({x-2, y-10, x+2, y+10}, {1.0f, 1.0f, 1.0f, 1.0f}, angle);
    // graphics.drawRectangle({x-10, y+6, x, y+10}, {1.0f, 1.0f, 1.0f, 1.0f}, angle);
    // graphics.drawRectangle({x, y-10, x+10, y-6}, {1.0f, 1.0f, 1.0f, 1.0f}, angle);
    // graphics.drawRectangle({x+6, y, x+10, y+10}, {1.0f, 1.0f, 1.0f, 1.0f}, angle);
    // graphics.drawRectangle({x-10, y-10, x-6, y}, {1.0f, 1.0f, 1.0f, 1.0f}, angle);

    // graphics.drawCircle({x, y}, {1.0f, 0.0f, 0.0f, 1.0f}, r, 10);
}