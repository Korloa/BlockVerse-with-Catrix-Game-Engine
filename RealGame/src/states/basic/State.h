/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include<map>
#include<vector>

#include "components/Button.h"
#include "components/TextInput.h"

#include "core/GameController.h"

class TextRender;
class RectRender;

class State {
protected:
	std::vector<Button>sharedButtons;				//公有按钮，不会随页面切换而切换
	std::vector<int> pageStack;
	std::map<int, std::vector<Button>>pageButtons;
	//std::map<int, std::vector<TextInput>>pageTextInputs;	//尚未实现
	std::map<int, float>keyTimes; //去抖计时

public:
	virtual ~State() = default;
	virtual void procInput(GameController* game, float deltaTime) {}
	virtual void update(GameController* game, float deltaTime) = 0;
	virtual void render(GameController* game) = 0;//等于0子类必须要实现
	virtual bool enter(GameController* game) { return true; }
	virtual bool exit(GameController* game) { return true; }		//{}子类不一定要实现

	virtual void removeInputCallbacks(GLFWwindow* gameWindow) {}
	virtual void setInputCallbacks(GLFWwindow* gameWindow) {}

	bool isKeyPressed(GameController* game, int key, float debounceTime = 0.2f) {
		float currentTime = (float)glfwGetTime();
		if (glfwGetKey(game->getWindowInstance(), key) == GLFW_PRESS &&
			(!keyTimes.count(key) || currentTime - keyTimes[key] > debounceTime)) {
			//这个的作用的是防止短按被识别为多次，这样的效果是长时间按会产生周期性的回复，因为 32行一直在刷新时间
			keyTimes[key] = currentTime;
			return true;
		}
		return false;
	}
};    