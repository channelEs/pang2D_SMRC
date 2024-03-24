#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 0.f);

	instructions.init();
	int numLevels = 2;
	for (int i = 0; i < numLevels; ++i) {
		scenes.push_back(new Scene());
		scenes[i]->init(i);
	}
	inLevel = 0;
}

bool Game::update(int deltaTime)
{
	if (!instructions.isActive())
	{
		instructions.update(deltaTime);
	}
	else
	{
		scenes[inLevel]->update(deltaTime);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!instructions.isActive())
	{
		instructions.render();
	}
	else
	{
		scenes[inLevel]->render();
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	if (key == GLFW_KEY_1)
	{
		inLevel = 0;
	}
	if (key == GLFW_KEY_2)
	{
		inLevel = 1;
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



