/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
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
/*���ڴ������ڣ������������������OpenGL������*/
#include <glm/glm.hpp>
/*openGL mathematics
һ��ΪOpenGL��Ƶ���ѧ��*/
#include <glm/gtc/matrix_transform.hpp>
/*�������任*/

#include "render/Shader.h"

#include "world/World.h"

#include "render/Camera.h"

#include "components/Button.h"

#include "render/text/TextRender.h"

#include "render/2dObj/RectRender.h"


class Game {
public:
	Game();
	~Game();

	bool initialize();
	void run();
	void cleanup();
	// ��Ϸ����
	GLFWwindow* gameWindow;
private:
	// ��Ӱ
	Shader* shader;
	// �����
	Camera* camera;
	// ������
	World* world;

	TextRender* textRender; // ��� TextRenderer
	RectRender* rectRender; // ��� RectRenderer
	Button* testButton; // ���԰�ť


	float deltaTime;
	float lastFrame;

	bool firstMouse;
	float lastX, lastY;

	void processInput();
	void update();
	void render();
	// �ص���������
	static void framebuffer_size_callback(GLFWwindow* gameWindow, int width, int height);
	static void mouse_callback(GLFWwindow* gameWindow, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* gameWindow, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};


#endif
