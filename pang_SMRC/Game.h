#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include <irrKlang.h>
#include "Scene.h"
#include "Instructions.h"
#include "Text.h"

using namespace irrklang;

#define SCREEN_WIDTH 48*8*3
#define SCREEN_HEIGHT 34*8*3

#define ID_BLOCKS_BRICK_V 20
#define ID_BLOCKS_BRICK_H_01 24
#define ID_BLOCKS_BRICK_H_02 25

enum events {
	PLAYER_HIT
};

enum powerUps
{
	DYNAMITE, DOUBLE_WIRE, FREEZE_TIME, VULCAN_MISSILE, INVINCIBILITY
};

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

private:
	bool bPlay;					// Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	std::vector<Scene*> scenes; // instances of Scene for every level in the game
	Instructions instructions;	// instance for the instructions at the start of the game
	int inLevel;				// num of level playing
	int playerLives;			// num of player lives
	Text text;
	int time, stagePoints, totalPoints;
	bool isGodMode;
};


#endif // _GAME_INCLUDE


