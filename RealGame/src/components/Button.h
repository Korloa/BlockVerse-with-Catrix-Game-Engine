/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#pragma once
#include <glm/glm.hpp>
#include <string>
#include <functional>
#include "render/2dObj/RectRender.h"
#include "render/text/TextRender.h"

class GameController;			//前向声明Game

struct Button {
	glm::vec2 position;	//按钮左下角位置
	glm::vec2 size;		//按钮宽高

	std::string text;	//按钮文本
	std::function<void(GameController*)> onClick;	//点击回调函数

	glm::vec3 bgColor =		 { 0.6f,0.6f,0.6f };		//按钮背景颜色
	glm::vec3 hoverBgColor = { 0.8f,0.8f,1.0f };		//悬浮时按钮背景颜色
	glm::vec3 textColor =	 { 0.0f,1.0f,1.0f };        //文本颜色
	//glm::vec3 lineColor =	 {};						//边框颜色
	//glm::vec3 hoverLineColor = {};					//悬浮时边框颜色
	glm::vec3 disabledBgColor = { 0.4f,0.4f,0.4f };		//禁用状态背景颜色
	//glm::vec3 disabledLineColor = {};					//禁用状态线框颜色

	bool enabled = true;

	
	bool isHovered(double mouseX, double mouseY) {	//鼠标悬停
		return	mouseX > position.x &&
			mouseX < position.x + size.x &&
			mouseY > position.y &&
			mouseY < position.y + size.y;
	}

	//render渲染时会加入左对齐，右对齐，中心对齐三个选项
	void render(GameController* game,TextRender* textRender,RectRender* rectRender ,double mouseX, double mouseY);

	Button(const glm::vec2& pos, const glm::vec2& size, const std::string& text, std::function<void(GameController*)>callback);
};