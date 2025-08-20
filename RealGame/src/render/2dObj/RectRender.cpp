#include "render/2dObj/RectRender.h"
#include "render/Shader.h"

RectRender::RectRender():rectShader(nullptr),vao(0),vbo(0),ebo(0){}

//?? textRender需要vao,vbo rectrender需要vao,vbo,veo

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

	//创建一个Vertex Array Object
	//vao只记录vbo的相关配置信息，真实数据保存在vbo
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	//glGenxxx(GLsizei N,GLuint* names)函数格式比较固定，用于预先获取GPU资源的ID，N为申请数量，返回值在names数组中
	//值得注意一个结构
	//glGenxxx() --->  glBindxxx() ---->  glBufferData()
	//分配资源ID        声明数据结构			上传数据
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//	固定

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
	//				缓存类型			占用大小			数据指针			使用方法
	//使用方法：		GL_STATIC_DRAW	GL_DYNAMIC_DRAW		GL_STREAM_DRAW
	//				数据不会经常改变	数据经常改变			数据一直改变
	// 	
	//glBindBuffer(GLenum target,GLuint buffer);
	//				缓存类型			缓存器ID


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}