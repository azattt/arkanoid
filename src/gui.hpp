#pragma once

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <string>
#include <map>

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
    FT_Face face;
    std::map<int, std::map<char32_t, Character>> utf8_characters;

    // [start; end]
    void generateAlphabet(char32_t start, char32_t end, int pixelSize){
        assert(start <= end);
        FT_Set_Pixel_Sizes(face, 0, pixelSize);
        utf8_characters[pixelSize] = std::map<char32_t, Character>();
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
        font_path = "C:/Windows/Fonts/arial.ttf";
#endif
        
        FT_Error error = FT_New_Face(library, font_path, 0, &face);
        if ( error == FT_Err_Unknown_File_Format )
        {
            std::cout << "Не удалось загрузить шрифт: \"" << font_path << "\". Ошибка: FT_Err_Unknown_File_Format (формат не распознан)" << std::endl;
        }
        else if ( error )
        {
            std::cout << "Не удалось загрузить шрифт: \"" << font_path << "\". Ошибка: " << error << std::endl;
        }
        std::cout << "Шрифт \"" << font_path << "\" загружен. Количество глифов: " << face->num_glyphs << std::endl;
        error = FT_Set_Pixel_Sizes(face, 0, 16);
        if (error){
            std::cout << "Ошибка freetype: " << error << std::endl;
        }
        generateAlphabet(U'А', u'я', 50);
        generateAlphabet(U' ', U' ', 50);
        generateAlphabet(U'A', U'z', 50);
    }
    void drawMenu(Graphics graphics){
        std::u32string test_str(U"Arkanoid");
        float left = 300.0f, top = 500.0f;
        for (char32_t s: test_str){
            Character character = utf8_characters[50][s];
            graphics.drawRectangleWithTexture({left+character.x_offset, top-(character.height-character.y_offset)}, character.width, character.height, character.textureID, true);
            left += character.advance / 64;
            // std::cout << left << std::endl;
            
        }

    }
};