#include "stdafx.h"
#include "Input.h"
#include "Life.h"
#include <gl/glew.h>
#include <gl/glut.h>


void Input::mouseClick(int button, int state, int x, int y) {
	if (!updateMouse)
		return;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		this->mouseDownL = true;
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		this->mouseDownL = false;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		this->mouseDownR = true;
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		this->mouseDownR = false;
}

void Input::mouseMotion(int x, int y) {
	if (!updateMouse)
		return;
	mx = x;
	my = y;
	coordX = 1.0f*instance->sim->xbound * float(x) / float(glutGet(GLUT_WINDOW_WIDTH)) + 0.0f*coordX;
	coordY = 1.0f*instance->sim->ybound * (1.0f - float(y) / float(glutGet(GLUT_WINDOW_HEIGHT))) + 0.0f*coordY;
}

void Input::mousePassiveMotion(int x, int y) {
	if (!updateMouse)
		return;
	mx = x;
	my = y;
	coordX = 1.0f*instance->sim->xbound * float(x) / float(glutGet(GLUT_WINDOW_WIDTH)) + 0.0f*coordX;
	coordY = 1.0f*instance->sim->ybound * (1.0f - float(y) / float(glutGet(GLUT_WINDOW_HEIGHT))) + 0.0f*coordY;
}

void Input::keyboard(unsigned char key, int x, int y) {
	if (key == 'a')
		modebrush = mode_brush_add;
	if (key == 'r')
		modebrush = mode_brush_replace;
	if (key == 'd')
		modebrush = mode_brush_delete;
	if (key == 'm')
		modebrush = mode_brush_move;

	if (key == 'w')
		modematerial = sim_type_water;
	if (key == 'n')
		modematerial = sim_type_air;
	if (key == 'b')
		modematerial = sim_type_barrier;
	if (key == 'z')
		modematerial = sim_type_stone;

	if (key == 'p')
		paused = !paused;
	if (key == 's')
		save = true;
	if (key == 'l')
		load = true;
	if (key == 'c')
		clear = true;
	if (key == 'f')
		nextframe = true;
	if (key == 'x')
		specialfunc = true;

	if (key == '1')
		brushsize = 1;
	if (key == '2')
		brushsize = 2;
	if (key == '3')
		brushsize = 3;
	if (key == '4')
		brushsize = 4;
	if (key == '5')
		brushsize = 5;


	if (key == '0')
		instance->gphx->shadermode = (instance->gphx->shadermode + 1) % 3;
	if (key == '9')
		instance->gphx->bgmode = (instance->gphx->bgmode + 1) % 2;

	if (key == '=')
		density++;
	if (key == '-')
		density--;


	if (key == ']')
		instance->sim->flowvelocity *= 1.05f;
	if (key == '[')
		instance->sim->flowvelocity /= 1.05f;

	if (density < 0)
		density = 0;
	if (density > 5)
		density = 5;
}


void Input::special(int key, int x, int y) {
	if (key == GLUT_KEY_F9)
		instance->gphx->pressurecontrast /= 1.1f;
	if (key == GLUT_KEY_F10)
		instance->gphx->pressurecontrast *= 1.1f;
	if (key == GLUT_KEY_F11)
		instance->gphx->pressureoffset -= 0.1f;
	if (key == GLUT_KEY_F12)
		instance->gphx->pressureoffset += 0.1f;
}