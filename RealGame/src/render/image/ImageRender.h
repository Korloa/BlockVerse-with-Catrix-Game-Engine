#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Shader;

//可以发现每个渲染器都是具有相同结构，initialize,render,构造与解构

class ImageRender {
public:
	ImageRender();
	~ImageRender();

	bool initialize();

	void renderImage(GLuint textureID, glm::vec2 position, glm::vec2 size, float alpha);
private:
	Shader* imageShader;
	GLuint vao, vbo;
};