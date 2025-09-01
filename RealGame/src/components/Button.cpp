/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "Button.h"
#include "core/GameController.h"
#include "core/InputManager.h"

void Button::render(GameController* game,TextRender* textRender,RectRender* rectRender, double mouseX, double mouseY) {
	//Ҫ�Ľ���������ÿ��game��Ӧһ��render��Ⱦ����render������Rect,Text��Ⱦ����game
	//�ڶ����Ľ��������12.0f�ǹ���ģ���Ӧ����һ��ȫ��״̬��������ʲô������Ӧ�ô��ж�ȡ
	// ����.config�ļ� ��������ߵģ���Ӧ��Ūһ��ȫ�ֵģ��ǳ�������ʱ�ģ��ɾ�̬�ļ����豸�Ƶ���
	if (enabled) {
		rectRender->renderRect(position, size, isHovered(mouseX, mouseY) ? hoverBgColor : bgColor);
	}
	else {
		rectRender->renderRect(position, size, disabledBgColor);
	}
	//Ĭ���Ǿ��е�
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

void Button::removeCallback() {//����Ӧ�ü����̬
	InputManager& im = InputManager::getInstance();
	if (funcId < 0)
		return;
	remove = true;
	im.removeMouseButtonCallback(funcId);
}