#include <algorithm>
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
bool capturing_ball = false;
int captured_ball_index = 0;
bool inverted_controls = false;

unsigned int textureID;

bool keys[256];

const int TIME_DELTA = 10;

float r_x = 350, r_y = 100, r_w = 100, r_h = 20;
const int DELTA_X = 10;

float BONUS_FALL_SPEED = 2.0f;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arkanoid";

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
            int durability = rand() % 3 + 1; // Генерируем случайный индекс для выбора цвета
            durability = 1;
            BonusType bonus_type = BonusType(rand() % 7);
            // std::cout << bonus_type << std::endl;
            rectangles.push_back(BreakableRectangle{
                WindowCoordsRectangle{40 + x * 100.0f, 500 - 30.0f * y, 40 + x * 100.0f + 80, 500 - 30.0f * y + 15}, durability,
                baseColors[rand() % 3],
                bonus_type});
        }
    }
}

void Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (!inverted_controls && keys['D'] || keys['A'] && inverted_controls)
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
    else if (!inverted_controls && keys['A'] || keys['D'] && inverted_controls)
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
    else if (keys[' '])
    {
        if (captured_ball_index != -1)
        {
            float angle = (float)(rand() % 100) / 100 + 0.3f;
            if (rand() % 2)
            {
                angle = 3.14 - angle;
            }
            std::cout << balls[captured_ball_index].dx << std::endl;
            balls[captured_ball_index].dx = 5.0f * std::cos(angle);
            balls[captured_ball_index].dy = 5.0f * std::sin(angle);
            captured_ball_index = -1;
        }
    }
    for (int i = 0; i < rectangles.size(); i++)
    {
        if (rectangles[i].durability)
        {
            graphics.drawRectangle(rectangles[i].rect, rectangles[i].color);
        }
    }
    for (int i = 0; i < balls.size(); i++)
    {
        if (i == captured_ball_index)
        {
            balls[i].x = r_x + r_w / 2;
        }
        else
        {
            balls[i].move(rectangles);
            if (balls[i].x + balls[i].r < 0)
            {
                balls.erase(balls.begin() + i);
                continue;
            }
        }
        balls[i].draw(graphics);
    }

    for (int i = 0; i < bonuses.size(); i++)
    {
        bonuses[i].y -= BONUS_FALL_SPEED;
        Color color = {1.0f, 1.0f, 1.0f, 1.0f};
        if (bonuses[i].bonus_type >= HalfPlatform)
        {
            color = {1.0f, 0.0f, 0.0f, 1.0f};
        }
        graphics.drawRectangle({bonuses[i].x - 10, bonuses[i].y - 10, bonuses[i].x + 10, bonuses[i].y + 10}, color);
        if (bonuses[i].x - 10 <= r_x + r_w && bonuses[i].y - 10 <= r_y + r_h && bonuses[i].x + 10 >= r_x && bonuses[i].y + 10 >= r_y)
        {
            if (bonuses[i].bonus_type == DoubleBalls)
            {
                int balls_count = balls.size();
                for (int j = 0; j < balls_count; j++)
                {
                    float angle = (float)(rand() % 100) / 100 + 0.3f;
                    if (rand() % 2)
                    {
                        angle = 3.14 - angle;
                    }
                    Ball ball(r_x + r_w / 2, r_y + r_h / 2, 10.0, 3.0f * std::cos(angle), 4.0f * std::sin(angle), balls_count + j);
                    balls.push_back(ball);
                }
            }
            else if (bonuses[i].bonus_type == DoublePlatform)
            {
                r_x -= r_w / 2;
                r_w *= 2;
            }
            else if (bonuses[i].bonus_type == BallCapture)
            {
                if (captured_ball_index == -1)
                {
                    capturing_ball = true;
                }
            }
            else if (bonuses[i].bonus_type == ClearPenalties)
            {
                r_w = 100;
                inverted_controls = false;

            }
            else if (bonuses[i].bonus_type == ClearAll)
            {
                capturing_ball = false;
                r_w = 100;
                inverted_controls = false;

            }
            else if (bonuses[i].bonus_type == HalfPlatform)
            {
                r_x += r_w / 4;
                r_w /= 2;
            }
            else if (bonuses[i].bonus_type == InvertedControls)
            {
                inverted_controls = true;
            }
            bonuses.erase(bonuses.begin() + i);
        }
    }

    graphics.drawRectangle({r_x, r_y, r_x + r_w, r_y + r_h}, {0.0f, 1.0f, 1.0f, 1.0f});

    GLenum errors = glGetError();
    if (errors)
        std::cout << "Ошибки OpenGL: " << errors << std::endl;
    glutSwapBuffers();
    frame_counter += 1;
}

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

    generateMap();

    glutTimerFunc(TIME_DELTA, Timer, 0);
    glutDisplayFunc(Draw);
    glutKeyboardUpFunc(KeyboardUp);
    glutKeyboardFunc(KeyboardDown);

    glEnable(GL_TEXTURE_2D);                           // включение текстур
    glEnable(GL_BLEND);                                // прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    srand(time(NULL)); // инициализация генератора случайных чисел

    Ball ball(r_x + r_w / 2, r_y + r_h + 10, 10, 3, 4, 0);
    balls.push_back(ball);
    glutMainLoop();
    return 0;
}