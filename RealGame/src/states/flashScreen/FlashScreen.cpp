#include "states/flashScreen/FlashScreen.h"
#include "states/slide/slideState.h"
#include "core/GameController.h"

void FlashScreen::update(GameController* game, float deltaTime) {
	startTime += deltaTime;
	if (startTime >= 3.5f) {
		game->popState();
		game->pushState(new SlideState());
	}

	if (startTime <= 1.0f) {
		alpha = ease(startTime / 1.0f);
	}
	else if (startTime <= 2.5f) {
		alpha = 1.0f;
	}
	else {
		alpha = 1 - ease((startTime-2.5f) / 1.0f);
	}
}

void FlashScreen::render(GameController* game) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// 使用 ImageRenderer 渲染 Logo
	game->imageRender->renderImage(
		"logo/Catrix_Engine.png",     // 图片路径
		0, 0,                    // 左下角 x, y
		800, 250,                    // 宽高
		1.0f                         // 透明度
	);
	glfwSetWindowOpacity(flashWindow,alpha);
}

bool FlashScreen::enter(GameController* game) {
	console.info("Entering the FlashScreen State.");

	alpha = 0;
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	flashWindow = glfwCreateWindow(800, 250, "FlashScreen", NULL, NULL);
	glfwSetWindowPos(flashWindow, 960 - 400, 540 - 125);
	glfwSetWindowOpacity(flashWindow, 0);
	glfwShowWindow(flashWindow);

	glfwSetWindowAttrib(flashWindow, GLFW_DECORATED, GLFW_FALSE);
	glfwMakeContextCurrent(flashWindow);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		console.info("Failed to initialize GLEW");
		return false;
	}
	if (!game->imageRender->initialize()) {
		return false;
	}
	startTime = 0.0f;
	game->setWindowInstance(flashWindow);
	return true;
}

bool FlashScreen::exit(GameController* game) {
	console.info("Exiting the FlashScreen State.");
	glfwDestroyWindow(flashWindow);
	return true;
}
