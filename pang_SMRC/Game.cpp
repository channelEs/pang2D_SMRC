#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 0.f);

	instructions.init();
	int numLevels = 3;
	for (int i = 0; i < numLevels; ++i) {
		scenes.push_back(new Scene());
		scenes[i]->init(i);
	}
	inLevel = 0;

	if (!text.init("fonts/StarTrek_future.ttf"))
	{
		cout << "Could not load font!!!" << endl;
	}
}

bool Game::update(int deltaTime)
{
	if (!instructions.isActive())
	{
		instructions.update(deltaTime);
	}
	else
	{
		int event = scenes[inLevel]->update(deltaTime);
		if (event == PLAYER_HIT)
			--playerLives;
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
	else            
	{
		scenes[inLevel]->render();
		if (inLevel == 0)
		{
			text.render("Welcome to PANG", glm::vec2(SCREEN_WIDTH/2, 50), 50, glm::vec4(1, 1, 1, 1));
			text.render("1 -> LEVEL_01", glm::vec2(SCREEN_WIDTH *	0.25, SCREEN_HEIGHT * 0.75), 50, glm::vec4(1, 1, 1, 1));
			text.render("2 -> LEVEL_02", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.85), 50, glm::vec4(1, 1, 1, 1));
			text.render("0 -> MAIN MENU", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.95), 50, glm::vec4(1, 1, 1, 1));
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
	if (key == GLFW_KEY_SPACE && !instructions.isActive())
	{
		instructions.nextScene();
	}
	if (key == GLFW_KEY_C) {
		scenes[inLevel]->generateBang();
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



