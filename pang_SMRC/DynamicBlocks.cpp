#include "DynamicBlocks.h"


void DynamicBlocks::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("assets/bang.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 8), glm::vec2(1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.6f));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.7f));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.8f));
	sprite->addKeyframe(1, glm::vec2(0.f, 0.9f));

	sprite->changeAnimation(0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBlock.x), float(tileMapDispl.y + posBlock.y)));
}

void DynamicBlocks::render()
{
	sprite->render();
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
	int r1x = posBlock.x, r1y = posBlock.y, r1w = 16, r1h = 16;
	int r2x = pos.x, r2y = pos.y, r2w = size.x, r2h = size.y;
	if (r1x + r1w >= r2x && r1x <= r2x + r2w &&
		r1y + r1h >= r2y && r1y <= r2y + r2h) return true;
	return false;
}

glm::ivec2 DynamicBlocks::getPosition()
{
	return posBlock;
}