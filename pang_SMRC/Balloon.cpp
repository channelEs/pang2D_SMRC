#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Balloon.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 2
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
	int newPosY, newPosX;

	int colision;
	
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;

		if (jumpAngle == 180)
		{
			bJumping = false;
			newPosY = startY;
			// posBalloon.y = startY;
		}
		else
		{
			newPosY = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			colision = map->collisionCircularZones(glm::ivec2(posBalloon.x, newPosY), sizeBalloon);
			if (colision == 0 || colision == 7 || colision == 1)
			{ 
				bJumping = false;
			}

			// posBalloon.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = true;
		}
	}
	else
	{
		newPosY = posBalloon.y + FALL_STEP;
		colision = map->collisionCircularZones(glm::ivec2(posBalloon.x, newPosY), sizeBalloon);
		// posBalloon.y += FALL_STEP;
		if (colision == 5 || colision == 4 || colision == 3)
		{
			bJumping = true;
			jumpAngle = 0;
			startY = posBalloon.y;
		}
	}

	if (movingLeft)
	{
		newPosX = posBalloon.x - MOVE_STEP;
		colision = map->collisionCircularZones(glm::ivec2(newPosX, newPosY), sizeBalloon);
		if (colision == 7 || colision == 6 || colision == 5)
		{
			newPosX += MOVE_STEP;
			movingLeft = false;
		}
	}
	else 
	{
		newPosX = posBalloon.x + MOVE_STEP;
		colision = map->collisionCircularZones(glm::ivec2(newPosX, newPosY), sizeBalloon);
		if (colision == 1 || colision == 2 || colision == 3)
		{
			newPosX -= MOVE_STEP;
			movingLeft = true;
		}
	}
	
	posBalloon.y = newPosY;
	posBalloon.x = newPosX;

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