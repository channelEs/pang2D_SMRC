#include "PowerUps.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Game.h"

enum ObjectIcon
{
	DYNAMITE, DOUBLE_WIRE, FREEZE_TIME, POWER_WIRE, VULCAN_MISSILE, INVINCIBILITY, SLOW_TIME
};

void PowerUps::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int powerId)
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
}

void PowerUps::update(int deltaTime)
{
	sprite->update(deltaTime);
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