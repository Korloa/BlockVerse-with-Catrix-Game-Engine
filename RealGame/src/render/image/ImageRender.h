/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
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

//可以发现每个渲染器都是具有相同结构，initialize,render,构造与解构

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