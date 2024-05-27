#pragma once

#include <stdexcept>
#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "draw_utilities.hpp"

// FT_Error error = FT_Init_FreeType( &library );

// // if ( error )
// // {
// //   ... an error occurred during library initialization ...
// // }

class GUI{
    public:
    FT_Face face;
    unsigned int textureID;

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
        error = FT_Set_Char_Size(face, 0, 32*64, 100, 100);
        if (error){
            std::cout << "Ошибка freetype: " << error << std::endl;
        }
        unsigned int glyph_index = FT_Get_Char_Index(face, char32_t{'x'});
        std::cout << glyph_index <<  char32_t{'x'} << std::endl;
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error) {
            std::cout << "Ошибка freetype: " << error << std::endl;
        }
        if (face->glyph->format != FT_GLYPH_FORMAT_BITMAP){
            // std::cout << "[INFO] Неверный формат глифа" << std::endl;
            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
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

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glBindTexture(GL_TEXTURE_2D, 0);
        }
    void draw(Graphics graphics){

    }
};