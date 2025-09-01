/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */
#include "states/slide/SlideState.h"
#include "states/menuState/MenuState.h"

#include "utils/GLFWextension.h"
//enter和构造函数还是有所区别的
SlideState::SlideState() :timer(0.0f),alpha(0.0f),transition(0.5f),lastTime(2.5f),imageIndex(0),mainWindow(nullptr){
	imagePath = {
		"slide/slide_engine.png",
		"slide/slide_game.png"
	};
};

void SlideState::update(GameController* game, float deltaTime) {
	timer += deltaTime;
	if (timer <= transition) {
		alpha = timer / transition;
	}
	else if (timer > transition && timer <= (lastTime - transition)) {
		alpha = 1.0f;
	}
	else if (timer > (lastTime - transition) && timer <= lastTime) {
		alpha = 1.0f - (transition-lastTime+timer)/transition;
	}//else表示一个幻灯片播放完毕了
	else {

		alpha = 0.0f;
		timer = 0.0f;

		imageIndex++;
		//一轮循环已经结束
		if (imageIndex == imagePath.size()) {
			game->replaceState(new MenuState());
			return;
		}
	}
}

void SlideState::render(GameController* game) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	imageInfo* tempImg = new imageInfo();
	game->imageRender->getImageAttrib(imagePath[imageIndex], tempImg);
	game->imageRender->renderImage(
		imagePath[imageIndex],     // 图片路径
		960 - tempImg->width/2, 540-tempImg->height/2,                    // 左下角 x, y
		tempImg->width, tempImg->height,                    // 宽高
		alpha                        // 透明度
	);

	delete tempImg;

}

bool SlideState::enter(GameController* game) {
	console.info("Entering the SlideState State.");

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	mainWindow = glfwCreateWindow(videoMode->width, videoMode->height, "BlockVerse",NULL, NULL);
	game->setWindowInstance(mainWindow);

	glfwMakeContextCurrent(mainWindow);

	setWindowIcon(mainWindow, "logo/blockverse_logo.png");

	if (glewInit() != GLEW_OK) {
		console.error("Failed to initialize GLEW");
		return false;
	}

	if (!game->imageRender->initialize()) {
		console.error("Failed to initialize ImageRender");
		return false;
	}

	return true;
}

bool SlideState::exit(GameController* game) {
	console.info("Exiting the SlideState State.");
	return true;
}