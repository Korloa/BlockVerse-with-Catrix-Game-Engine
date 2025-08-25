#pragma once
#include "states/basic/State.h"

class MenuState:public State {
public:
	void update(GameController* game, float deltaTime) override;
	void render(GameController* game) override;//����0�������Ҫʵ��

	bool enter(GameController* game) override;
	bool exit(GameController* game) override;
};