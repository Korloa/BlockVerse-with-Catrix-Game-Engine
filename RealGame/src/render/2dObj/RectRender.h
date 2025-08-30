/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
//相较于头文件守卫，更加的简洁
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Shader;

class RectRender {
public:
	RectRender();
	~RectRender();
	bool initialize();
	void renderRect(glm::vec2 position, glm::vec2 size, glm::vec3 color);
private:
	Shader* rectShader;
	GLuint vao, vbo, ebo;
};