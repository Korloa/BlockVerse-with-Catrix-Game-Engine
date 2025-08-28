#include "utils/GLFWextension.h"

void setWindowIcon(GLFWwindow* window,const char* iconPath) {
	GLFWimage images[1];
	int width, height, ch;
	unsigned char* data = stbi_load(iconPath, &width, &height, &ch, 4);
	if (!data) {
		console.error("Fail to load OpenGL window Icon.");
		return;
	}

	images[0].width = width;
	images[0].height = height;
	images[0].pixels = data;
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(data);
}