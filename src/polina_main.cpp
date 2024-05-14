#include <GL\glut.h>
#include <math.h>
#include "ball.hpp"
#include "vars.hpp"
#include <iostream>

float r_x=70.0, r_y=280.0, r_w=80.0, r_h=8.0; //координата x, координата y, ширина, высота 
const float DELTA_X = 5.0f;

Ball ball(r_y, 8);

void Draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.2, 0.8, 0.1);
    glRectf(r_x, r_y, r_x+r_w, r_y+r_h);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for(float i=0; i<2*3.14; i+=3.14/4)
    {
        glVertex2f(ball.x+ball.r*sin(i), ball.y+ball.r*cos(i));
    }

    glEnd();
    glutSwapBuffers();
}

void Timer(int)
{
    Draw();
    if(ball.active) ball.move();
    glutTimerFunc(33, Timer, 0);
}


void myKey(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'd':
        r_x+=DELTA_X;
        break;
    case 'a':
        r_x-=DELTA_X;
        break;
        default:
        break;
    }
    if(!ball.active)
    {
        ball.x=r_x+r_w/2;
        ball.y=r_y-ball.r;        
        }
    if(key==32 && !ball.active)
    {
        ball.active=true;
        ball.dx=4;
        ball.dy=-4;
    }
    Draw();
}

void Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 300, 300, 0, 1, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 200);
    glutCreateWindow("Arcanoid");
    glutDisplayFunc(Draw);
    glutTimerFunc(33, Timer, 0);
//    glutSpecialFunc(specialKey);
    glutKeyboardFunc(myKey);
    // glutKeyboardFunc(myKeyPress);
    Init();
    glutMainLoop();
    return 0;
}