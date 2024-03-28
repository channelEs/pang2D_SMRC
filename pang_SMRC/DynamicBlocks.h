#ifndef _DYNAMICBLOCKS_INCLUDE
#define _DYNAMICBLOCKS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class DynamicBlocks
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	bool isColisionRect(const glm::ivec2& pos, const glm::ivec2& size);

	glm::ivec2 getPosition();

private:

	glm::ivec2 tileMapDispl, posBlock;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif
