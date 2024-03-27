#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Balloon.h"
#include "Bang.h"
#include "TexturedQuad.h"

struct level {
	string levelPath;
	int numBalloons;
	int sizeBalloon;
	vector<glm::vec2> posBalloons;
};

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int lvlNum);
	int update(int deltaTime);
	void render();
	void generateBang();

private:
	void initShaders();
	void generateBalloon(const glm::ivec2 &pos, int size);

private:
	TileMap *map;							// tileMap active
	Player *player;							// instance of the player
	std::vector<Balloon*> balloonsVec;		// array of instances for every Balloon active
	std::vector<Bang*> bangs;

	Texture texs[1];
	TexturedQuad* texQuad[1];
	
	ShaderProgram texProgram;
	float currentTime, initTime;
	glm::mat4 projection;
	int typeBang;
	bool playerHit;
};


#endif // _SCENE_INCLUDE

