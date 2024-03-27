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
	bangs = std::vector<Bang*>();
	powers = std::vector<PowerUps*>();
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
	for (auto power : powers) {
		delete power;
	}
	balloonsVec.clear();
	bangs.clear();
	powers.clear();
}


void Scene::init(int lvlNum)
{
	initShaders();
	playerHit = false;

	level level;
	glm::ivec2 initPlayerPosition;

	// SET INSTRUC
	if (lvlNum == 0)
	{
		level.levelPath = "levels/instructions_MAP.txt";
		level.numBalloons = 0;
		initPlayerPosition = glm::ivec2(4, 12);

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

		texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.5f);
		texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		// Load textures
		texs[0].loadFromFile("./assets/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	}

	// SET LEVEL
	if (lvlNum == 1)
	{
		level.levelPath = "levels/level01_MAP.txt";
		level.numBalloons = 1;
		level.sizeBalloon = 48;
		for (int i = 0; i < level.numBalloons; ++i) {
			level.posBalloons.push_back(glm::vec2(i * (48 / (level.numBalloons + 2)) + (48 / (level.numBalloons + 2)), 26 * 0.1));
		}
		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

		texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
		texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		// Load textures
		texs[0].loadFromFile("./assets/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
		initPlayerPosition = glm::ivec2(4, 21);
	}
	else if (lvlNum == 2)
	{
		level.levelPath = "levels/level02_MAP.txt";
		level.numBalloons = 2;
		level.sizeBalloon = 24;
		for (int i = 0; i < level.numBalloons; ++i) {
			level.posBalloons.push_back(glm::vec2(i * (48 / (level.numBalloons + 2)) + (48 / (level.numBalloons + 2)), 26 * 0.1));
		}
		initPlayerPosition = glm::ivec2(4, 21);

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

		texCoords[0] = glm::vec2(0.5f, 0.5f); texCoords[1] = glm::vec2(1.f, 1.f);
		texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		// Load textures
		texs[0].loadFromFile("./assets/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	}

	map = TileMap::createTileMap(level.levelPath, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	int numBalloons = level.numBalloons;
	
	// INIT PLAYER
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(initPlayerPosition.x * map->getTileSize(), initPlayerPosition.y * map->getTileSize()));
	player->setTileMap(map);

	// INIT BALLOONS
	for (int ball = 0; ball < numBalloons; ++ball)
	{
		generateBalloon(glm::ivec2(level.posBalloons[ball].x * map->getTileSize(), level.posBalloons[ball].y * map->getTileSize()), level.sizeBalloon);
	}

	typeBang = 0;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH/3), float(SCREEN_HEIGHT/3), 0.f);
	currentTime = 0.0f;
	initTime = currentTime;
	powerActive = -1;
}

int Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	
	for (int bang = 0; bang < bangs.size(); ++bang) {
		bangs[bang]->update(deltaTime);
		bool deleteBang = false;
		if (bangs[bang]->getType() == 0)
		{
			if (map->collisionMoveUp(bangs[bang]->getPos(), bangs[bang]->getSize()) != -1)
				deleteBang = true;
		}
		else if (bangs[bang]->getType() == 2)
		{
			if (map->collisionMoveUp(bangs[bang]->getPos(), glm::ivec2(8, 8)) != -1) {
				deleteBang = true;
			}
		}

		if (!deleteBang)
		{
			int maxActualBalloons = balloonsVec.size();
			// colision with Bang
			for (int ball = 0; ball < balloonsVec.size(); ++ball) {
				if (balloonsVec[ball]->isColisionRectangle(bangs[bang]->getPos(), bangs[bang]->getSize()))
				{
					int sizeNew = balloonsVec[ball]->getSize() - 16;
					if (sizeNew == 0)
						sizeNew = 8;
					if (sizeNew >= 8)
					{
						generateBalloon(balloonsVec[ball]->getPos(), sizeNew);
						generateBalloon(balloonsVec[ball]->getPos(), sizeNew);
					}
					generatePowerUp(balloonsVec[ball]->getPos());
					
					delete balloonsVec[ball];
					for (int i = ball + 1; i < balloonsVec.size(); ++i) {
						balloonsVec[i - 1] = balloonsVec[i];
					}
					balloonsVec.pop_back();
					deleteBang = true;
					break;
				}
			}	
		}

		if (deleteBang)
		{
			delete bangs[bang];
			for (int i = bang + 1; i < bangs.size(); ++i) {
				bangs[i - 1] = bangs[i];
			}
			bangs.pop_back();
		}
	}

	for (int ball = 0; ball < balloonsVec.size(); ++ball) {
		if (balloonsVec[ball]->isColisionRectangle(player->getPosition(), glm::ivec2(32, 32)))
		{
			if (!playerHit)
			{
				playerHit = true;
				initTime = currentTime;
				player->setHit(true);
			}
		}
		balloonsVec[ball]->update(deltaTime);
	}
	if (playerHit) 
	{
		if (currentTime - initTime > 750.f)
		{
			player->setHit(false);
			if ((int)(currentTime - initTime) % 200 > 0 && (int)(currentTime - initTime) % 200 < 100 )
				player->setInvi();
		}
		if (currentTime - initTime > 2000.f)
		{
			playerHit = false;

		}

	}

	for (auto& power : powers)
	{
		power->update(deltaTime);
	}

	return -1;
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
	modelview = glm::translate(modelview, glm::vec3(0.f, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]);
	map->render();
	player->render();
	for (int ball = 0; ball < balloonsVec.size(); ++ball)
		balloonsVec[ball]->render();
	for (auto& bang : bangs) {
		bang->render();
	}
	for (auto& power : powers)
	{
		power->render();
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
	newBang->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, typeBang);
	newBang->setTileMap(map);
	if (typeBang == 0)
	{
		newBang->setPosition(glm::ivec2(player->getPosition().x + 16, player->getPosition().y + 32 - 189));
	}
	else if (typeBang == 1)
	{
		newBang->setPosition(player->getPosition());
	}
	bangs.push_back(newBang);
}

void Scene::generateBalloon(const glm::ivec2 &pos, int size)
{
	balloonsVec.push_back(new Balloon());
	int ball = balloonsVec.size() - 1;
	balloonsVec[ball]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, (ball % 2 == 1), glm::ivec2(size, size));
	balloonsVec[ball]->setPosition(pos);
	balloonsVec[ball]->setTileMap(map);
}

void Scene::generatePowerUp(const glm::ivec2& pos)
{
	int r = rand() % 100;
	cout << "RANDOM NUMBER: " << r << endl;
	if (r < 10)
	{
		r = rand() % 7;
		cout << "RANDOM NUMBER_02: " << r << endl;
		powers.push_back(new PowerUps());
		int power = powers.size() - 1;
		powers[power]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, r);
		powers[power]->setPosition(pos);
		powers[power]->setTileMap(map);
	}
}

/*
POWER UPS:

T - Dynamite
Y - Doble Wire
U - Freeze Time
I - Power Wire
O - Vulcan Missile
P - Invincibility
L - Slow Time
*/