#include "states/gameLoader/GameLoader.h"

void GameLoader::update(GameController* game, float deltaTime) {

}

void GameLoader::render(GameController* game) {

}

bool GameLoader::exit(GameController* game) {
	console.info("Exiting the GameLoader State.");
	return true;
}

bool GameLoader::enter(GameController* game) {
	console.info("Entering the GameLoader State.");
	return true;
}