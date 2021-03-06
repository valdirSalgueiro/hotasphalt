#include <stdlib.h>
#include <stdio.h>
#define GLFW_INCLUDE_ES2
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "Engine.h"
#include "util.h"

int init_gl(void);
void do_frame();
void shutdown_gl();

Engine* gameEngine;
const int width = 800,height = 480;
int dir;
			
void GLFWCALL keyfun(int key, int action);
			
int main(void) 
{
	gameEngine = new Engine();
	if (init_gl() == GL_TRUE) {		
		gameEngine->init(width,height);
		emscripten_set_main_loop(do_frame, 0, 1);
	}
		
	shutdown_gl();

	return 0;
}

int init_gl()
{
	if (glfwInit() != GL_TRUE) {
		printf("glfwInit() failed\n");
		return GL_FALSE;
	}

	if (glfwOpenWindow(width, height, 8, 8, 8, 8, 16, 0, GLFW_WINDOW) != GL_TRUE) {
		printf("glfwOpenWindow() failed\n");
    	return GL_FALSE;
    }
	
	 glfwSetKeyCallback(keyfun);

    return GL_TRUE;
}

void GLFWCALL keyfun(int key, int action) {
	if (key == GLFW_KEY_SPACE  )
		if(action == GLFW_RELEASE)
			gameEngine->setTouch(false);
		else
			gameEngine->setTouch(true);
		
	if(action == GLFW_RELEASE){
		if (key == GLFW_KEY_RIGHT)
			dir &= ~DIRECTION::RIGHT;

		if (key == GLFW_KEY_LEFT)
			dir &= ~DIRECTION::LEFT;

		if (key == GLFW_KEY_UP)
			dir &= ~DIRECTION::UP;

		if (key == GLFW_KEY_DOWN)
			dir &= ~DIRECTION::DOWN;
	}
	else
	{
		if (key == GLFW_KEY_RIGHT)
			dir |= DIRECTION::RIGHT;

		if (key == GLFW_KEY_LEFT)
			dir |= DIRECTION::LEFT;

		if (key == GLFW_KEY_UP)
			dir |= DIRECTION::UP;

		if (key == GLFW_KEY_DOWN)
			dir |= DIRECTION::DOWN;		
	}
		
}


void do_frame()
{	
	int state = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		int xpos, ypos;
		glfwGetMousePos (&xpos, &ypos);
		gameEngine->handleInput(0, xpos, ypos);
	}
	gameEngine->setDir(dir);
	gameEngine->update(1.0f/60.0f);
	gameEngine->render(1.0f/60.0f);
	glfwSwapBuffers();
}

void shutdown_gl()
{
	glfwTerminate();
}