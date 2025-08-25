#pragma once
#include "states/menuState/MenuState.h"	

void MenuState::update(GameController* game,float deltaTime) {
	game->kill();
}

void MenuState::render(GameController* game) {

}

bool MenuState::enter(GameController* game) {
	console.info("Entering the Menu State.");
	return true;
}

bool MenuState::exit(GameController* game) {
	console.info("Exiting the Menu State.");
	return true;
}