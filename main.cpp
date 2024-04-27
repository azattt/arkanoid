#include <ctime>

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "stb_image.h"



float platform_x = 0.0f;
float platform_y = 10.0f;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Color{
    GLclampf r;
    GLclampf g;
    GLclampf b;
};
int StongRectangle()
{
    int Strong=1+rand()%3;
    return Strong;
}
void drawRectangle(int x, int y, int w, int h, Color color={1.0f, 1.0f, 1.0f}){
    glLoadIdentity();
    glColor3f(color.r, color.g, color.b);
    glTranslatef(2.0f * x / SCREEN_WIDTH - 1.0f, -(2.0f * y / SCREEN_HEIGHT - 1.0f), 0.0f);
    glScalef(2.0f * static_cast<float>(w)/SCREEN_WIDTH, 2.0f * static_cast<float>(h)/SCREEN_HEIGHT, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, -1.0f);    
    glEnd();
}

void myDisp()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // platform
    drawRectangle(0, 500, 80, 20, {1.0f, 0.0f, 0.0f});
    
    for (int x = 0; x < 8; x++){
        for (int y = 0; y < 3; y++){
            int strength = StongRectangle();
            if(strength == 3)
            {
                drawRectangle(40+x*90, 100+y*30, 80, 20, {1.0f, 1.0f, 0.0f});
            }
            else if(strength == 2)
            {
                drawRectangle(40+x*90, 100+y*30, 80, 20, {0.0f, 1.0f, 0.0f});
            }
            else if(strength == 1)
            {
                drawRectangle(40+x*90, 100+y*30, 80, 20, {0.0f, 1.0f, 1.0f});
            }
        }
    }

    glutSwapBuffers();
}

int main(int argc, char **argv)
{   
    glutInit(&argc, argv);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Hello world!");

    srand(time(NULL)); // инициализация генератора случайных чисел

    glutDisplayFunc(myDisp);
    glutMainLoop();

    return 0;
}