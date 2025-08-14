class Game;

class State {
public:
	virtual ~State() = default;
	virtual void procInput(Game* game, float deltaTime) {}
	virtual void update(Game* game, float deltaTime) = 0;
	virtual void render(Game* game) = 0;//等于0子类必须要实现
	virtual void enter(Game* game) {}	
	virtual void exit(Game* game){}		//{}子类不一定要实现
	virtual 
};    