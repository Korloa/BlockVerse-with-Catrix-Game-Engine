/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "render/text/TextRender.h"
#include "render/Shader.h"
#include "debug/Console.h"

TextRender::TextRender() :ft(nullptr), face(nullptr), textShader(nullptr), vao(0), vbo(0) {}
TextRender::~TextRender() {
    for (auto& c : characters)
        glDeleteTextures(1, &c.second.textureID);
    characters.clear();
    if (face)
        FT_Done_Face(face);
    if (ft)
        FT_Done_FreeType(ft);
    if (textShader)
        delete textShader;
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
}

bool TextRender::initialize(const std::string& fontPath, unsigned int fontSize) {

    if (FT_Init_FreeType(&ft)) {
        console.error("Failed to initialize FreeType textrender.");
        return false;
    }
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        FT_Done_FreeType(ft);
        console.error("Failed to load the font.");
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        characters[c] = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    textShader = new Shader("render/text_vertex.glsl", "render/text_fragment.glsl");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

float TextRender::renderText(const std::string& text, float x, float y, float scale, glm::vec3 color, bool render) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textShader->use();
    glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f);
    textShader->setMat4("projection", projection);
    textShader->setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    float width = 0.0f;
    for (const auto& c : text) {
        if (characters.find(c) == characters.end()) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                face->glyph->bitmap.width, face->glyph->bitmap.rows,
                0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            characters[c] = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<GLuint>(face->glyph->advance.x)
            };
        }

        Character ch = characters[c];
        width += (ch.advance >> 6) * scale;

        if (render) {
            float xpos = x + ch.bearing.x * scale;
            float ypos = y - ch.bearing.y * scale;

            float w = ch.size.x * scale;
            float h = ch.size.y * scale;

            // 更新VBO内容
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos,     ypos,       0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 0.0f },

                { xpos,     ypos + h,   0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 0.0f },
                { xpos + w, ypos + h,   1.0f, 1.0f }
            };

      /*      float vertices[6][4] = {
         { xpos,     ypos + h,   0.0f, 0.0f },
         { xpos,     ypos,       0.0f, 1.0f },
         { xpos + w, ypos,       1.0f, 1.0f },

         { xpos,     ypos + h,   0.0f, 0.0f },
         { xpos + w, ypos,       1.0f, 1.0f },
         { xpos + w, ypos + h,   1.0f, 0.0f }
            };*/

            // 渲染字符纹理
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // 更新位置到下一个字符
            x += (ch.advance >> 6) * scale;
        }
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return width;
}

// TextRender.cpp
float TextRender::getTextWidth(const std::string& text, float scale) {
    float width = 0.0f;

    for (const auto& c : text) {
        // 如果字符还没加载，尝试加载
        if (characters.find(c) == characters.end()) {
            if (face && !FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                GLuint texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                characters[c] = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<GLuint>(face->glyph->advance.x)
                };
            }
            else {
                // 加载失败，跳过
                continue;
            }
        }

        const Character& ch = characters[c];
        width += (ch.advance >> 6) * scale;  // advance 是 1/64 像素，所以右移 6 位
    }

    return width;
}