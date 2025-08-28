#pragma once
#include "states/basic/State.h"

class SingleGameStarter : public State {
public:
	bool enter(GameController* game) override;
	bool exit(GameController* game) override;

	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;


};