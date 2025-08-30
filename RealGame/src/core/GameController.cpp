/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */


#include "core/GameController.h"

#include "states/basic/State.h"
#include "states/flashScreen/FlashScreen.h"		//Flash
#include "states/slide/SlideState.h"			//幻灯片
#include "states/gameState/GameState.h"			//游戏中
#include "states/settingState/SettingState.h"	//设置
#include "states/menuState/MenuState.h"			//菜单
#include "states/loadingState/LoadingState.h"	//过渡
#include "core/InputManager.h"

GameController* gameInstance = nullptr;

GameController::GameController() :gameWindow(nullptr), deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(0), lastY(0),
								isGamePaused(false),textRender(nullptr),rectRender(nullptr),imageRender(nullptr),cfg(nullptr){
	gameInstance = this;
}

GameController::~GameController() {
	cleanup();
}

bool GameController::initialize() {
	console.setConsoleVisible(true);

	cfg = new gameConfig();

	glfwInit();

	loadingState = &LoadingState::getInstance();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	textRender = new TextRender();
	rectRender = new RectRender();
	imageRender = new ImageRender();
	//if (!textRender->initialize("font/pixelFont.ttf", 24)) {
	//	console.info("Failed to initialize TextRenderer");
	//	return false;
	//}
	//if (!rectRender->initialize()) {
	//	console.info("Failed to initialize RectRenderer");
	//	return false;
	//}
	//if (!imageRender->initialize()) {
	//	console.info("Failed to initialize ImageRenderer");
	//	return false;
	//}
	
	pushState(new FlashScreen());
	return true;
}

float GameController::calcFrame() {
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	return deltaTime;
}

void GameController::run() {
	while (isControllerAlive()) {
		//存在一些问题，无法关闭窗口
		float delta = calcFrame();
		if (!stateStack.empty()) {
			stateStack.back()->update(this, delta);
			if (loadingState->visible) {
				loadingState->render(this);
				loadingState->update(this,delta);
				loadingState->procInput(this, delta);
			}
			else {
				stateStack.back()->procInput(this, delta);
				stateStack.back()->render(this);
			}
		}
		//双缓冲机制
		if (gameWindow) {
			glfwSwapBuffers(gameWindow);
			glfwPollEvents();
		}
	}
}

void GameController::pushState(State* newState) {
	if (!stateStack.empty()) {
		stateStack.back()->exit(this);
		stateStack.back()->removeInputCallbacks(gameWindow);
	}
	stateStack.push_back(newState);
	newState->enter(this);
	newState->setInputCallbacks(gameWindow);
}

void GameController::popState() {

	if (stateStack.empty()) {
		console.error("No more states existing in the main stack.");
		return;
	}

	stateStack.back()->exit(this);
	stateStack.back()->removeInputCallbacks(gameWindow);
	delete stateStack.back();
	stateStack.pop_back();

	if (stateStack.empty())
		console.warn("Empty state stack after popState.");
}

void GameController::replaceState(State* newState) {
	if (!stateStack.empty()) {
		stateStack.back()->exit(this);
		stateStack.back()->removeInputCallbacks(gameWindow);
		delete stateStack.back();
		stateStack.pop_back();
	}
	stateStack.push_back(newState);
	newState->enter(this);
	newState->setInputCallbacks(gameWindow);
}

void GameController::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void GameController::cleanup() {
	while (!stateStack.empty()) {
		popState();
	}

	if (cfg)
		delete cfg;

	if (textRender)
		delete textRender;
	if (imageRender)
		delete imageRender;
	if (rectRender)
		delete rectRender;
	
	glfwTerminate();
}

void GameController::kill() {
	console.info("Recive the exit command."); 
	//gameAlive = false; 
}