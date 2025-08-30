/*
 * Copyright (c) 2025 Kaixiang Zhang (�ſ���). All rights reserved.
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

class GameController;			//ǰ������Game

struct Button {
	glm::vec2 position;	//��ť���½�λ��
	glm::vec2 size;		//��ť���

	std::string text;	//��ť�ı�
	std::function<void(GameController*)> onClick;	//����ص�����

	glm::vec3 bgColor =		 { 0.6f,0.6f,0.6f };		//��ť������ɫ
	glm::vec3 hoverBgColor = { 0.8f,0.8f,1.0f };		//����ʱ��ť������ɫ
	glm::vec3 textColor =	 { 0.0f,1.0f,1.0f };        //�ı���ɫ
	//glm::vec3 lineColor =	 {};						//�߿���ɫ
	//glm::vec3 hoverLineColor = {};					//����ʱ�߿���ɫ
	glm::vec3 disabledBgColor = { 0.4f,0.4f,0.4f };		//����״̬������ɫ
	//glm::vec3 disabledLineColor = {};					//����״̬�߿���ɫ

	bool enabled = true;

	
	bool isHovered(double mouseX, double mouseY) {	//�����ͣ
		return	mouseX > position.x &&
			mouseX < position.x + size.x &&
			mouseY > position.y &&
			mouseY < position.y + size.y;
	}

	//render��Ⱦʱ���������룬�Ҷ��룬���Ķ�������ѡ��
	void render(GameController* game,TextRender* textRender,RectRender* rectRender ,double mouseX, double mouseY);

	Button(const glm::vec2& pos, const glm::vec2& size, const std::string& text, std::function<void(GameController*)>callback);
};