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
	//std::map<int, std::vector<TextInput>>pageTextInputs;	//尚未实现
	std::map<int, float>keyTimes; //去抖计时

public:
	virtual ~State() = default;
	virtual void procInput(Game* game, float deltaTime) {}
	virtual void update(Game* game, float deltaTime) = 0;
	virtual void render(Game* game) = 0;//等于0子类必须要实现
	virtual void enter(Game* game) {}	
	virtual void exit(Game* game){}		//{}子类不一定要实现

	bool isKeyPressed(Game* game, int key, float debounceTime = 0.2f) {
		float currentTime = glfwGetTime();
		if (glfwGetKey(game->gameWindow, key) == GLFW_PRESS &&
			(!keyTimes.count(key) || currentTime - keyTimes[key] > debounceTime)) {
			//这个的作用的是防止短按被识别为多次，这样的效果是长时间按会产生周期性的回复，因为 32行一直在刷新时间
			keyTimes[key] = currentTime;
			return true;
		}
		return false;
	}
};    