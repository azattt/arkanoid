#include "ball.hpp"
#include "vars.hpp"

#include <iostream>

void Ball::move(WindowCoordsRectangle* collision_rectangles, int num_rectangles)
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
        std::cout << 8 << std::endl;
    }
    else
    {
    // проверка столкновения с прямоугольниками
    
    for (int i = 0; i < num_rectangles; i++)
    {
        if (x - r <= collision_rectangles[i].top_right.x &&
            x + r >= collision_rectangles[i].bottom_left.x &&
            y - r <= collision_rectangles[i].top_right.y &&
            y + r >= collision_rectangles[i].bottom_left.y)
        {
            std::cout << "внутри\n";
            // лево
            if (x - r - dx < collision_rectangles[i].bottom_left.x){
                x = collision_rectangles[i].bottom_left.x - r;
                dx *= -1;
                std::cout << x << " 1\n";
                break;
            }
            // верх
            else if (y - r - dy > collision_rectangles[i].top_right.y){
                y = collision_rectangles[i].top_right.y + r;
                dy *= -1;
                std::cout << y << " 2\n";
                break;
            }
            // право
            else if (x + r - dx > collision_rectangles[i].top_right.x){
                x = collision_rectangles[i].top_right.x + r;
                dx *= -1;
                std::cout << y << " 3\n";
                break;
            }
            // низ
            else if (y + r - dy > collision_rectangles[i].bottom_left.y){
                dy *= -1;
                y = collision_rectangles[i].bottom_left.y - r;
                std::cout << y << " 4\n";
                break;
            }
        }
        
    }
    }
}
void Ball::draw(Graphics graphics){
    graphics.drawCircle({x, y}, {1.0f, 1.0f, 1.0f, 1.0f}, r, 10);
}