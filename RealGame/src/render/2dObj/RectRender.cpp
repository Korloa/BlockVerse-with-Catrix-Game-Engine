#include "render/2dObj/RectRender.h"
#include "render/Shader.h"

RectRender::RectRender():rectShader(nullptr),vao(0),vbo(0),ebo(0){}

//?? textRender��Ҫvao,vbo rectrender��Ҫvao,vbo,veo

RectRender::~RectRender() {
	if (rectShader)
		delete rectShader;
	if (vao)
		glDeleteVertexArrays(1, &vao);
	if (vbo)
		glDeleteBuffers(1, &vbo);
	if (ebo)
		glDeleteBuffers(1, &ebo);
}

bool RectRender::initialize() {
	rectShader = new Shader("render/rect_vertex.glsl", "render/rect_fragment.glsl");

	//����һ��Vertex Array Object
	//vaoֻ��¼vbo�����������Ϣ����ʵ���ݱ�����vbo
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	//glGenxxx(GLsizei N,GLuint* names)������ʽ�ȽϹ̶�������Ԥ�Ȼ�ȡGPU��Դ��ID��NΪ��������������ֵ��names������
	//ֵ��ע��һ���ṹ
	//glGenxxx() --->  glBindxxx() ---->  glBufferData()
	//������ԴID        �������ݽṹ			�ϴ�����
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//	�̶�

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(0);
	return true;
}

void RectRender::renderRect(glm::vec2 position, glm::vec2 size, glm::vec3 color){
	rectShader->use();
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 0.0f, 768.0f);
	rectShader->setMat4("projection", projection);
	rectShader->setVec3("color", color);
	float vertices[] = {
		position.x			,	position.y			,	0.0f,
		position.x + size.x	,	position.y			,	0.0f,
		position.x + size.x	,	position.y + size.y	,	0.0f,
		position.x			,	position.y + size.y	,	0.0f
	};

	unsigned int indices[] = { 0,1,2,2,3,0 };

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glBufferData(GLenum target,GLsizeiptr size,const void* data,GLenum useage);
	//				��������			ռ�ô�С			����ָ��			ʹ�÷���
	//ʹ�÷�����		GL_STATIC_DRAW	GL_DYNAMIC_DRAW		GL_STREAM_DRAW
	//				���ݲ��ᾭ���ı�	���ݾ����ı�			����һֱ�ı�
	// 	
	//glBindBuffer(GLenum target,GLuint buffer);
	//				��������			������ID


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}