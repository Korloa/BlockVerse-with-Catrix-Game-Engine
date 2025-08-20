#pragma once
#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Shader;

//���Է���ÿ����Ⱦ�����Ǿ�����ͬ�ṹ��initialize,render,������⹹

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