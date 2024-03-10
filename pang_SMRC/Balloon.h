#ifndef _BALLOON_INCLUDE
#define _BALLOON_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Balloon
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posBalloon;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};

#endif // !_BALLOON_INCLUDE

