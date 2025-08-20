#include<map>
#include<vector>

#include "components/Button.h"
#include "components/TextInput.h"
#include "core/Game.h"

#include<GLFW/glfw3.h>
class Game;
class TextRender;
class RectRender;

class State {
protected:
	std::vector<int> pageStack;
	std::map<int, std::vector<Button>>pageButtons;
	//std::map<int, std::vector<TextInput>>pageTextInputs;	//��δʵ��
	std::map<int, float>keyTimes; //ȥ����ʱ

public:
	virtual ~State() = default;
	virtual void procInput(Game* game, float deltaTime) {}
	virtual void update(Game* game, float deltaTime) = 0;
	virtual void render(Game* game) = 0;//����0�������Ҫʵ��
	virtual void enter(Game* game) {}	
	virtual void exit(Game* game){}		//{}���಻һ��Ҫʵ��

	bool isKeyPressed(Game* game, int key, float debounceTime = 0.2f) {
		float currentTime = glfwGetTime();
		if (glfwGetKey(game->gameWindow, key) == GLFW_PRESS &&
			(!keyTimes.count(key) || currentTime - keyTimes[key] > debounceTime)) {
			//��������õ��Ƿ�ֹ�̰���ʶ��Ϊ��Σ�������Ч���ǳ�ʱ�䰴����������ԵĻظ�����Ϊ 32��һֱ��ˢ��ʱ��
			keyTimes[key] = currentTime;
			return true;
		}
		return false;
	}
};    