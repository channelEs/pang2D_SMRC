#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bang.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 2
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define MOVE_STEP 2

void Bang::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("assets/bang.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::ivec2  sizeFire = glm::ivec2(8, 8);
	clicked = false;
	reloadTimer = 0;

	sprite = Sprite::createSprite(sizeFire, glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFire.x), float(tileMapDispl.y + posFire.y)));
}

void Bang::update(int deltaTime)
{
    sprite->update(deltaTime);



    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFire.x), float(tileMapDispl.y + posFire.y)));
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
	posFire = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFire.x), float(tileMapDispl.y + posFire.y)));
}

bool Bang::getClicked() {
	return clicked;
}
