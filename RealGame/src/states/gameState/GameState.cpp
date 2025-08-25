#include "states/gameState/GameState.h"

bool GameState::enter(GameController* game){
	console.info("Entering the Gaming State.");

	GLFWwindow* gameWindow = glfwCreateWindow(1024, 768, "BlockVerse v1.5.0", NULL, NULL);

	if (!gameWindow) {
		glfwTerminate();
		console.error("Fail to create the GLFW game window.");
		return false;
	}

	glfwMakeContextCurrent(gameWindow);

	glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//即便某些扩展是实验性的，也要加载它们,必须在glewInit()调用之前使用它
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		console.error("Fail to initialize the GLEW.");
		return false;
	}

	//开启深度测试后，每个像素点除了颜色信息外，还具有一个深度值(z)
	// ，在绘制像素前会首先比较深度，近距离才会写入帧缓存
	glEnable(GL_DEPTH_TEST);
	//开启颜色混合，让新颜色与缓冲区内的颜色进行混合，以实现透明效果(UI)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void GameState::update(GameController* game, float deltaTime) {
	exit(game);
}

void GameState::render(GameController* game) {

}

bool GameState::exit(GameController* game) {
	console.info("Exiting the Gaming State.");
	return true;
}
