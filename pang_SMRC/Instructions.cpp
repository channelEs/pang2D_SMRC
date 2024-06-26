#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Instructions.h"
#include "Game.h"

enum PlayerAnims
{
	MENU01, MENU02, MENU03, MENU04, MENU05, MENU06
};

void Instructions::init()
{
	initShaders();

	spritesheet.loadFromFile("assets/pang_startGame.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f), glm::vec2(0.5, 1.0/3), &spritesheet, &texProgram);
	sprite->setNumberAnimations(6);

		sprite->setAnimationSpeed(MENU01, 8);
		sprite->addKeyframe(MENU01, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(MENU02, 8);
		sprite->addKeyframe(MENU02, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(MENU03, 8);
		sprite->addKeyframe(MENU03, glm::vec2(0.f, 1.0 / 3));

		sprite->setAnimationSpeed(MENU04, 8);
		sprite->addKeyframe(MENU04, glm::vec2(0.5f, 1.0 / 3));

		sprite->setAnimationSpeed(MENU05, 8);
		sprite->addKeyframe(MENU05, glm::vec2(0.f, 2.0 / 3));

		sprite->setAnimationSpeed(MENU06, 8);
		sprite->addKeyframe(MENU06, glm::vec2(0.5f, 2.0 / 3));

	sceneNum = 0;
	sprite->changeAnimation(sceneNum);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2), 0.f);
	sprite->setPosition(glm::vec2(float(0.), float(0.)));
}

void Instructions::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Instructions::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	sprite->render();
}

bool Instructions::isActive()
{
	return (sceneNum >= 6);
}

void Instructions::nextScene()
{
	++sceneNum;
	sprite->changeAnimation(sceneNum);
}

void Instructions::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}