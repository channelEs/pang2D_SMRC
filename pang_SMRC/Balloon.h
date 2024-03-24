#ifndef _BALLOON_INCLUDE
#define _BALLOON_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Balloon
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool startLeft, glm::ivec2& size);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isColisionRectangle(const glm::ivec2& pos, const glm::ivec2& rect);

private:
	void setNextPos();

	bool bJumping;
	bool movingLeft;
	glm::ivec2 tileMapDispl, posBalloon, sizeBalloon;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};

#endif // !_BALLOON_INCLUDE

