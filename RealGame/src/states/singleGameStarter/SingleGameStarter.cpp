#include "states/singleGameStarter/SingleGameStarter.h"

void SingleGameStarter::update(GameController* game, float deltaTime) {

}

void SingleGameStarter::render(GameController* game) {

}

bool SingleGameStarter::exit(GameController* game) {
	console.info("Exiting the GameLoader State.");
	return true;
}

bool SingleGameStarter::enter(GameController* game) {
	console.info("Entering the GameLoader State.");
	return true;
}