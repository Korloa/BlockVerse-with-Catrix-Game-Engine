#include "states/settingState/SettingState.h"

void SettingState::update(GameController* game, float deltaTime) {

}

void SettingState::render(GameController* game) {

}

bool SettingState::enter(GameController* game) {
	console.info("Entering the Settings State.");
	return true;
}

bool SettingState::exit(GameController* game) {
	console.info("Exiting the Settings State.");
	return true;
}