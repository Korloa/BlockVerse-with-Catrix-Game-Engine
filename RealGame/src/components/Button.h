#pragma once
#include <glm/glm.hpp>
#include <string>
#include <functional>

class Game;			//ǰ������Game

struct Button {
	glm::vec2 position;	//��ť���½�λ��
	glm::vec2 size;		//��ť���

	std::string text;	//��ť�ı�
	std::function<void(Game*)> onClick;	//����ص�����

	glm::vec3 bgColor =		 { 0.6f,0.6f,0.6f };		//��ť������ɫ
	glm::vec3 hoverBgColor = { 0.8f,0.8f,1.0f };		//����ʱ��ť������ɫ
	glm::vec3 textColor =	 { 1.0f,1.0f,1.0f };        //�ı���ɫ
	//glm::vec3 lineColor =	 {};						//�߿���ɫ
	//glm::vec3 hoverLineColor = {};					//����ʱ�߿���ɫ
	glm::vec3 disabledBgColor = { 0.4f,0.4f,0.4f };		//����״̬������ɫ
	//glm::vec3 disabledLineColor = {};					//����״̬�߿���ɫ

	bool enabled = true;

	
	bool isHovered(double mouseX, double mouseY) {	//�����ͣ
		return	mouseX > position.x				&&
				mouseX < position.x + size.x	&&
				mouseY > position.y				&&
				mouseY < position.y + size.y;
	}

	//render��Ⱦʱ���������룬�Ҷ��룬���Ķ�������ѡ��
	void render(Game* game, double mouseX, double mouseY);
};