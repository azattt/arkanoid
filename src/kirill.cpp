#include <GL/glut.h>
#include <ctime>
#include <iostream>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

float platform_x = 0.0f;
float platform_speed = 0.2f;
float platform_width = 0.2f;
float platform_height = 0.05f;
float platform_y = -0.8f; // платформа внизу окна

float ball_radius = 0.05f;
float ball_x = 0.0f;
float ball_y = platform_y+platform_height+ball_radius;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Color {
    GLclampf r;
    GLclampf g;
    GLclampf b;
};

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

void drawRectangle(float x, float y, float w, float h, Color color) {
    glColor3f(color.r, color.g, color.b);
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(w, h, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
    glPopMatrix();
}

float ball_speed_x = 0.00f;
float ball_speed_y = 0.01f;
void myDisp() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // platform
    drawRectangle(platform_x, platform_y, platform_width, platform_height, {1.0f, 0.0f, 0.0f});

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            drawRectangle(-0.9f + x * 0.25f, 0.8f - y * 0.1f, 0.2f, 0.05f, colorArray[x][y]);
        }
    }

    drawRectangle(ball_x-ball_radius, ball_y-ball_radius, ball_radius, ball_radius, {1.0f, 1.0f, 1.0f});

    glutSwapBuffers();
}

void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            if (platform_x - platform_speed > -1.0f) { // Проверяем, не выходит ли платформа за левую границу окна
                platform_x -= platform_speed;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (platform_x + platform_speed + platform_width <= 1.0f) { // Проверяем, не выходит ли платформа за правую границу окна
                platform_x += platform_speed;
            }else{
                std::cout << 1.0f - (platform_x + platform_speed + platform_width); 
            }
            break;
    }

    glutPostRedisplay();
}



void Timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(33, Timer, 0);
    ball_y += ball_speed_y;
    ball_x += ball_speed_x;
    if (ball_x >= 1.0f){
        ball_speed_x = -ball_speed_x;
    }
    if (ball_y >= 1.0f){
        ball_speed_y = -ball_speed_y;
    }
    if (ball_y <= platform_y+platform_height+ball_radius){
        ball_speed_y = -ball_speed_y;
    }
}

int main(int argc, char** argv) {
    srand(time(NULL)); // инициализация генератора случайных чисел

    generateColors();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Hello world!");

    glutDisplayFunc(myDisp);
    glutSpecialFunc(keyboard); // Специальная функция для клавиш-стрелок

    glOrtho(-1, 1, -1, 1, -1, 1);

    glutTimerFunc(33, Timer, 0);
    glutMainLoop();

    return 0;
}