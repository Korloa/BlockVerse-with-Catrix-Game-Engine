/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [FileName]
 * Description: [Brief description]
 */
#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include <GL/glew.h>
/*glew openGL extension Wrangler  
glEnable glCreateShader*/
#include <GLFW/glfw3.h>
/*用于创建窗口，处理输入输出，管理OpenGL上下文*/
#include <glm/glm.hpp>
/*openGL mathematics
一个为OpenGL设计的数学库*/
#include <glm/gtc/matrix_transform.hpp>
/*处理矩阵变换*/

#include "render/Shader.h"

#include "world/World.h"

#include "render/Camera.h"

class Game {
public:
	Game();
	~Game();

	bool initialize();
	void run();
	void cleanup();

private:
	// 游戏窗口
	GLFWwindow* gameWindow;
	// 光影
	Shader* shader;
	// 摄像机
	Camera* camera;
	// 主世界
	World* world;

	float deltaTime;
	float lastFrame;

	bool firstMouse;
	float lastX, lastY;

	void processInput();
	void update();
	void render();
	// 回调函数集合
	static void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height);
	static void mouse_callback(GLFWwindow* gameWindow, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* gameWindow, double xoffset, double yoffset);


	//temp tool render set
	void renderRect();
	void renderText();
};


#endif
