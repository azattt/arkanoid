#include"ball.hpp"
#include "vars.hpp"

#include <iostream>
void Ball::move()
{
    std::cout << x << " " << y << std::endl;
    x+=dx;
    y+=dy;
    if(x-r<=0)
    {
        x=0;
        dx*=-1;
    }
    else if (y-r<=0)
    {
        y=0;
        dy*=-1;
    }
    else if(x+r>=300)
    {
        x=300-r;
        dx*=-1;
    }
    else if(y+r>=r_y && y+r<=r_y+r_h && x>r_x && x<r_x+r_w)
    {
        dy*=-1;
    }
    else if(y>300) active=false;
}