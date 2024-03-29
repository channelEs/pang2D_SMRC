#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


void Game::init()
{
	/*
	// Check for available audio devices
	const char** devices = engine->getAvailableDevices();
	if (!devices || !*devices) {
		std::cerr << "No audio devices found" << std::endl;
		engine->drop();
	}
	std::cout << "Available audio devices:" << std::endl;
	for (int i = 0; devices[i]; ++i) {
		std::cout << devices[i] << std::endl;
	}

	// Play audio file
	ISound* sound = engine->play2D("sounds/baba-is-theme.mp3", true, false, true);
	if (!sound) {
		std::cerr << "Failed to play audio file" << std::endl;
		engine->drop();
	}
	*/


	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 0.f);

	instructions.init();
	int numLevels = 4;
	for (int i = 0; i < numLevels; ++i) {
		scenes.push_back(new Scene());
		scenes[i]->init(i);
	}
	inLevel = 0;

	if (!text.init("fonts/Acme-Regular.ttf"))
	{
		cout << "Could not load font!!!" << endl;
	}
	playerLives = 3;
	time = 90;
	stagePoints = totalPoints = 0;
	isGodMode = false;
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
			isGodMode = false;
		}
		if (event == 2)
		{
			delete scenes[inLevel];
			scenes[inLevel] = new Scene();
			scenes[inLevel]->init(inLevel);
			inLevel = 5;
			isGodMode = false;
			totalPoints += stagePoints;
		}
		if (event == 6)
		{
			delete scenes[inLevel];
			scenes[inLevel] = new Scene();
			scenes[inLevel]->init(inLevel);
			inLevel = 6;
			isGodMode = false;
		}
		if (time == 0) {
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
		text.render("SPACE -> skip for instructions", glm::vec2(SCREEN_WIDTH * 0.5 - 64*3, SCREEN_HEIGHT - 20), 20, glm::vec4(1, 1, 1, 1));
	}
	else if (inLevel == 5)
	{
		text.render("0 -> RESET GAME", glm::vec2(SCREEN_WIDTH * 0.4, 20), 20, glm::vec4(1, 1, 1, 1));
		text.render("1 -> BARCELONA", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.3), 34, glm::vec4(0, 1, 1, 1));
		text.render("2 -> LONDON", glm::vec2(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.4), 34, glm::vec4(0.545, 0.27, 0.074, 1));
		text.render("3 -> NEW YORK", glm::vec2(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.5), 34, glm::vec4(0.043, 0.498, 0.671, 1));
		text.render("LIVES: " + std::to_string(playerLives), glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.7), 34, glm::vec4(1, 1, 1, 1));
		text.render("LAST STAGE POINTS: " + std::to_string(stagePoints), glm::vec2(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.7), 34, glm::vec4(1, 1, 1, 1));
		text.render("TOTAL POINTS: " + std::to_string(totalPoints), glm::vec2(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.8), 34, glm::vec4(1, 1, 1, 1));
	}
	else if (inLevel == 6)
	{
		scenes[0]->render();
		text.render("GAME OVER", glm::vec2(SCREEN_WIDTH * 0.4, SCREEN_HEIGHT * 0.1), 50, glm::vec4(0.9, 0, 0, 1));
		text.render("0 -> RESET GAME", glm::vec2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.25), 20, glm::vec4(1, 1, 1, 1));
		text.render("ESC -> EXIT GAME", glm::vec2(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.3), 20, glm::vec4(1, 1, 1, 1));
		text.render("MADE BY:", glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.75), 50, glm::vec4(1, 1, 1, 1));
		text.render("ROGER CANAL", glm::vec2(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT * 0.65), 34, glm::vec4(1, 1, 1, 1));
		text.render("SARA MENDEZ", glm::vec2(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT * 0.75), 34, glm::vec4(1, 1, 1, 1));
		text.render("TOTAL POINTS: " + std::to_string(totalPoints), glm::vec2(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT * 0.9), 34, glm::vec4(0, 0.7, 0.7, 1));
	}
	else            
	{
		scenes[inLevel]->render();
		if (inLevel == 0)
		{
			text.render("Welcome to PANG", glm::vec2(7, 39), 34, glm::vec4(1, 1, 1, 1));
			text.render("0 -> RESET GAME", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.6), 34, glm::vec4(1, 1, 1, 1));
			text.render("1 -> BARCELONA", glm::vec2(SCREEN_WIDTH *	0.15, SCREEN_HEIGHT * 0.7), 34, glm::vec4(0, 1, 1, 1));
			text.render("2 -> LONDON", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.8), 34, glm::vec4(0.545, 0.27, 0.074, 1));
			text.render("3 -> NEW YORK", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.9), 34, glm::vec4(0.043, 0.498, 0.671, 1));
			text.render("C -> SHOOT", glm::vec2(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.75), 34, glm::vec4(1, 0, 0, 1));
			text.render("G -> GOD MODE", glm::vec2(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.85), 34, glm::vec4(1, 1, 1, 1));
		}
		else
		{
			int power = scenes[inLevel]->getPowerActiveId();
			isGodMode = scenes[inLevel]->isGodModeActive();
			stagePoints = scenes[inLevel]->getPlayerScore();
			if (inLevel == 1)
				text.render("BARCELONA", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.85), 34, glm::vec4(0, 1, 1, 1));
			else if (inLevel == 2)
				text.render("LONDON", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.85), 34, glm::vec4(0.545, 0.27, 0.074, 1));
			else if (inLevel == 3)
				text.render("NEW YORK", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.85), 34, glm::vec4(0.043, 0.498, 0.671, 1));
			
			switch (power)
			{
			case DOUBLE_WIRE:
				text.render("BANG: DOUBLE_WIRE", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.95), 34, glm::vec4(1, 1, 1, 1));
				break;

			case VULCAN_MISSILE:
				text.render("BANG: VULCAN MISSILE", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.95), 34, glm::vec4(1, 1, 1, 1));
				break;

			default:
				text.render("BANG: WIRE", glm::vec2(SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.95), 34, glm::vec4(1, 1, 1, 1));
				break;
			}
			
			text.render("LIVES: " + std::to_string(playerLives), glm::vec2(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.85), 34, glm::vec4(1, 1, 1, 1));
			text.render("STAGE POINTS: " + std::to_string(stagePoints), glm::vec2(SCREEN_WIDTH * 0.55, SCREEN_HEIGHT * 0.95), 34, glm::vec4(1, 1, 1, 1));
			
			time = 90 - scenes[inLevel]->getCurrentTime() * 0.001;
			if (time < 20 && time > 10)
				text.render("TIME: " + std::to_string(time), glm::vec2(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.10), 34, glm::vec4(1, 1, 0, 1));
			else if (time <= 10)
				text.render("TIME: " + std::to_string(time), glm::vec2(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.10), 34, glm::vec4(1, 0, 0, 1));
			else
				text.render("TIME: " + std::to_string(time), glm::vec2(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.10), 34, glm::vec4(1, 1, 1, 1));
		}
	}
	if (isGodMode)
	{
		text.render("GOD MODE", glm::vec2(SCREEN_WIDTH * 0.5 - 64, SCREEN_HEIGHT - 2), 50, glm::vec4(0.855, 0.647, 0.125, 1));
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	if (key == GLFW_KEY_0)
	{
		inLevel = 0;
		totalPoints = 0;
	}
	if (key == GLFW_KEY_1)
	{
		inLevel = 1;
		delete scenes[inLevel];
		scenes[inLevel] = new Scene();
		scenes[inLevel]->init(inLevel);
	}
	if (key == GLFW_KEY_2)
	{
		inLevel = 2;
		delete scenes[inLevel];
		scenes[inLevel] = new Scene();
		scenes[inLevel]->init(inLevel);
	}
	if (key == GLFW_KEY_3)
	{
		inLevel = 3;
		delete scenes[inLevel];
		scenes[inLevel] = new Scene();
		scenes[inLevel]->init(inLevel);
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
	if (key == GLFW_KEY_G && instructions.isActive())
	{
		isGodMode = !isGodMode;
		scenes[inLevel]->setGodMode(isGodMode);
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



