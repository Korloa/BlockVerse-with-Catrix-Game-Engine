#include "states/slide/SlideState.h"
#include "states/menuState/MenuState.h"

//enter和构造函数还是有所区别的
SlideState::SlideState() :timer(0.0f),alpha(0.0f),transition(0.8f),lastTime(2.5f),imageIndex(0){
	imagePath = {
		"flashscreen/slide_engine",
		"flashscreen/slide_game"
	};
};

void SlideState::update(GameController* game, float deltaTime) {
	timer += deltaTime;
	if (timer <= transition) {
		alpha = timer / transition;
	}
	else if (timer > transition && timer <= (lastTime - transition)) {
		alpha = 1.0f;
	}
	else if (timer > (lastTime - transition) && timer <= lastTime) {
		alpha = (transition-lastTime+timer)/transition;
	}//else表示一个幻灯片播放完毕了
	else {
		imageIndex = (imageIndex + 1) % imagePath.size();
		//一轮循环已经结束
		if (imageIndex == 0) {
			game->replaceState(new MenuState());
		}

		alpha = 0.0f;
		timer = 0.0f;
	}
}

void SlideState::render(GameController* game) {

}

bool SlideState::enter(GameController* game) {
	console.info("Entering the SlideState State.");

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	mainWindow = glfwCreateWindow(videoMode->width, videoMode->height, "BlockVerse", monitor, NULL);
	game->setWindowInstance(mainWindow);
	return true;
}

bool SlideState::exit(GameController* game) {
	console.info("Exiting the SlideState State.");
	return true;
}