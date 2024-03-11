#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Balloon.h"

typedef Balloon* BalloonPtr;

struct level {
	string levelPath;
	int numBalloons;
	vector<glm::vec2> posBalloons;
};

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	BalloonPtr* balloons;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int numBalloons;

};


#endif // _SCENE_INCLUDE
