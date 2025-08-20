#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
//ͨ�������й��߰�װ��OpenGL,Ĭ�϶���GLEW
// GLAD�ǲ���Ĭ�ϰ�װ�򼯳ɣ���Ҫ��������������
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
private:
	FT_Library ft;
	FT_Face face;

	//ע��˳������
	struct Character {
		GLuint textureID;	//typedef unsigned int GLuint
		//��ƽ̨Ҫ��
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};


	std::map<unsigned char, struct Character> characters;

	Shader* textShader;
	GLuint vao, vbo;
	//vao����������� vertex array object
	//vbo���㻺����� vertex buffer object
};