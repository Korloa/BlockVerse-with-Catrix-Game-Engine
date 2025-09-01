/*
 * Copyright (c) 2025 Kaixiang Zhang (ÕÅ¿­Ïè). All rights reserved.
 * Author: Kaixiang Zhang
 * File: [Chunk.cpp]
 * Description: [Control the generation logic of the chunk.]
 */

#include "states/singleGameStarter/SingleGameStarter.h"
#include "states/gameState/GameState.h"

void SingleGameStarter::update(GameController* game, float deltaTime) {
	seedInput->update(deltaTime);
}

void SingleGameStarter::render(GameController* game) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	seedInput->render();

	double mouseX, mouseY;

	GLFWwindow* window = game->getWindowInstance();
	if (window) {
		glfwGetCursorPos(window, &mouseX, &mouseY);
	}



	for (auto& btn : sharedButtons) {
		btn.render(game, game->textRender, game->rectRender, mouseX, mouseY);
	}

	if (pageStack.empty())
		return;
	int curPage = pageStack.back();
	auto it = pageButtons.find(curPage);
	if (it != pageButtons.end()) {
		for (auto& button : it->second) {
			button.render(game, game->textRender, game->rectRender, mouseX, mouseY);
		}
	}
}

bool SingleGameStarter::exit(GameController* game) {
	console.info("Exiting the SingleGame State.");
	for (auto& btn : pageButtons[SINGLELOAD::DEFAULT]) {
		btn.removeCallback();
	}
	for (auto& btn : sharedButtons) {
		btn.removeCallback();
	}
	return true;
}

bool SingleGameStarter::enter(GameController* game) {
	console.info("Entering the SingleGame State.");

	seedInput = new InputBox();

	seedInput->initialize("font/pixelFont.ttf", 25, {960,540});

	pageStack.clear();
	pageButtons.clear();

	pageStack.push_back(SINGLELOAD::DEFAULT);

	sharedButtons.push_back(Button(glm::vec2(960 - buttonWidth / 2, 600), glm::vec2(buttonWidth, buttonHeight), "Create World", [&](GameController* g) { g->replaceState(new GameState(std::stoi(seedInput->getValue()))); }));
	pageButtons.emplace(
		SINGLELOAD::DEFAULT,
		std::vector<Button>{
		Button(glm::vec2(960 - buttonWidth / 2, 1000), glm::vec2(buttonWidth, buttonHeight), "Back", [this](GameController* g) { this->back(g); }),
	});

	for (auto& btn : pageButtons[SINGLELOAD::DEFAULT]) {
		btn.initialize();
	}
	for (auto& btn : sharedButtons) {
		btn.initialize();
	}

	return true;
}

SingleGameStarter::SingleGameStarter():seedInput(nullptr){
}
SingleGameStarter::	~SingleGameStarter() {

}