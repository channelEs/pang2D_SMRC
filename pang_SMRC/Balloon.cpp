#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Balloon.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96

void Balloon::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("assets/pang_BlueAssets.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1, 0.25), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBalloon.x), float(tileMapDispl.y + posBalloon.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
}