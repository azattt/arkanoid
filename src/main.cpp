#include <iostream>
#include <exception>
#include <ctime>

#include <GL/glut.h>

#include "game_structs.hpp"
#include "draw_utilities.hpp"


float r_x=70.0, r_y=280.0, r_w=80.0, r_h=8.0; //координата x, координата y, ширина, высота 
const float DELTA_X = 1.0f;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arcanoid"; 

Graphics graphics(screenWidth, screenHeight);

Color colorArray[8][3];
Color baseColors[3]; // Сохраняем базовые цвета

void generateColors() {
    baseColors[0] = {1.0f, 0.0f, 1.0f}; // Фиолетовый
    baseColors[1] = {1.0f, 1.0f, 0.0f}; // Желтый
    baseColors[2] = {1.0f, 0.5f, 0.0f}; // Оранжевый

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            int colorIndex = rand() % 3; // Генерируем случайный индекс для выбора цвета
            colorArray[x][y] = baseColors[colorIndex];
        }
    }
}

void Draw(){
    std::cout << "Draw()" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    graphics.drawRectangle({0,0}, 400, 300);
    graphics.drawRectangle({{10, 0}, {20, 10}});
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            graphics.drawRectangle({40 + x * 100, 500 - 30 * y}, 80, 15);
            // drawRectangle(-0.9f + x * 0.25f, 0.8f - y * 0.1f, 0.2f, 0.05f, colorArray[x][y]);
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

void reshapeCallback(int w, int h){
    std::cout << w << " " << h;
    graphics.screenWidth = w;
    graphics.screenHeight = h;
}

int main(int argc, char **argv)
{   
    
    srand(time(NULL)); // инициализация генератора случайных чисел
    generateColors();
    system("chcp 65001"); // Чтобы русские буквы отображались в консоли cmd Windows
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(windowTitle);
    // https://stackoverflow.com/questions/28052053/c-glut-opengl-resize-window-event
    glutReshapeFunc(reshapeCallback);

    glEnable(GL_TEXTURE);
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}