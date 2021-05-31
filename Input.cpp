#include "stdafx.h"
#include "Input.h"
#include "Life.h"
#include <gl/glew.h>
#include <gl/glut.h>


void Input::mouseClick(int button, int state, int x, int y) {
	if (!updateMouse)
		return;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		this->mouseDown = true;
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		this->mouseDown = false;
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

	if (key == 'p')
		paused = !paused;
	if (key == 's')
		save = true;
	if (key == 'l')
		load = true;
	if (key == 'c')
		clear = true;
	if (key == 'f')
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


	if (key == '=')
		density++;
	if (key == '-')
		density--;

	if (density < 0)
		density = 0;
	if (density > 5)
		density = 5;
}