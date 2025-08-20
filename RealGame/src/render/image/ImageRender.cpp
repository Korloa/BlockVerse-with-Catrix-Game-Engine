#include "render/image/ImageRender.h"
#include "render/Shader.h"

//glVertexAttribPointer(
// GLuint index //顶点属性索引
// GLint size 每个顶点的分量数
// GLenum type 数据类型
// GLboolean normalized 是否归一化
// GLsizei stride 步长
// const void* pointer 起始偏移量
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