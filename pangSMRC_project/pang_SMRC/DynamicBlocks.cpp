#include "DynamicBlocks.h"


void DynamicBlocks::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int lenghtX)
{
	spritesheet.loadFromFile("assets/brikDestroyAnimation.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(lenghtX, 8), glm::vec2(1., 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 1);
	sprite->setAnimationSpeed(1, 3);
	float i = 0.f;
	if (lenghtX == 4)
		i = 0.f;
	else if (lenghtX == 3)
		i = 0.5f;

	sprite->addKeyframe(0, glm::vec2(0.f, 0.f + i));

	sprite->addKeyframe(1, glm::vec2(0.f, 0.1f + i));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.2f + i));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.3f + i));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.4f + i));
	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
	lenght = lenghtX;
	isDestroy = false;
}

void DynamicBlocks::render()
{
	sprite->render();
}

void DynamicBlocks::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void DynamicBlocks::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void DynamicBlocks::setPosition(const glm::vec2& pos)
{
	posBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

bool DynamicBlocks::isColisionRect(const glm::ivec2& pos, const glm::ivec2& size)
{
	int r1x = posBlock.x, r1y = posBlock.y, r1w = lenght, r1h = 8;
	int r2x = pos.x, r2y = pos.y, r2w = size.x, r2h = size.y;
	if (r1x + r1w >= r2x && r1x <= r2x + r2w &&
		r1y + r1h >= r2y && r1y <= r2y + r2h) return true;
	return false;
}

glm::ivec2 DynamicBlocks::getPosition()
{
	return posBlock;
}

int DynamicBlocks::getSize()
{
	return lenght;
}

void DynamicBlocks::setIsDestroy(bool isB, float currentT)
{
	isDestroy = isB;
	if (isDestroy)
		sprite->changeAnimation(1);
	iniTimeDestroy = currentT;
}

float DynamicBlocks::getDestroyTime()
{
	return iniTimeDestroy;
}

bool DynamicBlocks::isDestroyActive()
{
	return isDestroy;
}