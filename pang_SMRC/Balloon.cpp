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
	
	setNextPos();

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

bool Balloon::isColisionRectangle(const glm::ivec2& pos, const glm::ivec2& rect)
{
	glm::ivec2 circCenter = glm::ivec2(posBalloon.x + sizeBalloon.x / 2.0f, posBalloon.y + sizeBalloon.x / 2.0f);

	// Calculate the closest point on the rectangle to the circle
	float closestX = glm::clamp(float(circCenter.x), float(pos.x), float(pos.x + rect.x));
	float closestY = glm::clamp(float(circCenter.y), float(pos.y), float(pos.y + rect.y));

	// Calculate the distance between the circle's center and the closest point on the rectangle
	float distance = glm::length(glm::vec2(closestX - circCenter.x, closestY - circCenter.y));

	// If the distance is less than or equal to the circle's radius, they intersect
	return distance <= float(sizeBalloon.x / 2);
}

void Balloon::setNextPos()
{
	int newPosY, newPosX;
	int newPosYit = posBalloon.y, newPosXit = posBalloon.x;

	int newJumpAngle;
	int newJumpAngleIt = jumpAngle;

	int colision;

	/*
	   /|
	  / |
	 /  | => JUMP_ANGLE_STEP
	/   |
	-----
		=> MOVE_STEP
	*/
	int incIt;
	for (incIt = 1; incIt <= 2; ++incIt)
	{
		newPosY = newPosYit;
		newPosX = newPosXit;
		newJumpAngle = newJumpAngleIt;
		if (bJumping)
		{
			newJumpAngleIt = newJumpAngle + JUMP_ANGLE_STEP * incIt / 2;

			if (newJumpAngleIt == 180)
			{
				bJumping = false;
				newPosY = startY;
				break;
			}
			else
			{
				// Y -> JUMPING

				newPosYit = int(startY - 96 * sin(3.14159f * newJumpAngleIt / 180.f));
				if (newJumpAngleIt >= 90)
				{
					// Y -> JUMPING -> GOING DOWN

					if (movingLeft)
					{
						// X -> MOVING LEFT

						newPosXit = posBalloon.x - MOVE_STEP * incIt / 2;
						colision = map->collisionCircularZones(glm::ivec2(newPosXit, newPosYit), sizeBalloon);
						if (colision == 5)
						{
							newJumpAngle -= 90;
							bJumping = true;
							movingLeft = false;
							break;
						}
						else if (colision == 6)
						{
							movingLeft = false;
							break;
						}
					}
					else
					{
						// X -> MOVING RIGHT

						newPosXit = posBalloon.x + MOVE_STEP * incIt / 2;
						colision = map->collisionCircularZones(glm::ivec2(newPosXit, newPosYit), sizeBalloon);
						if (colision == 3)
						{
							newJumpAngle -= 90;
							bJumping = true;
							movingLeft = true;
							break;
						}
						else if (colision == 2)
						{
							movingLeft = true;
							break;
						}
					}
					if (colision == 4) {
						newJumpAngle -= 90;
						bJumping = true;
						break;
					}

				}
				else
				{
					// Y -> JUMPING -> GOING UP

					if (movingLeft)
					{
						// X -> MOVING LEFT

						newPosXit = posBalloon.x - MOVE_STEP * incIt / 2;
						colision = map->collisionCircularZones(glm::ivec2(newPosXit, newPosYit), sizeBalloon);
						if (colision == 7)
						{
							newJumpAngle += 90;
							movingLeft = false;
							bJumping = false;
							break;
						}
						else if (colision == 6)
						{
							movingLeft = false;
							break;
						}
					}
					else
					{
						// X -> MOVING RIGHT

						newPosXit = posBalloon.x + MOVE_STEP * incIt / 2;
						colision = map->collisionCircularZones(glm::ivec2(newPosXit, newPosYit), sizeBalloon);
						if (colision == 1)
						{
							newJumpAngle += 90;
							bJumping = false;
							movingLeft = true;
							break;
						}
						else if (colision == 2)
						{
							movingLeft = true;
							break;
						}
					}
					if (colision == 0) {
						newJumpAngle += 90;
						bJumping = false;
						break;
					}
				}
			}
		}
		else
		{
			// Y -> FALLING
			newPosYit = posBalloon.y + FALL_STEP * incIt / 2;

			if (movingLeft)
			{
				// X -> MOVING LEFT
				newPosXit = posBalloon.x - MOVE_STEP * incIt / 2;
				colision = map->collisionCircularZones(glm::ivec2(newPosXit, newPosYit), sizeBalloon);
				if (colision == 5)
				{
					bJumping = true;
					newJumpAngle = 0;
					startY = newPosY;
					movingLeft = false;
					break;
				}
				else if (colision == 6)
				{
					movingLeft = false;
					break;
				}
			}
			else
			{
				// X -> MOVING RIGHT

				newPosXit = posBalloon.x + MOVE_STEP * incIt / 2;
				colision = map->collisionCircularZones(glm::ivec2(newPosXit, newPosYit), sizeBalloon);
				if (colision == 3)
				{
					bJumping = true;
					newJumpAngle = 0;
					startY = newPosY;
					movingLeft = true;
					break;
				}
				else if (colision == 2)
				{
					movingLeft = true;
					break;
				}
			}
			if (colision == 4) {
				bJumping = true;
				newJumpAngle = 0;
				startY = newPosY;
				break;
			}
		}
	}
	posBalloon.y = newPosY;
	posBalloon.x = newPosX;
	jumpAngle = newJumpAngle;
}