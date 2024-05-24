#include <ctime>
#include <exception>
#include <iostream>
#include <vector>

#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ball.hpp"
#include "draw_utilities.hpp"
#include "game_structs.hpp"
#include "map.hpp"
#include "vars.hpp"

unsigned int frame_counter = 0;

unsigned int textureID;

bool keys[256];

const int TIME_DELTA = 10;

int r_x = 350, r_y = 100, r_w = 100, r_h = 20;
const int DELTA_X = 10;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arcanoid";

Graphics graphics(screenWidth, screenHeight);

Color baseColors[3];

std::vector<Ball> balls;
std::vector<BreakableRectangle> rectangles;

void generateMap()
{
    baseColors[0] = {1.0f, 0.0f, 1.0f, 1.0f}; // Фиолетовый
    baseColors[1] = {1.0f, 1.0f, 0.0f, 1.0f}; // Желтый
    baseColors[2] = {1.0f, 0.5f, 0.0f, 1.0f}; // Оранжевый
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            int durability = rand() % 3 + 6; // Генерируем случайный индекс для выбора цвета
            rectangles.push_back(BreakableRectangle{
                WindowCoordsRectangle{40 + x * 100, 500 - 30 * y, 40 + x * 100 + 80, 500 - 30 * y + 15}, durability,
                baseColors[rand() % 3]});
        }
    }
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (keys['D'])
    {
        if (r_x + r_w < screenWidth)
        {
            r_x += DELTA_X;
        }
        else
        {
            r_x = 800 - r_w;
        }
    }
    else if (keys['A'])
    {
        if (r_x - DELTA_X > 0)
        {
            r_x = r_x - DELTA_X;
        }
        else
        {
            r_x = 0;
        }
    }
    for (int i = 0; i < rectangles.size(); i++)
    {
        if (rectangles[i].durability)
        {
            graphics.drawRectangle(rectangles[i].rect, rectangles[i].color);
        }
    }
    for (auto &i : balls)
    {
        i.move(rectangles);
        i.draw(graphics);
    }

    graphics.drawRectangle({r_x, r_y, r_x + r_w, r_y + r_h}, {0.0f, 1.0f, 1.0f, 1.0f});
    GLenum errors = glGetError();
    if (errors)
        std::cout << "Ошибки OpenGL: " << errors << std::endl;
    glutSwapBuffers();
    frame_counter += 1;
}

// https://stackoverflow.com/questions/31058604/limiting-fps-in-glut
void Timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(TIME_DELTA, Timer, 0);
}

void KeyboardDown(unsigned char key, int x, int y)
{
    if (isalpha(key))
    {
        key = toupper(key);
    }
    keys[key] = true;
}
void KeyboardUp(unsigned char key, int x, int y)
{
    if (isalpha(key))
    {
        key = toupper(key);
    }
    keys[key] = false;
}

// вызывается каждый раз, когда окно изменяет свой размер
void reshapeCallback(int w, int h)
{
    std::cout << "reshape " << w << " " << h << std::endl;
    glViewport(0, 0, w, h); // сообщаем OpenGL новый размер окна
    // перезагружаем матрицу проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
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

    int x = 200;
    int y = 200;
    int pr_width = 200;
    int pr_height = 30;
    generateMap();
    // for (int i = 1; i < 7; i++){
    //     rectangles[i].rect = {x, y+pr_height*(i-1), x+pr_width, y+pr_height*i};
    //     rectangles[i].durability = -1;
    // }
    // rectangles[1].color = {228.0f / 255, 3.0f / 255, 3.0f / 255, 1.0f};
    // rectangles[2].color = {255.0f / 255, 140.0f / 255, 0.0f / 255, 1.0f};
    // rectangles[3].color = {255.0f / 255, 237.0f / 255, 0.0f / 255, 1.0f};
    // rectangles[4].color = {0.0f / 255, 128.0f / 255, 38.0f / 255, 1.0f};
    // rectangles[5].color = {36.0f / 255, 64.0f / 255, 142.0f / 255, 1.0f};
    // rectangles[6].color = {115.0f / 255, 41.0f / 255, 130.0f / 255, 1.0f};

    glutTimerFunc(TIME_DELTA, Timer, 0);
    glutDisplayFunc(Draw);
    glutKeyboardUpFunc(KeyboardUp);
    glutKeyboardFunc(KeyboardDown);

    glEnable(GL_TEXTURE_2D);                           // включение текстур
    glEnable(GL_BLEND);                                // прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    srand(time(NULL)); // инициализация генератора случайных чисел

    float angle = -30 / 180.0f * 3.1415926f;
    // for (int i = 0; i < 1000; i++)
    // {
    //     Ball new_ball(400, 300, 2);
    //     float speed = 20.0f;
    //     float angle = rand();
    //     new_ball.dx = speed * std::cos(angle);
    //     new_ball.dy = speed * std::sin(angle);
    //     // new_ball.dx = new_ball.dy = 0;
    //     // new_ball.dx = -1;
    //     // std::cout << new_ball.dx << " " << new_ball.dy << " " << speed << std::endl;
    //     balls.push_back(new_ball);
    // }

    rectangles[0].rect = {0, 0, 800, 100};
    rectangles[0].durability = -1;

    balls.push_back(Ball(500, 110, 10));
    balls[balls.size()-1].dx = -1;
    balls[balls.size()-1].dy = 0;
    glutMainLoop();
    return 0;
}