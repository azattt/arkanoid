#include <iostream>
#include <exception>
#include <ctime>
#include <vector>

#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "game_structs.hpp"
#include "draw_utilities.hpp"
#include "ball.hpp"
#include "vars.hpp"
#include "map.hpp"

unsigned int textureID;

bool keys[256];

const int TIME_DELTA = 10;

int r_x = 350, r_y = 100, r_w = 100, r_h = 20;
const int DELTA_X = 10;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arcanoid"; 

Graphics graphics(screenWidth, screenHeight);

Color baseColors[3];
BreakableRectangle rectangles[24];

Ball ball(400, 200, 10);

std::vector<Ball> balls;

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
    if (keys['D']){
        if (r_x + r_w < screenWidth){
            r_x += DELTA_X;
        }else{
            r_x = 800 - r_w;
        }
    }else if (keys['A']){
        if (r_x - DELTA_X > 0){
            r_x = r_x - DELTA_X;
        }else{
            r_x = 0;
        }
    }
    for (int i = 0; i < sizeof(rectangles) / sizeof(BreakableRectangle); i++){
        if (rectangles[i].durability > 0){
            graphics.drawRectangle(rectangles[i].rect, baseColors[rectangles[i].durability - 1]);
        }
    }
    for (auto& i: balls)
    {
        i.move(rectangles, sizeof(rectangles) / sizeof(BreakableRectangle));
        i.draw(graphics);
    }

    ball.move(rectangles, sizeof(rectangles) / sizeof(BreakableRectangle));
    ball.draw(graphics);
    graphics.drawRectangle({r_x, r_y, r_x+r_w, r_y+r_h}, {0.0f, 1.0f, 1.0f, 1.0f});
    GLenum errors = glGetError();
    if (errors)
        std::cout << "Ошибки OpenGL: " << errors << std::endl;
    glutSwapBuffers();
}

// https://stackoverflow.com/questions/31058604/limiting-fps-in-glut
void Timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(TIME_DELTA, Timer, 0);
}

void KeyboardDown( unsigned char key, int x, int y ) {
    if ( isalpha( key ) ) {
        key = toupper( key );
    }
    std::cout << key << std::endl;
    keys[ key ] = true;
}
void KeyboardUp( unsigned char key, int x, int y ) {
    if ( isalpha( key ) ) {
        key = toupper( key );
    }
    keys[ key ] = false;        
}

// вызывается каждый раз, когда окно изменяет свой размер
void reshapeCallback(int w, int h){
    std::cout << "reshape " << w << " " << h << std::endl;
    glViewport(0, 0, w, h); // сообщаем OpenGL новый размер окна
    // перезагружаем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ();
    // эта операция на самом деле перемножает текущую матрицу с новой, поэтому нужно
    // glLoadIdentity перед этим
    glOrtho(0, w, 0, h, -1.0, 1.0); 

    // сообщаем модулю отрисовки новые ширину и высоту
    graphics.screenWidth = w;
    graphics.screenHeight = h;
}


int main(int argc, char **argv)
{   
#ifdef _WIN32
    system("chcp 65001"); // Чтобы русские буквы отображались в консоли cmd Windows
#endif
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(windowTitle);
    // https://stackoverflow.com/questions/28052053/c-glut-opengl-resize-window-event
    glutReshapeFunc(reshapeCallback);
    glutTimerFunc(TIME_DELTA, Timer, 0);
    glutDisplayFunc(Draw);
    glutKeyboardUpFunc(KeyboardUp);
    glutKeyboardFunc(KeyboardDown);

    glEnable(GL_TEXTURE_2D); // включение текстур
    glEnable(GL_BLEND); // прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    srand(time(NULL)); // инициализация генератора случайных чисел
    generateMap();
    for (int i = 0; i < 5; i++){
        Ball new_ball(400, 200, 10);
        new_ball.dx = 5.0f*std::cos(1.0f + i/5.0f);
        new_ball.dy = 5.0f*std::sin(1.0f + i/5.0f);
        std::cout << new_ball.dx << " " << new_ball.dy << std::endl;
        balls.push_back(new_ball);
    }

    ball.initializeTexture();

    rectangles[0].rect = {0, 0, 800, 200};
    rectangles[0].durability = 3;

    glutMainLoop();
    return 0;
}