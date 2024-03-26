#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Banner.h"
#include "Game.h"

Banner::~Banner() {

}


enum MonedaAnims
{
	TURN, NONE
};

void Banner::init(glm::ivec2 tileMapPos, ShaderProgram& shaderProgram)
{
	shader = shaderProgram;
	spritesheetBanner.loadFromFile("assets/BannerSuperior.png", TEXTURE_PIXEL_FORMAT_RGBA);
	banner = Sprite::createSprite(glm::ivec2(256 * 2, 32 * 2), glm::vec2(1, 1), &spritesheetBanner, &shaderProgram);

	spritesheetMoneda.loadFromFile("assets/MonedaBanner.png", TEXTURE_PIXEL_FORMAT_RGBA);
	moneda = Sprite::createSprite(glm::ivec2(8 * 2, 8 * 2), glm::vec2(0.25, 1), &spritesheetMoneda, &shaderProgram);
	moneda->setNumberAnimations(2);

	moneda->setAnimationSpeed(TURN, 8);
	moneda->addKeyframe(TURN, glm::vec2(0.25 * 0, 1));
	moneda->addKeyframe(TURN, glm::vec2(0.25 * 1, 1));
	moneda->addKeyframe(TURN, glm::vec2(0.25 * 2, 1));

	moneda->setAnimationSpeed(NONE, 8);
	moneda->addKeyframe(NONE, glm::vec2(0.25 * 3, 1));

	text = new Text[5];
	text[0].init(glm::vec2(float(2 * 16), float(3 * 16)), shaderProgram, 2, 0, "0000000"); //Puntos
	text[1].init(glm::vec2(float(13 * 16), float(3 * 16)), shaderProgram, 2, 0, "00"); //Monedas
	text[2].init(glm::vec2(float(19 * 16), float(3 * 16)), shaderProgram, 2, 0, "0"); //World
	text[3].init(glm::vec2(float(21 * 16), float(3 * 16)), shaderProgram, 2, 0, "0"); //Level
	text[4].init(glm::vec2(float(26 * 16), float(3 * 16)), shaderProgram, 2, 0, "000"); //Tiempo

	tileMapDispl = tileMapPos;
	moneda->changeAnimation(TURN);
	banner->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));
	moneda->setPosition(glm::vec2(float(tileMapDispl.x + (11 * 16)), float(tileMapDispl.y + (3 * 16))));
}

void Banner::restart()
{

}

void Banner::update(int deltaTime, int pos_camara)
{
	banner->setPosition(glm::vec2(float(tileMapDispl.x + pos_camara), float(tileMapDispl.y)));
	moneda->setPosition(glm::vec2(float(tileMapDispl.x + (11 * 16) + pos_camara), float(tileMapDispl.y + (3 * 16))));
	for (int i = 0; i < 5; ++i) text[i].setPosition(glm::vec2(float(tileMapDispl.x + pos_camara), float(tileMapDispl.y)));
}

void Banner::render()
{
	banner->render();
	moneda->render();
	for (int i = 0; i < 5; ++i) text[i].render();
}

void Banner::setPoints(int points)
{
	if (points < 10) text[0].setText("000000" + to_string(points));
	else if (points < 100) text[0].setText("00000" + to_string(points));
	else if (points < 1000) text[0].setText("0000" + to_string(points));
	else if (points < 10000) text[0].setText("000" + to_string(points));
	else if (points < 100000) text[0].setText("00" + to_string(points));
	else if (points < 1000000) text[0].setText("0" + to_string(points));
	else text[0].setText(to_string(points));
}

void Banner::setMonedas(int monedas)
{
	if (monedas < 10) text[1].setText("0" + to_string(monedas));
	else text[1].setText(to_string(monedas));
}

void Banner::setLevel(int world, int level)
{
	text[2].setText(to_string(world));
	text[3].setText(to_string(level));
}

void Banner::setTime(float gameTime)
{
	int time = 400 - int(gameTime / 1000);
	if (int(gameTime) < 0) text[4].setText("   ");
	else if (time < 10) text[4].setText("00" + to_string(time));
	else if (time < 100) text[4].setText("0" + to_string(time));
	else if (time <= 400) text[4].setText(to_string(time));
}


