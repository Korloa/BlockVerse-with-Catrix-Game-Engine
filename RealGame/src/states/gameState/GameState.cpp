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


	//����ĳЩ��չ��ʵ���Եģ�ҲҪ��������,������glewInit()����֮ǰʹ����
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		console.error("Fail to initialize the GLEW.");
		return false;
	}

	//������Ȳ��Ժ�ÿ�����ص������ɫ��Ϣ�⣬������һ�����ֵ(z)
	// ���ڻ�������ǰ�����ȱȽ���ȣ�������Ż�д��֡����
	glEnable(GL_DEPTH_TEST);
	//������ɫ��ϣ�������ɫ�뻺�����ڵ���ɫ���л�ϣ���ʵ��͸��Ч��(UI)
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
