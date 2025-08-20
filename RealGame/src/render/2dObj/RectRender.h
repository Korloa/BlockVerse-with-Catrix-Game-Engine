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