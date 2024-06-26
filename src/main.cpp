#include <algorithm>
#include <ctime>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ball.hpp"
#include "draw_utilities.hpp"
#include "game_structs.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "vars.hpp"

FT_Library library;
GUI game_gui;

unsigned int frame_counter = 0;
bool capturing_ball = true;
int captured_ball_index = 0;
bool inverted_controls = false;

bool keys[256];

int TIME_DELTA = 1;

float r_x = 350, r_y = 100, r_w = 100, r_h = 20;
float bonus_width = 40;
float bonus_height = 20;

float ball_radius = 28.0f;
float ball_axis_speed = 0.7f;
float DELTA_X = 2.0f;

float BONUS_FALL_SPEED = 0.2f;

int screenWidth = 800, screenHeight = 600;
const char windowTitle[] = "Arkanoid";

Graphics graphics(screenWidth, screenHeight);

Color baseColors[3];

std::vector<Ball> balls;
std::vector<BreakableRectangle> rectangles;

unsigned int bonusTextures[7];

enum GameStates
{
    Game,
    Menu,
    Lose,
    Win
};

GameStates game_state = Menu;

void generateMap()
{
    baseColors[0] = {1.0f, 0.0f, 1.0f, 1.0f}; // Фиолетовый
    baseColors[1] = {1.0f, 1.0f, 0.0f, 1.0f}; // Желтый
    baseColors[2] = {1.0f, 0.5f, 0.0f, 1.0f}; // Оранжевый
    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 4; y++)
        {
            int durability = rand() % 3 + 1; // Генерируем случайный индекс для выбора цвета
            BonusType bonus_type = NoBonus;
            // вероятность бонуса 50%;
            if (rand() % 2){
                bonus_type = BonusType(rand() % 7);
            }
            rectangles.push_back(BreakableRectangle{
                WindowCoordsRectangle{10 + x * 80.0f, 500 - 30.0f * y, 10 + x * 80.0f + 65, 500 - 30.0f * y + 15},
                durability, baseColors[durability - 1], bonus_type});
        }
    }
    // rectangles.push_back(BreakableRectangle{WindowCoordsRectangle{0, 0, 800, 100}, -1, {0.0f, 1.0f, 0.0f, 1.0f}, NoBonus});
}

