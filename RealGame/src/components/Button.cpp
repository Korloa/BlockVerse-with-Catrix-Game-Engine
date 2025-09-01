/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "Button.h"
#include "core/GameController.h"
#include "core/InputManager.h"

void Button::render(GameController* game,TextRender* textRender,RectRender* rectRender, double mouseX, double mouseY) {
	//要改进两个方向，每个game对应一个render渲染器，render来接手Rect,Text渲染而非game
	//第二个改进方向就是12.0f是估算的，是应当有一个全局状态管理器，什么参数都应该从中读取
	// 比如.config文件 这个是离线的，还应该弄一个全局的，是程序运行时的，由静态文件和设备推导的
	if (enabled) {
		rectRender->renderRect(position, size, isHovered(mouseX, mouseY) ? hoverBgColor : bgColor);
	}
	else {
		rectRender->renderRect(position, size, disabledBgColor);
	}
	//默认是居中的
	float textX = position.x+(size.x/2.0f)-(text.length()*12.0f/2.0f);
	float textY = position.y+(size.y/2.0f)+12.0f;
	textRender->renderText(text, textX, textY,1.0f,textColor,true);

}

Button::Button(const glm::vec2& pos, const glm::vec2& size, const std::string& text, std::function<void(GameController*)>callback) :position(pos),size(size),text(text),onClick(callback){
}

void Button::initialize() {
	InputManager& im = InputManager::getInstance();
	funcId = im.addMouseButtonCallback([&](const mouseButtonEvent& event) {
		if (event.mouseX > position.x &&
			event.mouseX < position.x + size.x &&
			event.mouseY > position.y &&
			event.mouseY < position.y + size.y && event.action == GLFW_PRESS && !remove) {
			onClick(gameInstance);
		}
	});
}

Button::~Button() {
	console.info("Button uninstalled");
}

void Button::removeCallback() {//这里应该加入多态
	InputManager& im = InputManager::getInstance();
	if (funcId < 0)
		return;
	remove = true;
	im.removeMouseButtonCallback(funcId);
}