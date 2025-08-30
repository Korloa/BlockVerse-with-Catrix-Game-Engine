/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
//�����ͷ�ļ����������ӵļ��
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