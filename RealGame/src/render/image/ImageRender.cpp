#include "render/image/ImageRender.h"
#include "render/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//glVertexAttribPointer(
// GLuint index //顶点属性索引
// GLint size 每个顶点的分量数
// GLenum type 数据类型
// GLboolean normalized 是否归一化
// GLsizei stride 步长
// const void* pointer 起始偏移量
// )
ImageRender::ImageRender():imageShader(nullptr),vao(0),vbo(0),ebo(0){}


ImageRender::~ImageRender() {
	if (imageShader)
		delete imageShader;
	if (vao)
		glDeleteVertexArrays(1, &vao);
	if (vbo)
		glDeleteBuffers(1, &vbo);
	clearCache();
}

// Gen -> bind -> attribute -> enable -> unbind
bool ImageRender::initialize() {
	//v1.0
	//imageShader = new Shader("image_vertex.glsl", "image_fragment.glsl");

	//glGenVertexArrays(1, &vao);
	//glGenBuffers(1, &vbo);
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0);

	//return true;
	console.info("Initializing the ImageRender...");

	imageShader = new Shader("render/image_vertex.glsl", "render/image_fragment.glsl");

	float vertices[] = {
		0.0f,0.0f,0.0f,0.0f,
		1.0f,0.0f,1.0f,0.0f,
		1.0f,1.0f,1.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f
	};
	GLuint indices[] = { 0,1,2,0,2,3 };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	return true;
}

void ImageRender::renderTexture(GLuint textureID, float x, float y, float width, float height, float alpha) {
	if (vao == 0 || vbo == 0 || ebo == 0) {
		console.error("ImageRender:No VAO,VBO or EBO accessbile for rendering texture");
		return;
	}
	int screenWidth = 1920, screenHeight = 1080;

	GLFWwindow* window = glfwGetCurrentContext();
	if (window) {
		glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	}
	imageShader->use();

	imageShader->setVec2("u_Position", x, y);
	imageShader->setVec2("u_Size", width, height);
	imageShader->setVec2("u_ScreenSize", (float)screenWidth, (float)screenHeight);
	imageShader->setFloat("u_Alpha", alpha);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	imageShader->setInt("u_Texture", 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint ImageRender::loadTexture(const std::string& path) {
	auto it = textureCache.find(path);
	if (it != textureCache.end()) {
		return it->second;
	}
	int w, h, ch;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 0);
	if (!data) {
		console.error("ImageRender: Failed to load the image");
		return 0;
	}
	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	GLenum format = (ch == 1) ? GL_RED : (ch == 3) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	textureCache[path] = tex;
	return tex;
}


void ImageRender::renderImage(const std::string& path, float x, float y, float width, float height, float alpha) {
	GLuint texID = loadTexture(path);
	if (!texID) {
		return;
	}
	renderTexture(texID, x, y, width, height, alpha);
}

void ImageRender::clearCache() {  
	for (auto& pair : textureCache) {
		glDeleteTextures(1, &pair.second);
	}
	textureCache.clear();
}

void ImageRender::clearCache(const std::string& path) {
	auto it = textureCache.find(path);
	if (it != textureCache.end()) {
		glDeleteTextures(1, &it->second);
		textureCache.erase(it);
	}
}


void ImageRender::getImageAttrib(const std::string& path,imageInfo* img) {
	stbi_load(path.c_str(), &(img->width), &(img->height), &(img->ch), 0);
}