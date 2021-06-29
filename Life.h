#pragma once
#include "Simulation.h"
#include "Graphics.h"
#include "Timer.h"
#include "Input.h"

#define target_fps 100

class Life {
public:
	Life() : phys_1("Physics"), phys_2("Array"), graphics_1("Graphics") {}
	int start(int argc, char** argv);
	Simulation *sim;
	Graphics *gphx;
	Input *input;

	Timer phys_1;
	Timer phys_2;
	Timer graphics_1;

	bool timerenabled = false;
};

extern Life *instance;