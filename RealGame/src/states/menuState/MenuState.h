#pragma once
#include "states/basic/State.h"

class MenuState:public State {
public:
	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;//等于0子类必须要实现

	bool enter(GameController* game) override;
	bool exit(GameController* game) override;

	float buttonWidth = 700.0f;
	float buttonHeight = 70.0f;

	float buttonMargin = 110.0f;
	float buttonTop = 480.0f;
};