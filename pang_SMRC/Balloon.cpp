#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Balloon.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 2
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MOVE_STEP 2

void Balloon::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool startLeft, glm::ivec2& size)
{
	spritesheet.loadFromFile("assets/balloonsQUAD.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sizeBalloon = size;
	bJumping = false;
	movingLeft = startLeft;
	
	sprite = Sprite::createSprite(sizeBalloon, glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBalloon.x), float(tileMapDispl.y + posBalloon.y)));
}

void Balloon::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posBalloon.y = startY;
		}
		else
		{
			posBalloon.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = true;
		}
	}
	else
	{
		posBalloon.y += FALL_STEP;
		if (map->collisionMoveDown(posBalloon, sizeBalloon, &posBalloon.y))
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posBalloon.y;
		}
	}

	if (movingLeft)
	{
		posBalloon.x -= MOVE_STEP;
		if (map->collisionMoveLeft(posBalloon, sizeBalloon))
		{
			posBalloon.x += MOVE_STEP;
			movingLeft = false;
		}
	}
	else 
	{
		posBalloon.x += MOVE_STEP;
		if (map->collisionMoveRight(posBalloon, sizeBalloon))
		{
			posBalloon.x -= MOVE_STEP;
			movingLeft = true;
		}
	}


	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBalloon.x), float(tileMapDispl.y + posBalloon.y)));
}

void Balloon::render()
{
	sprite->render();
}

void Balloon::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Balloon::setPosition(const glm::vec2& pos)
{
	posBalloon = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBalloon.x), float(tileMapDispl.y + posBalloon.y)));
}