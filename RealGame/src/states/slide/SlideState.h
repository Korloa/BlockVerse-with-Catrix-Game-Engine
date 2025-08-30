/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include "states/basic/State.h"

class SlideState : public State {
private:
	float timer;		//计时器
	std::vector<std::string>imagePath;	//图片路径
	float alpha;		//透明
	int imageIndex;	//当前图片索引

	float lastTime;		//整个显示时间
	float transition;	//渐隐渐显时间

	GLFWwindow* mainWindow;
public:
	SlideState();
	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;

	bool enter(GameController* game) override;
	bool exit(GameController* game) override;
};