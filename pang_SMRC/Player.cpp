#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("assets/pang_BlueAssets.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.75f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 16);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 16);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getKey(GLFW_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)) == ID_BLOCKS_BRICK_V)
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getKey(GLFW_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)) == ID_BLOCKS_BRICK_V)
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	int id_collision_down = map->collisionMoveDown(posPlayer, glm::ivec2(36, 36), &posPlayer.y);

	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		int id_collision_up = map->collisionMoveUp(posPlayer, glm::ivec2(32, 32));
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else if (id_collision_up == ID_BLOCKS_BRICK_H_01 || id_collision_up == ID_BLOCKS_BRICK_H_02)
		{
			bJumping = false;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !(id_collision_down == -1);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;

		if(id_collision_down != -1)
		{
			if(Game::instance().getKey(GLFW_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition() const {
	return posPlayer; 
}




