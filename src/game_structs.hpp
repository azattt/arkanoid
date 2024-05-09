#pragma once
#include "GL/gl.h"
/*
Координаты внутри окна
     (0, height)       
            ^ y
            |
            |      
            |------> x
     (0, 0)          (width, 0)
*/
struct WindowCoords
{
    unsigned int x, y;
};

/* 
Если мы хотим отрисовать прямоугольник с помощью данных координат
верхнего левого угла и нижнего правого угла
 *------------top_right
 |                    |
 |                    |
 bottom_left---------*
*/
struct WindowCoordsRectangle{
    WindowCoords bottom_left;
    WindowCoords top_right;
};

struct Color
{
    GLclampf r, g, b, a;
};