#pragma once

#include "states/menuState/MenuState.h"	

#include "states/settingState/SettingState.h"
#include "states/gameLoader/GameLoader.h"
#include "states/singleGameStarter/SingleGameStarter.h"
#include "states/netGameStarter/netGameStarter.h"

enum MENU {
	MAIN
};

void MenuState::update(GameController* game,float deltaTime) {
	//game->kill();
}

void MenuState::render(GameController* game) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (pageStack.empty())
		return;
	int curPage = pageStack.back();
	auto it = pageButtons.find(curPage);
	if (it != pageButtons.end()) {

		double mouseX, mouseY;

		GLFWwindow* window = game->getWindowInstance();
		if (window) {
			glfwGetCursorPos(window, &mouseX, &mouseY);
		}


		for (auto& button : it->second) {
			button.render(game, game->textRender, game->rectRender, mouseX, mouseY);
		}
	}

	game->imageRender->renderImage("logo/blockverse_title.png", 960 - 1000.0f / 2, buttonTop + 2 * buttonMargin, 1000.0f, 200.0f,1.0f);
}

bool MenuState::enter(GameController* game) {
	if (!game->rectRender->initialize() || !game->textRender->initialize("font/pixelFont.ttf",30) || !game->imageRender->initialize()){
		console.error("Fatal Error:cound not initialize rectRender in the MenuState.");
		game->kill();
		return false;
	}

	game->getInputManager().initialize(game->getWindowInstance());

	game->getInputManager().addKeyCallback([&](const keyEvent& event){
		if(event.key == GLFW_KEY_F12 && event.action == GLFW_PRESS) {
			console.toggleVisible();
		}
	});


	//glfwMakeContextCurrent(game->getWindowInstance());

	//if (glewInit() != GLEW_OK) {
	//	console.error("Failed to initialize GLEW");
	//	return false;
	//}

	pageStack.clear();
	pageButtons.clear();

	pageStack.push_back(MENU::MAIN);

	pageButtons.emplace(
		MENU::MAIN,  
		std::vector<Button>{
			Button(glm::vec2(960 - buttonWidth/2, buttonTop), glm::vec2(buttonWidth, buttonHeight), "New Game", [](GameController* g) { g->replaceState(new SingleGameStarter()); }),
			Button(glm::vec2(960 - buttonWidth / 2, buttonTop+ 1* buttonMargin), glm::vec2(buttonWidth, buttonHeight), "Load Game", [](GameController* g) { g->replaceState(new GameLoader()); }),
			Button(glm::vec2(960 - buttonWidth / 2, buttonTop +2* buttonMargin), glm::vec2(buttonWidth, buttonHeight), "Online Game", [](GameController* g) { g->replaceState(new NetGameStarter()); }),
			Button(glm::vec2(960 - buttonWidth / 2, buttonTop +3* buttonMargin), glm::vec2(buttonWidth, buttonHeight), "Settings", [](GameController* g) { g->replaceState(new SettingState()); })
		}
	);


	console.info("Entering the Menu State.");
	return true;
}

bool MenuState::exit(GameController* game) {
	console.info("Exiting the Menu State.");
	return true;
}