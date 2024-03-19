#ifndef _BANG_INCLUDE
#define _BANG_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Bang
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPos();


private:
	glm::ivec2 tileMapDispl, posFire, sizeFire;
	int jumpAngle, startY;
	int reloadTimer;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif

