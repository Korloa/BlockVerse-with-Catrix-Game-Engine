#include "render/image/ImageRender.h"
#include "render/Shader.h"

//glVertexAttribPointer(
// GLuint index //������������
// GLint size ÿ������ķ�����
// GLenum type ��������
// GLboolean normalized �Ƿ��һ��
// GLsizei stride ����
// const void* pointer ��ʼƫ����
// )
ImageRender::ImageRender():imageShader(nullptr),vao(0),vbo(0){}

ImageRender::~ImageRender() {
	if (imageShader)
		delete imageShader;
	if (vao)
		glDeleteVertexArrays(1, &vao);
	if (vbo)
		glDeleteBuffers(1, &vbo);
}

// Gen -> bind -> attribute -> enable -> unbind
bool ImageRender::initialize() {
	imageShader = new Shader("image_vertex.glsl", "image_fragment.glsl");

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return true;
}

void ImageRender::renderImage(GLuint textureID, glm::vec2 position, glm::vec2 size, float alpha) {

}