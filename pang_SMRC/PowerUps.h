#ifndef _POWERUPS_INCLUDE
#define _POWERUPS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class PowerUps
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int powerId, float currentTime);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() const;

	bool isColisionRect(const glm::ivec2& pos, const glm::ivec2& size);
	int getPowerID();
	float getIniTime();

private:
	glm::ivec2 tileMapDispl, posObj;
	Sprite* sprite;
	TileMap* map;
	Texture spritesheet;
	float iniTime;
};

#endif // _POWERUPS_INCLUDE

