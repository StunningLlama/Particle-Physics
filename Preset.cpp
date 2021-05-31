#include "stdafx.h"
#include "Preset.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "Simulation.h"
#include "Life.h"
#include "Input.h"

void Preset::startToilet() {
	drawing = true;
	frame = 0;
	instance->input->updateMouse = false;
	instance->input->coordX = 0.0f;
	instance->input->coordY = 0.0f;
	instance->input->keyboard('b', 0, 0);
	instance->input->keyboard('a', 0, 0);
	instance->input->keyboard('1', 0, 0);
}

void Preset::genToilet() {
	Input *in = instance->input;
	if (frame == 0) {
		in->mouseDown = true;
		in->coordX = cx + innerradius*cos(toRad(thetai));
		in->coordY = cy + innerradius*sin(toRad(thetai));
	}
	else if (frame < 40) {
		float frac = frame / 40.0f;
		in->coordX = cx + innerradius*cos(toRad(thetai + (thetaf - thetai)*frac));
		in->coordY = cy + innerradius*sin(toRad(thetai + (thetaf - thetai)*frac));
	}
	else if (frame < 80) {
		float frac = (frame-40.0f) / 40.0f;
		float px = cx + innerradius*cos(toRad(thetaf));
		float py = cy + innerradius*sin(toRad(thetaf));
		float ux = sin(toRad(thetaf));
		float uy = -cos(toRad(thetaf));
		in->coordX = px + ux * dist * frac;
		in->coordY = py + uy * dist * frac;
	}
	else if (frame == 80) {
		in->mouseDown = false;
	}
	else if (frame == 81) {
		in->mouseDown = true;
		in->coordX = cx + outerradius*cos(toRad(thetai));
		in->coordY = cy + outerradius*sin(toRad(thetai));
	}
	else if (frame < 122) {
		float frac = (frame - 82.0f) / 40.0f;
		in->coordX = cx + outerradius*cos(toRad(thetai + (thetaf - thetai)*frac));
		in->coordY = cy + outerradius*sin(toRad(thetai + (thetaf - thetai)*frac));
	}
	else if (frame < 162) {
		float frac = (frame - 122.0f) / 40.0f;
		float px = cx + outerradius*cos(toRad(thetaf));
		float py = cy + outerradius*sin(toRad(thetaf));
		float ux = sin(toRad(thetaf));
		float uy = -cos(toRad(thetaf));
		in->coordX = px + ux * dist * frac;
		in->coordY = py + uy * dist * frac;
	}
	else if (frame == 162) {
		drawing = false;
		in->mouseDown = false;
		instance->input->updateMouse = true;
	}
	frame++;
}

float Preset::toRad(float deg) {
	return ((float)M_PI)*deg / 180.0f;
}