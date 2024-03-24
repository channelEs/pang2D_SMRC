#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bang.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 2
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MOVE_STEP 2

#define TEXT_BANG_01 1.f / 79.f

void Bang::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int typeBang)
{
	type = typeBang;
	
	if (type == 0)
	{
		spritesheet.loadFromFile("assets/fire_01.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sizeBang = glm::ivec2(9, 34);
		sprite = Sprite::createSprite(glm::ivec2(9, 189), glm::vec2(TEXT_BANG_01, 1.f), &spritesheet, &shaderProgram);
	}
	else if (type == 1)
	{
		spritesheet.loadFromFile("assets/bang.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(18, 8), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	}
	reloadTimer = 0;

	if (type == 0)
	{
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(0, 89);
		
		for (int i = 0; i < 79; ++i)
		{
			sprite->addKeyframe(0, glm::vec2(i * TEXT_BANG_01, 0.f));
		}
		sprite->changeAnimation(0);
	}

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBangStatic.x), float(tileMapDispl.y + posBangStatic.y)));
}

void Bang::update(int deltaTime)
{
    sprite->update(deltaTime);
	if (type == 0)
	{
		sizeBang.y += 2* 156.f / 79.f; // 190 steps / 60 framesXsec
		posBangDync.y = posBangStatic.y + (189 - sizeBang.y);
	}
	else if (type == 1)
	{
		posBangStatic.y -= 2;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBangStatic.x), float(tileMapDispl.y + posBangStatic.y)));
	}
}



void Bang::render()
{
	sprite->render();

}

void Bang::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Bang::setPosition(const glm::vec2& pos)
{
	posBangStatic = pos;
	posBangDync = glm::ivec2(posBangStatic.x, posBangStatic.y + (189 - sizeBang.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBangStatic.x), float(tileMapDispl.y + posBangStatic.y)));
}

glm::ivec2 Bang::getPos() {
	return posBangDync;
}

glm::ivec2 Bang::getSize()
{
	return sizeBang;
}

int Bang::getType()
{
	return type;
}