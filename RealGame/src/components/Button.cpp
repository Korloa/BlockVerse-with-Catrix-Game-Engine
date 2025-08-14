#include "Button.h"
#include "core/Game.h"

void Button::render(Game* game, double mouseX, double mouseY) {
	//Ҫ�Ľ���������ÿ��game��Ӧһ��render��Ⱦ����render������Rect,Text��Ⱦ����game
	//�ڶ����Ľ��������12.0f�ǹ���ģ���Ӧ����һ��ȫ��״̬��������ʲô������Ӧ�ô��ж�ȡ
	// ����.config�ļ� ��������ߵģ���Ӧ��Ūһ��ȫ�ֵģ��ǳ�������ʱ�ģ��ɾ�̬�ļ����豸�Ƶ���
	if (enabled) {
		game->renderRect(position, size, isHovered(mouseX, mouseY) ? hoverBgColor : bgColor);
	}
	else {
		game->renderRect(position, size, disabledBgColor);
	}
	float textX = position.x+(size.x/2.0f)-(text.length()*12.0f/2.0f);
	float textY = position.y+(size.y/2.0f)-12.0f;
	game->renderText(text, glm::vec2(textX, textY),1.0f,textColor);
}