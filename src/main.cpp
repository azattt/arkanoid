#include <iostream>
#include <exception>
#include <ctime>

#include <GL/glut.h>

#include "game_structs.hpp"
#include "draw_utilities.hpp"
#include "ball.hpp"


float r_x=70.0, r_y=280.0, r_w=80.0, r_h=8.0; //координата x, координата y, ширина, высота 
const float DELTA_X = 1.0f;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arcanoid"; 

Graphics graphics(screenWidth, screenHeight);

Color colorArray[8][4];
Color baseColors[4]; // Сохраняем базовые цвета

void generateColors() {
    baseColors[0] = {1.0f, 0.0f, 1.0f, 1.0f}; // Фиолетовый
    baseColors[1] = {1.0f, 1.0f, 0.0f, 1.0f}; // Желтый
    baseColors[2] = {1.0f, 0.5f, 0.0f, 1.0f}; // Оранжевый

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            int colorIndex = rand() % 3; // Генерируем случайный индекс для выбора цвета
            colorArray[x][y] = baseColors[colorIndex];
        }
    }
}

void Draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    graphics.drawRectangle({800, 300}, 200, 100, {0.0f, 0.0f, 1.0f, 0.8f});
    graphics.drawRectangle({300, 100, 700, 300}, {0.0f, 1.0f, 0.0f, 0.4f});
    graphics.drawRectangle({800, 300}, 200, 100, {0.0f, 0.0f, 1.0f, 1.0f});
    graphics.drawRectangle({{10, 0}, {20, 10}});
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            graphics.drawRectangle({40 + x * 100, 500 - 30 * y}, 80, 15, colorArray[x][y]);
        }
    }
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

// вызывается каждый раз, когда окно изменяет свой размер
void reshapeCallback(int w, int h){
    glViewport(0, 0, w, h); // сообщаем OpenGL новый размер окна
    graphics.screenWidth = w;
    graphics.screenHeight = h;
}

int main(int argc, char **argv)
{   
    
    srand(time(NULL)); // инициализация генератора случайных чисел
    generateColors();
    system("chcp 65001"); // Чтобы русские буквы отображались в консоли cmd Windows
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(windowTitle);
    // https://stackoverflow.com/questions/28052053/c-glut-opengl-resize-window-event
    glutReshapeFunc(reshapeCallback);

    glEnable(GL_TEXTURE); // включение текстур
    glEnable(GL_BLEND); // прозрачность
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}