/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
//通过命令行工具安装的OpenGL,默认都是GLEW
// GLAD是不会默认安装或集成，需要到官网进行配置
#include FT_FREETYPE_H
#include <map>
#include <string>

class Shader;

class TextRender {
public:
	TextRender();
	~TextRender();
	bool initialize(const std::string& fontPath, unsigned int fontSize);
	float renderText(const std::string& text, float x, float y, float scale,glm::vec3 color,bool render=true);
	float getTextWidth(const std::string& text, float scale);
private:
	FT_Library ft;
	FT_Face face;

	//注意顺序问题
	struct Character {
		GLuint textureID;	//typedef unsigned int GLuint
		//跨平台要求
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};


	std::map<unsigned char, struct Character> characters;

	Shader* textShader;
	GLuint vao, vbo;
	//vao顶点数组对象 vertex array object
	//vbo顶点缓冲对象 vertex buffer object
};