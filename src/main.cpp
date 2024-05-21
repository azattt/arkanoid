#include <iostream>
#include <exception>
#include <ctime>

#include <GL/glut.h>

#include "game_structs.hpp"
#include "draw_utilities.hpp"
#include "ball.hpp"
#include "vars.hpp"
#include "map.hpp"


const int TIME_DELTA = 10;

int r_x = 350, r_y = 100, r_w = 100, r_h = 20;
const int DELTA_X = 20;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arcanoid"; 

Graphics graphics(screenWidth, screenHeight);

Color baseColors[3];
BreakableRectangle rectangles[24];

Ball ball(400, 200, 5);

void generateMap() {
    baseColors[0] = {1.0f, 0.0f, 1.0f, 1.0f}; // Фиолетовый
    baseColors[1] = {1.0f, 1.0f, 0.0f, 1.0f}; // Желтый
    baseColors[2] = {1.0f, 0.5f, 0.0f, 1.0f}; // Оранжевый
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            int durability = rand() % 3 + 1; // Генерируем случайный индекс для выбора цвета
            rectangles[y*8+x] = {
                WindowCoordsRectangle{40 + x * 100, 500 - 30 * y, 40 + x * 100 + 80, 500 - 30 * y + 15},
                durability
            };
        }
    }
}

void Draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    // graphics.drawRectangle({800, 300}, 200, 100, {0.0f, 0.0f, 1.0f, 0.8f});
    // graphics.drawRectangle({300, 100, 700, 300}, {0.0f, 1.0f, 0.0f, 0.4f});
    // graphics.drawRectangle({800, 300}, 200, 100, {0.0f, 0.0f, 1.0f, 1.0f});

    for (int i = 0; i < sizeof(rectangles) / sizeof(BreakableRectangle); i++){
        if (rectangles[i].durability > 0){
            graphics.drawRectangle(rectangles[i].rect, baseColors[rectangles[i].durability - 1]);
        }
    }
    // TODO: тут короче сделать чтобы каждый кадр вызывался этот метод ball.move и мячик двигался
    ball.move(rectangles, sizeof(rectangles) / sizeof(BreakableRectangle));
    // TODO: отрисовать этот мячик (применить новую функцию, которую я сейчас напишу)
    ball.draw(graphics);
    graphics.drawRectangle({r_x, r_y}, r_w, r_h);
    glutSwapBuffers();
}

// https://stackoverflow.com/questions/31058604/limiting-fps-in-glut
void Timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(TIME_DELTA, Timer, 0);
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
}

// вызывается каждый раз, когда окно изменяет свой размер
void reshapeCallback(int w, int h){
    glViewport(0, 0, w, h); // сообщаем OpenGL новый размер окна
    // сообщаем модулю отрисовки новые ширину и высоту
    graphics.screenWidth = w;
    graphics.screenHeight = h;
}

int main(int argc, char **argv)
{   
    srand(time(NULL)); // инициализация генератора случайных чисел
    generateMap();
    system("chcp 65001"); // Чтобы русские буквы отображались в консоли cmd Windows
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(windowTitle);
    // https://stackoverflow.com/questions/28052053/c-glut-opengl-resize-window-event
    glutReshapeFunc(reshapeCallback);

    glEnable(GL_TEXTURE); // включение текстур
    glEnable(GL_BLEND); // прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    rectangles[0].rect = {0, 0, 800, 100};
    // rectangles[1] = {500, 0, 800, 600};

    glutTimerFunc(TIME_DELTA, Timer, 0);
    glutKeyboardFunc(myKey);
    glutDisplayFunc(Draw);
    glutMainLoop();
    return 0;
}