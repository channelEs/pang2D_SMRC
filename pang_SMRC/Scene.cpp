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
	blocks = std::vector<DynamicBlocks*>();
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
	for (auto block : blocks)
		delete block;
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
		texs[0].loadFromFile("./assets/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

		texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.5f);
		texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		// Load textures
		texs[0].loadFromFile("./assets/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
		glm::vec2 texCoords[2] = { glm::vec2(1.f, 1.f), glm::vec2(1.5f, 1.5f) };

		texCoords[0] = glm::vec2(0.f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.f);
		texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		// Load textures
		texs[0].loadFromFile("./assets/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	}
	else if (lvlNum == 3)
	{
		level.levelPath = "levels/level03_MAP.txt";
		level.numBalloons = 2;
		level.sizeBalloon = 24;
		for (int i = 0; i < level.numBalloons; ++i) {
			level.posBalloons.push_back(glm::vec2(i * (48 / (level.numBalloons + 2)) + (48 / (level.numBalloons + 2)), 26 * 0.1));
		}
		initPlayerPosition = glm::ivec2(4, 21);

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

		texCoords[0] = glm::vec2(0.5f, 0.f); texCoords[1] = glm::vec2(1.f, 0.5f);
		texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
		// Load textures
		texs[0].loadFromFile("./assets/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

	if (lvlNum == 2)
	{
		generateBlock(glm::ivec2(17 * map->getTileSize(), 8 * map->getTileSize()), 32);
		generateBlock(glm::ivec2(21 * map->getTileSize(), 8 * map->getTileSize()), 32);
		generateBlock(glm::ivec2(25 * map->getTileSize(), 8 * map->getTileSize()), 32);
		generateBlock(glm::ivec2(29 * map->getTileSize(), 8 * map->getTileSize()), 32);
	}
	else if (lvlNum == 3)
	{
		generateBlock(glm::ivec2(13 * map->getTileSize(), 17 * map->getTileSize()), 24);
		generateBlock(glm::ivec2(31 * map->getTileSize(), 17 * map->getTileSize()), 24);
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH/3), float(SCREEN_HEIGHT/3), 0.f);
	lvlNumber = lvlNum;
	currentTime = 0.0f;
	playerHitTime = currentTime;
	powerActive = -1;
	freezedTime = false;
	playerOutOfInvinci = false;
	resetPowers();
}

int Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	
	for (int bang = 0; bang < bangs.size(); ++bang) {
		bangs[bang]->update(deltaTime);
		bool deleteBang = false;

		for (int block = 0; block < blocks.size(); ++block)
		{
			if (blocks[block]->isColisionRect(bangs[bang]->getPos(), bangs[bang]->getSize()) && !blocks[block]->isDestroyActive())
			{
				blocks[block]->setIsDestroy(true, currentTime);
				deleteBang = true;
			}
		}

		if (bangs[bang]->getType() == 0)
		{
			if (map->collisionMoveUp(bangs[bang]->getPos(), bangs[bang]->getSize()) != -1)
				deleteBang = true;
		}
		else if (bangs[bang]->getType() == 1)
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
						glm::ivec2 ballPos = balloonsVec[ball]->getPos();
						ballPos.x -= 1;
						generateBalloon(ballPos, sizeNew);
						ballPos.x += 2;
						generateBalloon(ballPos, sizeNew);
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
		if (!freezedTime)
		{
			if (balloonsVec[ball]->isColisionRectangle(player->getPosition(), glm::ivec2(32, 32)))
			{
				if (!playerHit && !playerInvinci)
				{
					playerHitTime = currentTime;
					player->setHit(true);
					playerHit = true;
				}
				else if (!playerHit && playerInvinci)
				{
					playerHit = true;
					playerOutOfInvinci = true;
				}
			}
			balloonsVec[ball]->update(deltaTime);
		}
	}
	bool reStart = false;
	if (playerHit && !playerOutOfInvinci) 
	{
		if (currentTime - playerHitTime > 750.f)
		{
			player->setHit(false);
			reStart = true;
			if ((int)(currentTime - playerHitTime) % 200 > 0 && (int)(currentTime - playerHitTime) % 200 < 100)
				player->setInvi();
			else
			{
				player->setNormal();
				player->update(deltaTime);
			}
		}
		if (currentTime - playerHitTime > 2000.f)
		{
			playerHit = false;
			player->setNormal();
		}

	}

	for (int power = 0; power < powers.size(); ++power)
	{
		bool deletePower = false;
		int powerId = powers[power]->getPowerID();
		powers[power]->update(deltaTime);
		if (powers[power]->isColisionRect(player->getPosition(), glm::ivec2(32, 32)))
		{
			setPower(powers[power]->getPowerID());
			deletePower = true;
		}
		else if ((currentTime - powers[power]->getIniTime()) > 2000.f)
		{
			deletePower = true;
		}
		if (deletePower)
		{
			delete powers[power];
			for (int i = power + 1; i < powers.size(); ++i) {
				powers[i - 1] = powers[i];
			}
			powers.pop_back();
		}
	}

	if (freezedTime && (currentTime - freezeIniTime) > 5000.f)
	{
		freezedTime = false;
	}

	if (playerInvinci)
	{
		if ((currentTime - invinciTime) > 5000.f || playerHit)
		{
			playerHit = false;
			player->setIsInvi(false);
			playerOutOfInvinci = true;
			invinciTime = currentTime;
		}
	}
	if (playerOutOfInvinci && (currentTime - invinciTime) > 1000.f)
	{
		player->setOutOfInvi();
		playerInvinci = false;
		playerOutOfInvinci = false;
	}

	for (int block = 0; block < blocks.size(); ++block)
	{
		blocks[block]->update(deltaTime);
		if (blocks[block]->isDestroyActive() && (currentTime - blocks[block]->getDestroyTime()) > 1000.f)
		{
			glm::ivec2 posBl = blocks[block]->getPosition();
			for (int i = 0; i < blocks[block]->getSize() / 8; ++i)
			{
				map->setTile(posBl.x / map->getTileSize() + i, posBl.y / map->getTileSize(), -1);
			}
			delete blocks[block];
			for (int i = block + 1; i < blocks.size(); ++i) {
				blocks[i - 1] = blocks[i];
			}
			blocks.pop_back();
		}
	}

	if (balloonsVec.size() == 0 && lvlNumber != 0 && !playerHit)
		return 2;

	if (reStart)
		return 1;

	if (balloonsVec.size() == 0 && lvlNumber == 3 && !playerHit)
		return 6;

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
	for (auto& bang : bangs)
		bang->render();
	for (auto& power : powers)
		power->render();
	for (auto& block : blocks)
		block->render();
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
	if (bangs.size() < numberBangs)
	{
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
}

void Scene::generateBlock(const glm::ivec2& pos, int lenght)
{
	blocks.push_back(new DynamicBlocks());
	int block = blocks.size() - 1;
	blocks[block]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, lenght);
	blocks[block]->setPosition(pos);
	blocks[block]->setTileMap(map);
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
	if (r < 50)
	{
		r = rand() % 7;
		powers.push_back(new PowerUps());
		int power = powers.size() - 1;
		powers[power]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, r, currentTime);
		powers[power]->setPosition(glm::ivec2(pos.x, pos.y + 5));
		powers[power]->setTileMap(map);
	}
}

