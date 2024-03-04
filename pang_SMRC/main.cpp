#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


#define TARGET_FRAMERATE 60.0f


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		Game::instance().keyPressed(key);
	else if (action == GLFW_RELEASE)
		Game::instance().keyReleased(key);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game::instance().mouseMove(int(xpos), int(ypos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		Game::instance().mousePress(button);
	else if (action == GLFW_RELEASE)
		Game::instance().mouseRelease(button);
}


int main(void)
{
	GLFWwindow* window;
	double timePerFrame = 1.f / TARGET_FRAMERATE, timePreviousFrame, currentTime;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Set window initial position */
	glfwSetWindowPos(window, 100, 100);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Set callbacks */
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	/* Init glew to have access to GL extensions */
	glewExperimental = GL_TRUE;
	glewInit();

	/* Init step of the game loop */
	Game::instance().init();
	timePreviousFrame = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		currentTime = glfwGetTime();
		if (currentTime - timePreviousFrame >= timePerFrame)
		{
			/* Update & render steps of the game loop */
			if(!Game::instance().update(int(1000.0f * (currentTime - timePreviousFrame))))
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			Game::instance().render();
			timePreviousFrame = currentTime;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		}

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

