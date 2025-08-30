/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<string>
#include<unordered_map>
#include <GLFW/glfw3.h>

class Shader;

//���Է���ÿ����Ⱦ�����Ǿ�����ͬ�ṹ��initialize,render,������⹹

struct imageInfo {
	int width;
	int height;
	int ch;
};

class ImageRender {
public:
	ImageRender();
	~ImageRender();

	bool initialize();

	void renderImage(const std::string& imagePath, float x, float y, float width, float height, float alpha);

	void renderTexture(GLuint texturID,float x,float y,float width,float height,float alpha);

	GLuint loadTexture(const std::string& path);

	void getImageAttrib(const std::string& path,imageInfo* img);

	void clearCache();
	void clearCache(const std::string& path);
private:

	Shader* imageShader;
	GLuint vao, vbo,ebo;

	std::unordered_map<std::string, GLuint>textureCache;
};