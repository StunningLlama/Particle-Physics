#include "Includes.h"
#include "Input.h"
#include "Life.h"


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
	if (button == 3 && state == GLUT_DOWN)
		brushsize = clamp(brushsize + 1, 1, 5);
	if (button == 4 && state == GLUT_DOWN)
		brushsize = clamp(brushsize - 1, 1, 5);
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
		instance->gphx->gui.setBrush(mode_brush_add);
	if (key == 'r')
		instance->gphx->gui.setBrush(mode_brush_replace);
	if (key == 'd')
		instance->gphx->gui.setBrush(mode_brush_delete);
	if (key == 'm')
		instance->gphx->gui.setBrush(mode_brush_move);
	if (key == 'q')
		instance->gphx->gui.setBrush(mode_brush_drag);

	if (key >= '1' && key <= '9')
		instance->gphx->gui.setMaterial(key - '1');

	if (key == 'p')
		instance->gphx->gui.togglePause();
	if (key == 0x13)
		save = true;
	if (key == 0x0F)
		load = true;
	if (key == 'c')
		clear = true;
	if (key == 'f')
		nextframe = true;
	if (key == 'g')
		instance->sim->enablegravity = !instance->sim->enablegravity;
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