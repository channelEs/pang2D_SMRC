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
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, HIT, EMPTY, STAIRS, STAIRS_STAND
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bStairs = false;

	powerSheet.loadFromFile("assets/inviniPlayer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	powerSprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &powerSheet, &shaderProgram);
	powerSprite->setNumberAnimations(2);
	powerSprite->setAnimationSpeed(0, 16);
	powerSprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	powerSprite->addKeyframe(0, glm::vec2(0.5f, 0.f));

	powerSprite->setAnimationSpeed(1, 20);
	powerSprite->addKeyframe(1, glm::vec2(0.f, 0.f));
	powerSprite->addKeyframe(1, glm::vec2(0.5f, 0.5f)); //	empty
	powerSprite->addKeyframe(1, glm::vec2(0.5f, 0.f));
	
	powerSprite->changeAnimation(0);

	spritesheet.loadFromFile("assets/pang_BlueAssets.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
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

		sprite->setAnimationSpeed(HIT, 4);
		sprite->addKeyframe(HIT, glm::vec2(0.2f, 0.5f));
		sprite->addKeyframe(HIT, glm::vec2(0.3f, 0.5f));

		sprite->setAnimationSpeed(EMPTY, 1);
		sprite->addKeyframe(EMPTY, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(STAIRS, 4);
		sprite->addKeyframe(STAIRS, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(STAIRS, glm::vec2(0.6f, 0.f));
		sprite->addKeyframe(STAIRS, glm::vec2(0.7f, 0.f));
		sprite->addKeyframe(STAIRS, glm::vec2(0.8f, 0.f));

		sprite->setAnimationSpeed(STAIRS_STAND, 1);
		sprite->addKeyframe(STAIRS_STAND, glm::vec2(0.6f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	isHit = false;
	stairsPressed = false;
	isInvi = false;
	lives = 3;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	int id_collision_down = map->collisionMoveDown(posPlayer, glm::ivec2(36, 36), &posPlayer.y, false);
	int id_collision_up = map->collisionMoveUp(posPlayer, glm::ivec2(36, 36));
	int id_colision_right = map->collisionMoveRight(posPlayer, glm::ivec2(32, 32));
	int id_colision_left = map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32));

	if (isHit)
	{
		if (sprite->animation() != HIT)
			sprite->changeAnimation(HIT);
		sprite->setFreeze(true);
		if (id_collision_down == -1 || (id_collision_down <= 3 && id_collision_down >= 0))
			posPlayer.y += FALL_STEP;
	}
	else
	{

		if (Game::instance().getKey(GLFW_KEY_UP) && (id_collision_up >= 0 && id_collision_up <= 3) && !stairsPressed)
		{
			bStairs = true;
			stairsPressed = true;
			sprite->changeAnimation(STAIRS);
		}

		if (Game::instance().getKey(GLFW_KEY_DOWN) && (id_collision_down >= 0 && id_collision_down <= 3) && !stairsPressed)
		{
			bStairs = true;
			stairsPressed = true;
			sprite->changeAnimation(STAIRS);
		}

		if (!Game::instance().getKey(GLFW_KEY_UP) && !Game::instance().getKey(GLFW_KEY_DOWN) && stairsPressed)
			stairsPressed = false;

		if (!bStairs)
		{
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

			id_collision_down = map->collisionMoveDown(posPlayer, glm::ivec2(36, 36), &posPlayer.y, true);
			posPlayer.y += FALL_STEP;
		}
		else
		{
			if (Game::instance().getKey(GLFW_KEY_UP))
			{
				bool enter = false;
				while (id_colision_right >= 0 && id_colision_right <= 3)
				{
					posPlayer.x += 2;
					id_colision_right = map->collisionMoveRight(posPlayer, glm::ivec2(32, 32));
					enter = true;
				}
				if (enter)
					posPlayer.x -= 2;

				enter = false;
				while (id_colision_left >= 0 && id_colision_left <= 3)
				{
					posPlayer.x -= 2;
					id_colision_left = map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32));
					enter = true;
				}
				if (enter)
					posPlayer.x += 2;
			
				if (!(id_collision_up >= 0 && id_collision_up <= 3))
				{
					bStairs = false;
					while (id_collision_down >= 0 && id_collision_down <= 3)
					{
						posPlayer.y -= 1;
						id_collision_down = map->collisionMoveDown(posPlayer, glm::ivec2(36, 36), &posPlayer.y, true);
					}
					++posPlayer.y;
					sprite->changeAnimation(STAND_LEFT);
				}

				if ((id_collision_up >= 0 && id_collision_up <= 3) || (id_collision_down >= 0 && id_collision_down <= 3))
				{
					posPlayer.y -= 1;
				}
			}
			else if (Game::instance().getKey(GLFW_KEY_DOWN))
			{
				bool enter = false;
				while (id_colision_right >= 0 && id_colision_right <= 3)
				{
					posPlayer.x += 2;
					id_colision_right = map->collisionMoveRight(posPlayer, glm::ivec2(32, 32));
				}
				while (id_colision_left >= 0 && id_colision_left <= 3)
				{
					posPlayer.x -= 2;
					id_colision_left = map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32));
				}

				if (!(id_collision_down >= 0 && id_collision_down <= 3))
				{
					bStairs = false;
					while (id_collision_down == -1)
					{
						posPlayer.y += 1;
						id_collision_down = map->collisionMoveDown(posPlayer, glm::ivec2(36, 36), &posPlayer.y, false);
					}
					sprite->changeAnimation(STAND_LEFT);
				}
				else
				{
					posPlayer.y += 1;
				}
			}
			else 
			{
				sprite->changeAnimation(STAIRS_STAND);
			}
		}
	}
	
	if (isInvi)
	{
		powerSprite->update(deltaTime);
		powerSprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	if (isInvi)
		powerSprite->render();
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

int Player::getLives() {
	return lives;
}


void Player::setHit(bool isB)
{
	if (isHit && !isB)
	{
		sprite->setFreeze(false);
		sprite->changeAnimation(STAND_LEFT);
		if (!isInvi && lives > 0) --lives;
	}
	isHit = isB;
}

void Player::setInvi()
{
	sprite->changeAnimation(EMPTY);
}

void Player::setNormal()
{
	sprite->changeAnimation(STAND_LEFT);
}

void Player::setIsInvi(bool isB)
{
	if (!isB)
		powerSprite->changeAnimation(1);
	else
	{
		powerSprite->changeAnimation(0);
		isInvi = isB;
	}
}

void Player::setOutOfInvi()
{
	isInvi = false;
}