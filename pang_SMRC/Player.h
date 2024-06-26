#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setHit(bool isB);
	void setInvi();
	void setNormal();
	void setIsInvi(bool isB);
	void setOutOfInvi();
	void setScore(int points);
	void resetStreak();
	void updateStreak(int bubbleSize);

	int getLives();
	int getScore();
	int calculateStreakMultiplier();

	glm::vec2 getPosition() const;
	
private:
	bool bStairs, isHit, isInvi, stairsPressed;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, lives, score, lastBubbleSize, streakLength;
	Texture spritesheet, powerSheet;
	Sprite *sprite, *powerSprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


