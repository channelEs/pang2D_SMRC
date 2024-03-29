#ifndef _BANG_INCLUDE
#define _BANG_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Bang
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::ivec2 getPos();
	glm::ivec2 getSize();
	int getType();


private:
	glm::ivec2 tileMapDispl, posBangDync, posBangStatic, sizeBang;
	int type;
	int reloadTimer;
	Texture spritesheet;
	Sprite* sprite;

};

#endif

