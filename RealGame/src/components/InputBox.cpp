/*
 * Copyright (c) 2025 Kaixiang Zhang (张凯翔). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [InputBox.cpp]
 * Description: [A commponent to receive the text and display it.]
 */

#include "components/InputBox.h"
#include "core/GameController.h"

InputBox::InputBox():position(0.0f,0.0f),size(500.0f,50.0f),cursorPos(0),isFocused(false),cursorBlinkTime(0.0f),cursorVisible(false),keyId(0){
}
InputBox::~InputBox() {
	InputManager& im = InputManager::getInstance();
	if (!keyId) {
		im.removeKeyCallback(keyId);
	}
}

bool InputBox::initialize(const std::string& fontPath,unsigned int fontSize,glm::vec2 pos){
	if (!gameInstance->rectRender->initialize() || !gameInstance->textRender->initialize("font/pixelFont.ttf",26)) {
		return false;
	}
	position.x = pos.x - size.x/2;
	position.y = pos.y - size.y / 2;

	InputManager& im = InputManager::getInstance();

	im.addMouseButtonCallback([this](const mouseButtonEvent& event){
		this->onMouseClick(event.mouseX, event.mouseY);
	});

	keyId = im.addKeyCallback([this](const keyEvent& event) {
		this->onKeyPress(event);
	});


	keyToCharMap[GLFW_KEY_0] = '0';
	keyToCharMap[GLFW_KEY_1] = '1';
	keyToCharMap[GLFW_KEY_2] = '2';
	keyToCharMap[GLFW_KEY_3] = '3';
	keyToCharMap[GLFW_KEY_4] = '4';
	keyToCharMap[GLFW_KEY_5] = '5';
	keyToCharMap[GLFW_KEY_6] = '6';
	keyToCharMap[GLFW_KEY_7] = '7';
	keyToCharMap[GLFW_KEY_8] = '8';
	keyToCharMap[GLFW_KEY_9] = '9';

	keyToCharShiftMap[GLFW_KEY_0] = ')';
	keyToCharShiftMap[GLFW_KEY_1] = '!';
	keyToCharShiftMap[GLFW_KEY_2] = '@';
	keyToCharShiftMap[GLFW_KEY_3] = '#';
	keyToCharShiftMap[GLFW_KEY_4] = '$';
	keyToCharShiftMap[GLFW_KEY_5] = '%';
	keyToCharShiftMap[GLFW_KEY_6] = '^';
	keyToCharShiftMap[GLFW_KEY_7] = '&';
	keyToCharShiftMap[GLFW_KEY_8] = '*';
	keyToCharShiftMap[GLFW_KEY_9] = '(';

	for (int key = GLFW_KEY_A; key <= GLFW_KEY_Z; key++) {
		keyToCharMap[key] = 'a' + (key - GLFW_KEY_A);
		keyToCharShiftMap[key] = 'A' + (key - GLFW_KEY_A);
	}

	keyToCharMap[GLFW_KEY_SPACE] = ' ';
	keyToCharMap[GLFW_KEY_MINUS] = '-';
	keyToCharShiftMap[GLFW_KEY_MINUS] = '_';
	keyToCharMap[GLFW_KEY_EQUAL] = '=';
	keyToCharShiftMap[GLFW_KEY_EQUAL] = '+';
	keyToCharMap[GLFW_KEY_LEFT_BRACKET] = '[';
	keyToCharShiftMap[GLFW_KEY_LEFT_BRACKET] = '{';
	keyToCharMap[GLFW_KEY_RIGHT_BRACKET] = ']';
	keyToCharShiftMap[GLFW_KEY_RIGHT_BRACKET] = '}';
	keyToCharMap[GLFW_KEY_BACKSLASH] = '\\';
	keyToCharShiftMap[GLFW_KEY_BACKSLASH] = '|';
	keyToCharMap[GLFW_KEY_SEMICOLON] = ';';
	keyToCharShiftMap[GLFW_KEY_SEMICOLON] = ':';
	keyToCharMap[GLFW_KEY_APOSTROPHE] = '\'';
	keyToCharShiftMap[GLFW_KEY_APOSTROPHE] = '"';
	keyToCharMap[GLFW_KEY_COMMA] = ',';
	keyToCharShiftMap[GLFW_KEY_COMMA] = '<';
	keyToCharMap[GLFW_KEY_PERIOD] = '.';
	keyToCharShiftMap[GLFW_KEY_PERIOD] = '>';
	keyToCharMap[GLFW_KEY_SLASH] = '/';
	keyToCharShiftMap[GLFW_KEY_SLASH] = '?';
	keyToCharMap[GLFW_KEY_GRAVE_ACCENT] = '`';
	keyToCharShiftMap[GLFW_KEY_GRAVE_ACCENT] = '~';

	return true;
}

