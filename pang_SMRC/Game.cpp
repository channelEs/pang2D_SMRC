#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 0.f);

	instructions.init();
	int numLevels = 4;
	for (int i = 0; i < numLevels; ++i) {
		scenes.push_back(new Scene());
		scenes[i]->init(i);
	}
	inLevel = 0;

	if (!text.init("fonts/StarTrek_future.ttf"))
	{
		cout << "Could not load font!!!" << endl;
	}
	playerLives = 3;
	time = points = 0;
}

bool Game::update(int deltaTime)
{
	if (!instructions.isActive())
	{
		instructions.update(deltaTime);
	}
	else if (inLevel < scenes.size())
	{
		int event = scenes[inLevel]->update(deltaTime);
		if (event == 1)
		{
			delete scenes[inLevel];
			scenes[inLevel] = new Scene();
			scenes[inLevel]->init(inLevel);
			--playerLives;
			if (playerLives == 0)
			{
				inLevel = 6;
				playerLives = 3;
			}
		}
		if (event == 2)
		{
			delete scenes[inLevel];
			scenes[inLevel] = new Scene();
			scenes[inLevel]->init(inLevel);
			inLevel = 5;
		}
		if (event == 6)
		{
			delete scenes[inLevel];
			scenes[inLevel] = new Scene();
			scenes[inLevel]->init(inLevel);
			inLevel = 6;
		}
		if (time == 60) {
			inLevel = 6;
		}
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!instructions.isActive())
	{
		instructions.render();
		text.render("SPACE -> skip instructions", glm::vec2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.9), 50, glm::vec4(1, 1, 1, 1));
	}
	else if (inLevel == 5)
	{
		text.render("1 -> LEVEL_01", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.3), 50, glm::vec4(1, 1, 1, 1));
		text.render("2 -> LEVEL_02", glm::vec2(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.5), 50, glm::vec4(1, 1, 1, 1));
		text.render("3 -> LEVEL_03", glm::vec2(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.7), 50, glm::vec4(1, 1, 1, 1));
	}
	else if (inLevel == 6)
	{
		text.render("GAME OVER", glm::vec2(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.3), 50, glm::vec4(1, 1, 1, 1));
		text.render("0 -> MAIN MENU", glm::vec2(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT * 0.50), 50, glm::vec4(1, 1, 1, 1));
		text.render("MADE BY:", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.75), 50, glm::vec4(1, 1, 1, 1));
		text.render("ROGER CANAL", glm::vec2(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT * 0.75), 50, glm::vec4(1, 1, 1, 1));
		text.render("SARA MENDEZ", glm::vec2(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT * 0.85), 50, glm::vec4(1, 1, 1, 1));
	}
	else            
	{
		scenes[inLevel]->render();
		if (inLevel == 0)
		{
			text.render("Welcome to PANG", glm::vec2(SCREEN_WIDTH/2, 50), 50, glm::vec4(1, 1, 1, 1));
			text.render("1 -> LEVEL_01", glm::vec2(SCREEN_WIDTH *	0.25, SCREEN_HEIGHT * 0.75), 50, glm::vec4(1, 1, 1, 1));
			text.render("2 -> LEVEL_02", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.85), 50, glm::vec4(1, 1, 1, 1));
			text.render("3 -> LEVEL_03", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.95), 50, glm::vec4(1, 1, 1, 1));
			text.render("0 -> MAIN MENU", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.65), 50, glm::vec4(1, 1, 1, 1));
			text.render("C -> SHOOT", glm::vec2(SCREEN_WIDTH * 0.60, SCREEN_HEIGHT * 0.65), 50, glm::vec4(1, 1, 1, 1));
			text.render("G -> GOD MODE", glm::vec2(SCREEN_WIDTH * 0.60, SCREEN_HEIGHT * 0.75), 50, glm::vec4(1, 1, 1, 1));
		}
		else
		{
			int power = scenes[inLevel]->getPowerActiveId();
			time = scenes[inLevel]->getCurrentTime() * 0.001;
			points = scenes[inLevel]->getPlayerScore();
			text.render("POWER: " + std::to_string(power), glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.85), 50, glm::vec4(1, 1, 1, 1));
			text.render("LIVES: " + std::to_string(playerLives), glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.95), 50, glm::vec4(1, 1, 1, 1));
			text.render("TIME: " + std::to_string(time), glm::vec2(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.10), 50, glm::vec4(1, 1, 1, 1));
			text.render("POINTS: " + std::to_string(points), glm::vec2(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.85), 50, glm::vec4(1, 1, 1, 1));
		}
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	if (key == GLFW_KEY_0)
	{
		inLevel = 0;
	}
	if (key == GLFW_KEY_1)
	{
		inLevel = 1;
	}
	if (key == GLFW_KEY_2)
	{
		inLevel = 2;
	}
	if (key == GLFW_KEY_3)
	{
		inLevel = 3;
	}
	if (key == GLFW_KEY_SPACE && !instructions.isActive())
	{
		instructions.nextScene();
	}
	if (key == GLFW_KEY_C) {
		scenes[inLevel]->generateBang();
	}
	if (key == GLFW_KEY_T)
	{
		scenes[inLevel]->setPower(DYNAMITE);
	}
	if (key == GLFW_KEY_Y)
	{
		scenes[inLevel]->setPower(DOUBLE_WIRE);
	}
	if (key == GLFW_KEY_U)
	{
		scenes[inLevel]->setPower(FREEZE_TIME);
	}
	if (key == GLFW_KEY_I)
	{
		scenes[inLevel]->setPower(VULCAN_MISSILE);
	}
	if (key == GLFW_KEY_O)
	{
		scenes[inLevel]->setPower(INVINCIBILITY);
	}

	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



