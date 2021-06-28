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
	if (key == 'g')
		modebrush = mode_brush_drag;

	if (key == '1')
		modematerial = sim_type_water;
	if (key == '2')
		modematerial = sim_type_air;
	if (key == '3')
		modematerial = sim_type_barrier;
	if (key == '4')
		modematerial = sim_type_stone;
	if (key == '5')
		modematerial = sim_type_sand;
	if (key == '6')
		modematerial = sim_type_cloth;
	if (key == '7')
		modematerial = sim_type_elastic;
	if (key == '8')
		modematerial = sim_type_plastic;
	if (key == '9')
		modematerial = sim_type_brittle;

	if (key == 'p')
		paused = !paused;
	if (key == 0x13)
		save = true;
	if (key == 0x17)
		load = true;
	if (key == 'c')
		clear = true;
	if (key == 'f')
		nextframe = true;
	if (key == 'x')
		specialfunc = true;

	if (key == 'w')
		brushsize = clamp(brushsize + 1, 1, 5);
	if (key == 's')
		brushsize = clamp(brushsize - 1, 1, 5);


	if (key == '`')
		instance->gphx->shadermode = (instance->gphx->shadermode + 1) % 3;
	if (key == '\t')
		instance->gphx->bgmode = (instance->gphx->bgmode + 1) % 2;
	if (key == '/')
		instance->gphx->displaybonds = !instance->gphx->displaybonds;

	if (key == '=')
		density = clamp(density + 1, 0, 5);
	if (key == '-')
		density = clamp(density - 1, 0, 5);


	if (key == ']')
		instance->sim->flowvelocity *= 1.05f;
	if (key == '[')
		instance->sim->flowvelocity /= 1.05f;

	if (key == ',')
		instance->sim->boundarytype = (instance->sim->boundarytype + 1) % 2;
}


void Input::special(int key, int x, int y) {
	if (key == GLUT_KEY_F5)
		instance->gphx->pressurecontrast /= 1.1f;
	if (key == GLUT_KEY_F6)
		instance->gphx->pressurecontrast *= 1.1f;
	if (key == GLUT_KEY_F7)
		instance->gphx->pressureoffset -= 0.1f;
	if (key == GLUT_KEY_F8)
		instance->gphx->pressureoffset += 0.1f;
}

int Input::clamp(int x, int min, int max) {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}