void InputBox::onKeyPress(const keyEvent& event) {
	if (!isFocused || event.action != GLFW_PRESS) return;

	// 处理功能键
	switch (event.key) {
	case GLFW_KEY_BACKSPACE:
		if (cursorPos > 0) {
			text.erase(cursorPos - 1, 1);
			cursorPos--;
		}
		break;
	case GLFW_KEY_DELETE:
		if (cursorPos < text.length()) {
			text.erase(cursorPos, 1);
		}
		break;
	case GLFW_KEY_LEFT:
		if (cursorPos > 0) cursorPos--;
		break;
	case GLFW_KEY_RIGHT:
		if (cursorPos < text.length()) cursorPos++;
		break;
	case GLFW_KEY_HOME:
		cursorPos = 0;
		break;
	case GLFW_KEY_END:
		cursorPos = text.length();
		break;
	case GLFW_KEY_ENTER:
	case GLFW_KEY_KP_ENTER:
		// 可以在这里处理回车确认事件
		break;
	default:
		// 处理可打印字符
		if (maxLength > 0 && text.length() >= maxLength) {
			// 已达到最大长度限制
			break;
		}

		// 检查是否是可打印字符
		char c = 0;
		bool hasShift = (event.mods & GLFW_MOD_SHIFT);

		if (hasShift && keyToCharShiftMap.find(event.key) != keyToCharShiftMap.end()) {
			c = keyToCharShiftMap[event.key];
		}
		else if (keyToCharMap.find(event.key) != keyToCharMap.end()) {
			c = keyToCharMap[event.key];
		}

		if (c != 0) {
			text.insert(cursorPos, 1, c);
			cursorPos++;
		}
		break;
	}

	// 重置光标闪烁
	cursorVisible = true;
	cursorBlinkTime = 0.0f;
}

void InputBox::render() {
	gameInstance->rectRender->renderRect(position - glm::vec2(6.0f, 6.0f),
		size + glm::vec2(12.0f, 12.0f),
		isFocused ? glm::vec3(0.2f, 0.5f, 0.8f) : glm::vec3(0.9f, 0.9f, 0.9f));
	gameInstance->rectRender->renderRect(position, size, glm::vec3(0.0f, 0.0f, 0.0f));
	
	if (!text.empty()) {
		gameInstance->textRender->renderText(text, position.x + 5.0f, position.y + (size.y + 20.0f) / 2.0f,
			1.0f, glm::vec3(1.0f, 1.0f, 1.0f), true);
	}
	else {
		gameInstance->textRender->renderText("Input the world seed.", position.x + 5.0f, position.y + (size.y + 20.0f) / 2.0f,
			1.0f, glm::vec3(0.2f, 0.2f, 0.2f), true);
	}

	if (isFocused && cursorVisible) {
		drawCursor();
	}
}

void InputBox::drawCursor() {
	std::string textBeforeCursor = text.substr(0, cursorPos);
	float textWidth = gameInstance->textRender->getTextWidth(textBeforeCursor, 1.0f);
	float cursorX = position.x + 5.0f + textWidth;
	float cursorTop = position.y + 5.0f;
	float cursorBottom = position.y + size.y - 5.0f;
	gameInstance->rectRender->renderRect(
		glm::vec2(cursorX, cursorTop),
		glm::vec2(1.0f, cursorBottom - cursorTop),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
}

void InputBox::update(float deltaTime) {
	if (!isFocused)
		return;
	cursorBlinkTime += deltaTime;
	if (cursorBlinkTime >= 0.5f) {
		cursorVisible = !cursorVisible;
		cursorBlinkTime -= 0.5f;
	}
}

void InputBox::onMouseClick(int x, int y) {
	bool wasFocus = isFocused;
	isFocused = (x >= position.x && x <= position.x + size.x &&
		y >= position.y && y <= position.y + size.y);

	if (isFocused && !wasFocus) {
		cursorPos = text.length();
		cursorVisible = true;
		cursorBlinkTime = 0.0f;
	}
	else if (!isFocused) {
		cursorVisible = false;
	}
}