class Game;

class State {
public:
	virtual ~State() = default;
	virtual void procInput(Game* game, float deltaTime) {}
	virtual void update(Game* game, float deltaTime) = 0;
	virtual void render(Game* game) = 0;//����0�������Ҫʵ��
	virtual void enter(Game* game) {}	
	virtual void exit(Game* game){}		//{}���಻һ��Ҫʵ��
	virtual 
};    