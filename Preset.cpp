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
	instance->input->modematerial = sim_type_barrier;
	instance->input->modebrush = mode_brush_add;
	instance->input->brushsize = 1;
}

void Preset::genToilet() {
	Input *in = instance->input;
	if (frame == 0) {
		in->mouseDownL = true;
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
		in->mouseDownL = false;
	}
	else if (frame == 81) {
		in->mouseDownL = true;
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
		in->mouseDownL = false;
		instance->input->updateMouse = true;
	}
	frame++;
}


void Preset::startAirfoil() {
	drawing = true;
	frame = 0;
	instance->input->updateMouse = false;
	instance->input->coordX = 0.0f;
	instance->input->coordY = 0.0f;
	instance->input->modematerial = sim_type_barrier;
	instance->input->modebrush = mode_brush_add;
	instance->input->brushsize = 1;
}

void Preset::genAirfoil() {
	Input* in = instance->input;

#ifdef BIG_CHUNGUS_SIZE
	float centerx = 250.0f;
	float centery = 250.0f;
	float scale = 60.0f;
#else
	float centerx = 100.0f;
	float centery = 130.0f;
	float scale = 40.0f;
#endif

	if (frame == 0) {
		in->mouseDownL = true;
		in->coordX = centerx + scale * xParam(0);
		in->coordY = centery + scale * yParam(0);
	}
	else if (frame < 200) {
		float frac = frame / 200.0f;
		in->coordX = centerx + scale * xParam(frac*8.0f);
		in->coordY = centery + scale * yParam(frac * 8.0f);
	}
	else if (frame == 200) {
		drawing = false;
		in->mouseDownL = false;
		instance->input->updateMouse = true;
	}
	frame++;
}

float Preset::xParam(float t) {
	return sqrt(1 + (4.0f - t) * (4.0f - t)) - 1;
}

float Preset::yParam(float t) {
	return 0.25*((atan(1.2f * (t - 4.0f)) - atan(-4.0f * 1.2f)) * (atan(0.4f * (8.0f - t))));
}

float Preset::toRad(float deg) {
	return ((float)M_PI)*deg / 180.0f;
}