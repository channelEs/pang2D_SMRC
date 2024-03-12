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
	map02 = NULL;
	map03 = NULL;
	map04 = NULL;
	player = NULL;
	balloonsVec = std::vector<Balloon*>();
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if (map02 != NULL)
		delete map02;
	if (map03 != NULL)
		delete map03;
	if (map04 != NULL)
		delete map04;
	if(player != NULL)
		delete player;
	for (auto balloon : balloonsVec) {
		delete balloon;
	}
	balloonsVec.clear();
}


void Scene::init()
{
	initShaders();
	level level01;
	level01.levelPath = "levels/map_blocks.txt";
	level01.numBalloons = 2;
	for (int i = 0; i < level01.numBalloons; ++i) {
		level01.posBalloons.push_back(glm::vec2(i * (SCREEN_X / level01.numBalloons) + (SCREEN_X / level01.numBalloons), 15));
	}
	map = TileMap::createTileMap(level01.levelPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map02 = TileMap::createTileMap("levels/level02_floors.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map03 = TileMap::createTileMap("levels/level02_stairs.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);	
	map04 = TileMap::createTileMap("levels/level02_ghosts.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	numBalloons = level01.numBalloons;
	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	bang = new Bang();
	bang->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bang->setTileMap(map);
	bang->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	for (int ball = 0; ball < numBalloons; ++ball)
	{
		glm::ivec2 size;
		if (ball == 0) size = glm::ivec2(48, 48);
		else size = glm::ivec2(24, 24);
		balloonsVec.push_back(new Balloon());
		balloonsVec[ball]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, (ball % 2 == 1), size);
		balloonsVec[ball]->setPosition(glm::vec2(level01.posBalloons[ball].x * map->getTileSize(), level01.posBalloons[ball].y * map->getTileSize()));
		balloonsVec[ball]->setTileMap(map);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH/2), float(SCREEN_HEIGHT/2), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for (int ball = 0; ball < numBalloons; ++ball)
		balloonsVec[ball]->update(deltaTime);
	bang->update(deltaTime);
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
	map02->render();
	map03->render();
	map04->render();
	player->render();
	for (int ball = 0; ball < numBalloons; ++ball)
		balloonsVec[ball]->render();
	if (bang->getClicked()) {
		bang->render();
	}
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



