#include <iostream>
#include <exception>

#include <GL\glut.h>

#include "game_structs.hpp"
#include "draw_utilities.hpp"


float r_x=70.0, r_y=280.0, r_w=80.0, r_h=8.0; //координата x, координата y, ширина, высота 
const float DELTA_X = 1.0f;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arcanoid"; 

void Draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawRectangle({{10, 0}, {20, 10}});
    glutSwapBuffers();
}

// https://stackoverflow.com/questions/31058604/limiting-fps-in-glut
void Timer(int)
{
    glutPostRedisplay();
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
    Draw();
}



int main(int argc, char **argv)
{   
    // Чтобы русские буквы отображались в консоли cmd Windows
    system("chcp 65001");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(windowTitle);
    glEnable(GL_TEXTURE);
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}