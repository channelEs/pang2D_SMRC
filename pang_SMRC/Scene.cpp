#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
	balloons = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (balloons != NULL)	
		delete[] balloons;
}


void Scene::init()
{
	initShaders();
	level level01;
	level01.levelPath = "levels/level01_test.txt";
	level01.numBalloons = 2;
	// level01.posBalloons.push_back(glm::vec2(4, 15));
	// level01.posBalloons.push_back(glm::vec2(8, 15));
	for (int i = 0; i < level01.numBalloons; ++i) {
		level01.posBalloons.push_back(glm::vec2(i * (SCREEN_X / level01.numBalloons) + (SCREEN_X / level01.numBalloons), 15));
	}


	map = TileMap::createTileMap(level01.levelPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	numBalloons = level01.numBalloons;
	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	balloons = new BalloonPtr[numBalloons];
	for (int ball = 0; ball < numBalloons; ++ball)
	{
		balloons[ball] = new Balloon();
		balloons[ball]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, (ball%2 == 1));
		balloons[ball]->setPosition(glm::vec2(level01.posBalloons[ball].x * map->getTileSize(), level01.posBalloons[ball].y * map->getTileSize()));
		balloons[ball]->setTileMap(map);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH/2), float(SCREEN_HEIGHT/2), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (int ball = 0; ball < numBalloons; ++ball)
		balloons[ball]->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	for (int ball = 0; ball < numBalloons; ++ball)
		balloons[ball]->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



