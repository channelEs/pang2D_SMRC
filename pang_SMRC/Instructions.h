#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE


#include "Sprite.h"
#include "ShaderProgram.h"

class Instructions
{

public:
	void init();
	void update(int deltaTime);
	void render();
	void initShaders();

	bool isActive();
	void nextScene();

private:
	int sceneNum;
	ShaderProgram texProgram;
	Texture spritesheet;
	Sprite* sprite;
	glm::mat4 projection;
};
#endif
