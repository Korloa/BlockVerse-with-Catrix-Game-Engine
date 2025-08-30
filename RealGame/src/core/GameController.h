/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "debug/Console.h"

#include "core/gameConfig.h"

#include "render/text/TextRender.h"
#include "render/2dObj/RectRender.h"
#include "render/image/ImageRender.h"

#include "core/InputManager.h"

class LoadingState;

// State��Ӧ��ʹ��ǰ�����������������ѭ������ͷ�ļ������⣬�����˱������������������Ļ������˳������
class State;

class GameController {
private:
	gameConfig* cfg;

	bool gameAlive = true;

	std::vector<State*>stateStack;

	GLFWwindow* gameWindow;

	float deltaTime;
	float lastFrame;
	bool firstMouse;
	double lastX, lastY;
	bool isGamePaused;

	float calcFrame();
	GameController(const GameController&) = delete;

	GameController& operator=(const GameController&) = delete;
	
public:
	gameConfig* getConfig() { return cfg; }

	InputManager& getInputManager() { return InputManager::getInstance(); }

	LoadingState* loadingState;

	TextRender* textRender;
	ImageRender* imageRender;
	RectRender* rectRender;

	bool isControllerAlive() { return gameAlive; }
	void kill();

	GameController();
	~GameController();

	Console console;

	GLFWwindow* getWindowInstance() { return gameWindow;}
	void setWindowInstance(GLFWwindow* window) { gameWindow = window; }

	bool initialize();
	void run();
	void cleanup();

	//״̬����
	void pushState(State* state);
	void popState();
	void replaceState(State* state);

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

extern GameController* gameInstance;
