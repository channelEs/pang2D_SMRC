#include "PowerUps.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"

void PowerUps::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int powerId, float currentTime)
{
	spritesheet.loadFromFile("assets/iconsPowerUps.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(INVINCIBILITY, 8);
	sprite->addKeyframe(INVINCIBILITY, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(FREEZE_TIME, 8);
	sprite->addKeyframe(FREEZE_TIME, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(DYNAMITE, 8);
	sprite->addKeyframe(DYNAMITE, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(SLOW_TIME, 8);
	sprite->addKeyframe(SLOW_TIME, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(POWER_WIRE, 8);
	sprite->addKeyframe(POWER_WIRE, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(DOUBLE_WIRE, 8);
	sprite->addKeyframe(DOUBLE_WIRE, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(VULCAN_MISSILE, 8);
	sprite->addKeyframe(VULCAN_MISSILE, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(powerId);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
	iniTime = currentTime;
}

void PowerUps::update(int deltaTime)
{
	sprite->update(deltaTime);
	posObj.y += 1;
	int id_collision_down = map->collisionMoveDown(posObj, glm::ivec2(16, 16), &posObj.y, true);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

void PowerUps::render()
{
	sprite->render();
}

void PowerUps::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void PowerUps::setPosition(const glm::vec2& pos)
{
	posObj = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posObj.x), float(tileMapDispl.y + posObj.y)));
}

bool PowerUps::isColisionRect(const glm::ivec2& pos, const glm::ivec2& size)
{
	int r1x = posObj.x, r1y = posObj.y, r1w = 16, r1h = 16;
	int r2x = pos.x, r2y = pos.y, r2w = size.x, r2h = size.y;
	if (r1x + r1w >= r2x && r1x <= r2x + r2w &&
		r1y + r1h >= r2y && r1y <= r2y + r2h) return true;
	return false;
}

int PowerUps::getPowerID()
{
	return sprite->animation();
}

float PowerUps::getIniTime()
{
	return iniTime;
}