void handle_game_keyboard(int dt)
{
    if (!inverted_controls && keys['D'] || keys['A'] && inverted_controls)
    {
        if (r_x + r_w < screenWidth)
        {
            r_x += DELTA_X * dt;
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
            r_x -= DELTA_X * dt;
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
            balls[captured_ball_index].dx = ball_axis_speed * std::cos(angle);
            balls[captured_ball_index].dy = ball_axis_speed * std::sin(angle);
            captured_ball_index = -1;
        }
    }
}
void handle_map()
{
    bool at_least_one_rectangle_alive = false;
    for (int i = 0; i < rectangles.size(); i++)
    {
        if (rectangles[i].durability)
        {
            graphics.drawRectangle(rectangles[i].rect, rectangles[i].color);
            if (rectangles[i].durability > 0) at_least_one_rectangle_alive = true;
        }
    }
    if (!at_least_one_rectangle_alive){
        game_state = Win;
    }
}
void handle_balls(int dt)
{
    bool at_least_one_ball_is_alive = false;
    for (int i = 0; i < balls.size(); i++)
    {
        if (i == captured_ball_index)
        {
            balls[i].x = r_x + r_w / 2;
            balls[i].y = r_y + r_h + balls[i].r;
        }
        else
        {
            balls[i].move(rectangles, dt);
            if (balls[i].y + balls[i].r < 0)
            {
                balls.erase(balls.begin() + i);
                i--;
                continue;
            }
        }
        at_least_one_ball_is_alive = true;
        balls[i].draw(graphics);
    }
    if (!at_least_one_ball_is_alive && game_state != Win)
    {
        game_state = Lose;
    }
}
void handle_bonuses(int dt)
{
    for (int i = 0; i < bonuses.size(); i++)
    {
        bonuses[i].y -= BONUS_FALL_SPEED * dt;
        graphics.drawRectangleWithTexture({bonuses[i].x - bonus_width / 2, bonuses[i].y - bonus_height / 2,
                                           bonuses[i].x + bonus_width / 2, bonuses[i].y + bonus_height / 2},
                                          bonusTextures[bonuses[i].bonus_type]);
        if (bonuses[i].x - bonus_width / 2 <= r_x + r_w && bonuses[i].y - bonus_height / 2 <= r_y + r_h &&
            bonuses[i].x + bonus_width / 2 >= r_x && bonuses[i].y + bonus_height / 2 >= r_y)
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
                    Ball ball(r_x + r_w / 2, r_y + r_h / 2, ball_radius, ball_axis_speed * std::cos(angle),
                              ball_axis_speed * std::sin(angle), balls_count + j);
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
                if (r_w < 100){
                    r_w = 100;
                }
                inverted_controls = false;
            }
            else if (bonuses[i].bonus_type == ClearAll)
            {
                r_w = 100;
                inverted_controls = false;
                if (captured_ball_index != -1)
                {
                    capturing_ball = false;
                    float angle = (float)(rand() % 100) / 100 + 0.3f;
                    if (rand() % 2)
                    {
                        angle = 3.14 - angle;
                    }
                    balls[captured_ball_index].dx = ball_axis_speed * std::cos(angle);
                    balls[captured_ball_index].dy = ball_axis_speed * std::sin(angle);
                    captured_ball_index = -1;
                }
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
}
void handle_platform()
{
    graphics.drawRectangle({r_x, r_y, r_x + r_w, r_y + r_h}, {0.0f, 1.0f, 1.0f, 1.0f});
}

int time_elapsed = 0;
void Draw()
{
    if (time_elapsed == 0){
        time_elapsed = glutGet(GLUT_ELAPSED_TIME);
    }
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    int dt = current_time - time_elapsed;
    if (dt < 0){
        std::cout << "dt < 0" << std::endl;
    }
    time_elapsed = current_time;

    glClear(GL_COLOR_BUFFER_BIT);

    if (game_state == Game)
    {
        handle_game_keyboard(dt);
        handle_map();
        handle_balls(dt);
        handle_bonuses(dt);
        handle_platform();
    }
    else if (game_state == Menu)
    {
        game_gui.drawMenu(graphics);
    }
    else if (game_state == Lose)
    {
        handle_map();
        handle_bonuses(dt);
        handle_platform();
        game_gui.drawLose(graphics);
    }
    else if (game_state == Win)
    {
        handle_game_keyboard(dt);
        handle_map();
        handle_balls(dt);
        handle_bonuses(dt);
        handle_platform();
        game_gui.drawWin(graphics);
    }
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

void GUI::startGameCallback(int button, int state, int x, int y)
{
    if (button == 0 && state == 1)
    {
        if (x >= 250 && x <= 550 && y >= 260 && y <= 340)
        {
            if (game_state == Menu || game_state == Lose || game_state == Win)
            {
                balls.clear();
                Ball ball(r_x + r_w / 2, r_y + r_h + ball_radius, ball_radius, 0, 0, 0);
                float angle = (float)(rand() % 100) / 100 + 0.3f;
                if (rand() % 2)
                {
                    angle = 3.14 - angle;
                }
                balls.push_back(ball);
                rectangles.clear();
                generateMap();
                capturing_ball = false;
                captured_ball_index = 0;
                inverted_controls = false;
                game_state = Game;
                r_w = 100;
                r_x = 350;
                bonuses.clear();
            }
        }
    }
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

    glEnable(GL_TEXTURE_2D);                           // включение текстур
    glEnable(GL_BLEND);                                // прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    srand(time(NULL)); // инициализация генератора случайных чисел

    FT_Error error = FT_Init_FreeType(&library);
    if (error)
    {
        std::cout << "Ошибка загрузки FreeType: " << error << std::endl;
    }

    game_gui.init(library);
    game_gui.setMenuCallbacks();

    stbi_set_flip_vertically_on_load(true);
    glGenTextures(7, bonusTextures);
    std::vector<std::string> bonusTexturesFilenames{"double_balls.bmp",    "double_platform.bmp", "ball_capture.bmp",
                                                    "clear_penalties.bmp", "clear_all.bmp",       "half_platform.bmp",
                                                    "invert_controls.bmp"};
    int i = 0;
    for (std::string filename : bonusTexturesFilenames)
    {
        std::string path = std::string("resources/") + filename;
        int width, height, channels;
        ;
        unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (image == nullptr)
        {
            std::cout << "Не удалось загрузить текстуру \"" << path << "\" с диска" << std::endl;
            i++;
            continue;
        }
        if (bonusTextures[i] == 0)
        {
            std::cout << "Не удалось создать текстуру" << std::endl;
            i++;
            continue;
        }
        glBindTexture(GL_TEXTURE_2D, bonusTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glBindTexture(GL_TEXTURE_2D, 0);
        i++;
    }

    glutMainLoop();
    return 0;
}