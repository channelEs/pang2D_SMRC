#ifndef _BANNER_INCLUDE
#define _BANNER_INCLUDE

#include "Sprite.h"
#include "Text.h"


class Banner
{

public:
	~Banner();
	void init(glm::ivec2 pos, ShaderProgram& shaderProgram);
	void restart();

	void update(int deltaTime, int pos_camara);
	void render();

	void setPoints(int points);
	void setMonedas(int monedas);
	void setLevel(int world, int level);
	void setTime(float gameTime);


private:
	glm::ivec2 tileMapDispl;
	ShaderProgram shader;
	Texture spritesheetBanner, spritesheetMoneda;
	Sprite* banner, * moneda;
	Text* text;
};


#endif // _BANNER_INCLUDE
