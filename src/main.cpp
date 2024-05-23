#include <iostream>
#include <exception>
#include <ctime>

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

void generateMap() {
    baseColors[0] = {1.0f, 0.0f, 1.0f, 1.0f}; // Фиолетовый
    baseColors[1] = {1.0f, 1.0f, 0.0f, 1.0f}; // Желтый
    baseColors[2] = {1.0f, 0.5f, 0.0f, 1.0f}; // Оранжевый
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 3; y++) {
            int durability = rand() % 3 + 1; // Генерируем случайный индекс для выбора цвета
            rectangles[y*8+x] = {
                WindowCoordsRectangle{40 + x * 100, 500 - 30 * y, 40 + x * 100 + 80, 500 - 30 * y + 15},
                durability,
                baseColors[durability-1]
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

    // graphics.drawRectangle({800, 300}, 200, 100, {0.0f, 0.0f, 1.0f, 0.8f});
    // graphics.drawRectangle({300, 100, 700, 300}, {0.0f, 1.0f, 0.0f, 0.4f});
    // graphics.drawRectangle({800, 300}, 200, 100, {0.0f, 0.0f, 1.0f, 1.0f});

    for (int i = 0; i < sizeof(rectangles) / sizeof(BreakableRectangle); i++){
        if (rectangles[i].durability > 0){
            graphics.drawRectangle(rectangles[i].rect, rectangles[i].color);
        }
    }
    // TODO: тут короче сделать чтобы каждый кадр вызывался этот метод ball.move и мячик двигался
    ball.move(rectangles, sizeof(rectangles) / sizeof(BreakableRectangle));
    // TODO: отрисовать этот мячик (применить новую функцию, которую я сейчас напишу)
    ball.draw(graphics);
    int angle = glutGet(GLUT_ELAPSED_TIME);
    graphics.drawRectangle({r_x, r_y, r_x+r_w, r_y+r_h}, {0.0f, 1.0f, 1.0f, 1.0f});
    // graphics.drawRectangleWithTexture({300, 300, 500, 500}, textureID, angle/5);
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
    srand(time(NULL)); // инициализация генератора случайных чисел
    generateMap();
#ifdef _WIN32
    system("chcp 65001"); // Чтобы русские буквы отображались в консоли cmd Windows
#endif
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow(windowTitle);
    // https://stackoverflow.com/questions/28052053/c-glut-opengl-resize-window-event
    glutReshapeFunc(reshapeCallback);

    rectangles[0].rect = {0, 0, 800, 100};

    int x = 200;
    int y = 200;
    int pr_width = 200;
    int pr_height = 30;
    for (int i = 1; i < 7; i++){
        rectangles[i].rect = {x, y+pr_height*(i-1), x+pr_width, y+pr_height*i};
    }
    rectangles[1].color = {228.0f / 255, 3.0f / 255, 3.0f / 255, 1.0f};
    rectangles[2].color = {255.0f / 255, 140.0f / 255, 0.0f / 255, 1.0f};
    rectangles[3].color = {255.0f / 255, 237.0f / 255, 0.0f / 255, 1.0f};
    rectangles[4].color = {0.0f / 255, 128.0f / 255, 38.0f / 255, 1.0f};
    rectangles[5].color = {36.0f / 255, 64.0f / 255, 142.0f / 255, 1.0f};
    rectangles[6].color = {115.0f / 255, 41.0f / 255, 130.0f / 255, 1.0f};

    glutTimerFunc(TIME_DELTA, Timer, 0);
    // glutKeyboardFunc(myKey);
    glutDisplayFunc(Draw);

    // glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutKeyboardUpFunc(KeyboardUp);
    glutKeyboardFunc(KeyboardDown);

    glEnable(GL_TEXTURE_2D); // включение текстур
    glEnable(GL_BLEND); // прозрачность

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // функция прозрачности
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load("./resources/svaston2.png", &width, &height, &channels, 0);
    if (image == nullptr){
        std::cout << "Не удалось загрузить текстуру с диска" << std::endl;
    }

    glGenTextures(1, &textureID);
    if (textureID == 0) {
        std::cout << "Не удалось создать текстуру" << std::endl;
    }

    std::cout << width << " " << height << " " << channels << std::endl;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout << glGetError() << std::endl;
    
    ball.initializeTexture();
    glutMainLoop();
    return 0;
}