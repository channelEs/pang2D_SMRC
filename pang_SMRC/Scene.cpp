#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
	balloonsVec = std::vector<Balloon*>();

}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	for (auto balloon : balloonsVec) {
		delete balloon;
	}
	for (auto bang : bangs) {
		delete bang;
	}
	balloonsVec.clear();
	bangs.clear();
}


void Scene::init(int lvlNum)
{
	initShaders();

	level level;
	// SET LEVEL
	if (lvlNum == 0)
	{
		level.levelPath = "levels/level01_MAP.txt";
		level.numBalloons = 4;
		for (int i = 0; i < level.numBalloons; ++i) {
			level.posBalloons.push_back(glm::vec2(i * (48 / (level.numBalloons + 2)) + (48 / (level.numBalloons + 2)), 26 * 0.1));
		}
	}
	else if (lvlNum == 1)
	{
		level.levelPath = "levels/level02_MAP.txt";
		level.numBalloons = 2;
		for (int i = 0; i < level.numBalloons; ++i) {
			level.posBalloons.push_back(glm::vec2(i * (SCREEN_X / level.numBalloons) + (SCREEN_X / level.numBalloons), 1));
		}
	}


	map = TileMap::createTileMap(level.levelPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	numBalloons = level.numBalloons;
	
	// INIT PLAYER
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	// INIT SHOTS
	bang = new Bang();
	bang->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	bang->setTileMap(map);
	bang->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	
	// INIT BALLOONS
	for (int ball = 0; ball < numBalloons; ++ball)
	{
		glm::ivec2 size;
		if (ball == 0) size = glm::ivec2(48, 48);
		else size = glm::ivec2(24, 24);
		balloonsVec.push_back(new Balloon());
		balloonsVec[ball]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, (ball % 2 == 1), size);
		balloonsVec[ball]->setPosition(glm::vec2(level.posBalloons[ball].x * map->getTileSize(), level.posBalloons[ball].y * map->getTileSize()));
		balloonsVec[ball]->setTileMap(map);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH/4), float(SCREEN_HEIGHT/4), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);

	for (int ball = 0; ball < numBalloons; ++ball) {
		balloonsVec[ball]->update(deltaTime);
	}
	for (int bang = 0; bang < bangs.size(); ++bang) {
		bangs[bang]->update(deltaTime);
		if (bangs[bang]->getPosY() <= 2) {
			delete bangs[bang];
			for (int i = bang + 1; i < bangs.size(); ++i) {
				bangs[i - 1] = bangs[i];
			}
			bangs.pop_back();
		}
	}


	
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
		balloonsVec[ball]->render();
	for (auto& bang : bangs) {
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

void Scene::generateBang() {
	Bang* newBang = new Bang();
	newBang->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	newBang->setTileMap(map);
	newBang->setPosition(player->getPosition());
	bangs.push_back(newBang);
}