void Scene::resetPowers()
{
	numberBangs = 1;
	typeBang = 0;
	powerActive = -1;
}

void Scene::setPower(int id)
{
	switch (id)
	{
	case DYNAMITE:
	{

		int ballsActive = balloonsVec.size();
		for (int ball = 0; ball < ballsActive; ++ball) {
			int sizeNew = balloonsVec[ball]->getSize() - 16;
			if (sizeNew == 0)
				sizeNew = 8;
			if (sizeNew >= 8)
			{
				generateBalloon(balloonsVec[ball]->getPos(), sizeNew);
				generateBalloon(balloonsVec[ball]->getPos(), sizeNew);
			}

			delete balloonsVec[ball];
		}
		for (int i = ballsActive; i < balloonsVec.size(); ++i) {
			balloonsVec[i - ballsActive] = balloonsVec[i];
		}
		for (int i = 0; i < ballsActive; ++i)
			balloonsVec.pop_back();
		powerActive = DYNAMITE;
	}
		break;

	case DOUBLE_WIRE:
		numberBangs = 2;
		powerActive = DOUBLE_WIRE;
		break;

	case FREEZE_TIME:
		freezeIniTime = currentTime;
		freezedTime = true;
		powerActive = FREEZE_TIME;
		break;

	case POWER_WIRE:
		powerActive = POWER_WIRE;
		break;

	case VULCAN_MISSILE:
		resetPowers();
		typeBang = 1;
		break;

	case INVINCIBILITY:
		playerInvinci = true;
		playerOutOfInvinci = false;
		invinciTime = currentTime;
		powerActive = INVINCIBILITY;
		player->setIsInvi(true);
		break;

	case SLOW_TIME:
		powerActive = SLOW_TIME;
		break;

	default:
		break;
	}
}


int Scene::getPowerActiveId()
{
	return powerActive;
}

int Scene::getPlayerLives()
{
	return player->getLives();
}

/*
POWER UPS:

T - Dynamite
Y - Doble Wire
U - Freeze Time
I - Vulcan Missile
O - Invincibility
*/