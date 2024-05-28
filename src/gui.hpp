#pragma once

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "draw_utilities.hpp"

// FT_Error error = FT_Init_FreeType( &library );

// // if ( error )
// // {
// //   ... an error occurred during library initialization ...
// // }

struct Character{
    int x_offset, y_offset;
    
}

class GUI{
    public:
    FT_Face face;
    std::map<char32_t, unsigned int> utf8_characters;

    // [start; end]
    unsigned int generateAlphabet(char32_t start, char32_t end){
        assert(start < end);
        unsigned int textureIDs[end-start+1];
        glGenTextures(end-start+1, textureIDs);
        for (int i = 0; i < end-start+1; i++){
            glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            
        }
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void init(FT_Library& library){
        const char* font_path;
#ifdef __linux__
        font_path = "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf";
#elif _WIN32
#error "Нужно задать путь до шрифта"; 
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
        std::u32string test_string(U"тест");
        for (char32_t utf8_char: test_string){
            error = FT_Load_Char(face, test_string[0], FT_LOAD_RENDER);
            if (error) {
                std::cout << "Ошибка freetype: " << error << std::endl;
            }
            int width = face->glyph->bitmap.width, height = face->glyph->bitmap.rows;
            glGenTextures(1, &textureID);
            if (textureID == 0)
            {
                std::cout << "Не удалось создать текстуру" << std::endl;
            }

            // std::cout << width << " " << height << " " << channels << std::endl;
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void draw(Graphics graphics){
        graphics.drawRectangleWithTexture({0.0f, 0.0f, 200.0f, 200.0f}, textureID);
    }
};