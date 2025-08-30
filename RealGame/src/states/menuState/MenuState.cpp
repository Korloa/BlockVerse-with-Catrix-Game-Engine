/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once

#include "states/menuState/MenuState.h"	

#include "states/settingState/SettingState.h"
#include "states/gameLoader/GameLoader.h"
#include "states/singleGameStarter/SingleGameStarter.h"
#include "states/netGameStarter/netGameStarter.h"

#include "render/Shader.h"
#include "render/Camera.h"
#include "world/World.h"

#include "states/loadingState/LoadingState.h"

enum MENU {
	MAIN
};

void MenuState::update(GameController* game,float deltaTime) {
	if (!LoadingState::getInstance().visible) {
		menuCamera->cameraYaw += cameraSpeed * deltaTime;
		if (menuCamera->cameraYaw >= 360.0f) {
			menuCamera->cameraYaw -= 360.0f;
		}

		menuCamera->updateCameraVectors();
	}
}

void MenuState::render(GameController* game) {
	// 3D
	if (menuWorld->generationComplete.load()) {

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);	//深度测试的比较函数
		//GL_NEVER GL_LESS GL_LEQUAL GL_EQUAL GL_GREATER GL_GEQUAL GL_NOTEQUAL GL_ALWAYS
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // 天空蓝
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		menuShader->use();

		glm::mat4 projection = glm::perspective(
			glm::radians(menuCamera->cameraZoom),
			(float)1920.0f / (float)1080.f,
			0.1f, 1000.0f
		);
		menuShader->setMat4("projection", projection);

		glm::mat4 view = menuCamera->getViewMatrix();
		menuShader->setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		menuShader->setMat4("model", model);

		menuShader->setVec3("lightPos", glm::vec3(0.0f, 256.0f, 0.0f));
		menuShader->setVec3("viewPos", menuCamera->cameraPosition);
		menuShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

		menuWorld->render(*menuShader);

		glDisable(GL_DEPTH_TEST); // 避免 UI 被深度测试挡住
		glDisable(GL_CULL_FACE);

	}

	//UI
	glClear(GL_DEPTH_BUFFER_BIT);

	if (pageStack.empty())
		return;
	int curPage = pageStack.back();
	auto it = pageButtons.find(curPage);
	if (it != pageButtons.end()) {

		double mouseX, mouseY;

		GLFWwindow* window = game->getWindowInstance();
		if (window) {
			glfwGetCursorPos(window, &mouseX, &mouseY);
		}


		for (auto& button : it->second) {
			button.render(game, game->textRender, game->rectRender, mouseX, mouseY);
		}
	}

	game->imageRender->renderImage("logo/blockverse_title.png", 960 - 1000.0f / 2, buttonTop + 2 * buttonMargin, 1000.0f, 200.0f,1.0f);
}

bool MenuState::enter(GameController* game) {
	console.info("Entering the Menu State.");


	if (!game->rectRender->initialize() || !game->textRender->initialize("font/pixelFont.ttf",30) || !game->imageRender->initialize()){
		console.error("Fatal Error:cound not initialize rectRender in the MenuState.");
		game->kill();
		return false;
	}

	game->getInputManager().initialize(game->getWindowInstance());

	game->getInputManager().addKeyCallback([&](const keyEvent& event){
		if(event.key == GLFW_KEY_F12 && event.action == GLFW_PRESS) {
			console.toggleVisible();
		}
	});

	menuCamera = new Camera(
		glm::vec3(0.0f, (float)horzion+30.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0.0f, 0.0f
	);

	//背景世界初始

	menuShader = new Shader("render/vertex.glsl", "render/fragment.glsl");

	menuWorld = new World(2757840732);

	LoadingState::getInstance().enter(game); //应当封装的更深，把	menuWorld->startGenerate();当作参数传递给Load结束完自动调用exit
	LoadingState::getInstance().setProgress(-1.0f);
	LoadingState::getInstance().setText("Loading");
	menuWorld->startGenerate();

	//glfwMakeContextCurrent(game->getWindowInstance());

	//if (glewInit() != GLEW_OK) {
	//	console.error("Failed to initialize GLEW");
	//	return false;
	//}

	pageStack.clear();
	pageButtons.clear();

	pageStack.push_back(MENU::MAIN);

	pageButtons.emplace(
		MENU::MAIN,  
		std::vector<Button>{
			Button(glm::vec2(960 - buttonWidth/2, buttonTop), glm::vec2(buttonWidth, buttonHeight), "New Game", [](GameController* g) { g->replaceState(new SingleGameStarter()); }),
			Button(glm::vec2(960 - buttonWidth / 2, buttonTop+ 1* buttonMargin), glm::vec2(buttonWidth, buttonHeight), "Load Game", [](GameController* g) { g->replaceState(new GameLoader()); }),
			Button(glm::vec2(960 - buttonWidth / 2, buttonTop +2* buttonMargin), glm::vec2(buttonWidth, buttonHeight), "Online Game", [](GameController* g) { g->replaceState(new NetGameStarter()); }),
			Button(glm::vec2(960 - buttonWidth / 2, buttonTop +3* buttonMargin), glm::vec2(buttonWidth, buttonHeight), "Settings", [](GameController* g) { g->replaceState(new SettingState()); })
		}
	);

	return true;
}

bool MenuState::exit(GameController* game) {
	console.info("Exiting the Menu State.");
	return true;
}