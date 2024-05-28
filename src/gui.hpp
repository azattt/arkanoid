#pragma once

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <string>
#include <map>

#include <GL/glut.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "draw_utilities.hpp"

struct Character{
    int x_offset, y_offset;
    unsigned int width, height;
    long int advance;
    unsigned int textureID;  
};

class GUI{
    public:
    bool leftMousePressed = false;

    FT_Face face;
    std::map<int, std::map<char32_t, Character>> utf8_characters;

    // [start; end]
    void generateAlphabet(char32_t start, char32_t end, int pixelSize){
        assert(start <= end);
        FT_Set_Pixel_Sizes(face, 0, pixelSize);
        if (utf8_characters.count(pixelSize) == 0){
            utf8_characters[pixelSize] = std::map<char32_t, Character>();
        }
        unsigned int textureIDs[end-start+1];
        glGenTextures(end-start+1, textureIDs);
        for (int i = 0; i < end-start+1; i++){
            glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            FT_Error error = FT_Load_Char(face, start+i, FT_LOAD_RENDER);
            if (error) {
                std::cout << "Ошибка freetype: " << error << std::endl;
            }
            int width = face->glyph->bitmap.width, height = face->glyph->bitmap.rows;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            utf8_characters[pixelSize][start+i] = Character{face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->advance.x, textureIDs[i]};
            // std::cout << face->glyph->advance.x << std::endl;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void init(FT_Library& library){
        const char* font_path;
#ifdef __linux__
        font_path = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf";
#elif _WIN32
        font_path = "C:/Windows/Fonts/Verdana.ttf";
#endif
        
        FT_Error error = FT_New_Face(library, font_path, 0, &face);
        if ( error == FT_Err_Unknown_File_Format )
        {
            std::cout << "Не удалось загрузить шрифт: \"" << font_path << "\". Ошибка: FT_Err_Unknown_File_Format (формат не распознан)" << std::endl;
            return;
        }
        else if ( error )
        {
            std::cout << "Не удалось загрузить шрифт: \"" << font_path << "\". Ошибка: " << error << std::endl;
            return;
        }
        std::cout << "Шрифт \"" << font_path << "\" загружен. Количество глифов: " << face->num_glyphs << std::endl;
        error = FT_Set_Pixel_Sizes(face, 0, 16);
        if (error){
            std::cout << "Ошибка freetype: " << error << std::endl;
            return;
        }
        generateAlphabet(U'А', u'я', 50);
        generateAlphabet(U' ', U' ', 50);
        generateAlphabet(U'A', U'z', 50);
        generateAlphabet(U'А', u'я', 20);
        generateAlphabet(U' ', U' ', 20);
        generateAlphabet(U'A', U'z', 20);
    }
    float getStringSize(std::u32string str, int pixelSize){
        float left = 0.0f;
        for (char32_t s: str){
            Character character = utf8_characters[pixelSize][s];
            left += character.advance / 64;
        }        
        return left;
    }
    float drawString(const Graphics& graphics, WindowCoords coords, std::u32string str, int pixelSize, Color color={1.0f, 1.0f, 1.0f, 1.0f}){
        float left = coords.x, top = coords.y;
        for (char32_t s: str){
            Character character = utf8_characters[pixelSize][s];
            graphics.drawRectangleWithTexture({left+character.x_offset, top-(character.height-character.y_offset)}, character.width, character.height, character.textureID, true, color);
            left += character.advance / 64;
        }
        return left;
    }
    float drawStringCentered(const Graphics& graphics, WindowCoords center, std::u32string str, int pixelSize, Color color={1.0f, 1.0f, 1.0f, 1.0f}){
        float width = getStringSize(str, pixelSize);
        drawString(graphics, {center.x-width/2, center.y}, str, pixelSize, color);
        return width;
    }
    void drawButton(const Graphics& graphics, WindowCoords center, float w, float h, std::u32string str, int pixelSize, Color bgColor, Color fgColor={1.0f, 1.0f, 1.0f, 1.0f}){
        graphics.drawRectangle({center.x-w/2, center.y-h/2, center.x+w/2, center.y+h/2}, bgColor);
        drawStringCentered(graphics, {center.x, center.y - pixelSize/2}, str, 20, fgColor);
    }

    static void startGameCallback(int button, int state, int x, int y);
    void setMenuCallbacks(){
        glutMouseFunc(startGameCallback);
    }
    void drawMenu(Graphics& graphics){
        drawStringCentered(graphics, {400.0f, 500.0f}, U"Arkanoid", 50);
        drawButton(graphics, {400.f, 300.f}, 300.0f, 80.0f, U"Новая игра", 20, {0.2f, 0.5f, 0.0f, 1.0f});
    }
    void drawLose(Graphics& graphics){
        drawStringCentered(graphics, {400.0f, 500.0f}, U"Вы проиграли", 50);
        drawButton(graphics, {400.0f, 300.0f}, 300.0f, 80.0f, U"Начать снова?", 20, {0.2f, 0.5f, 0.0f, 1.0f});
    }
};