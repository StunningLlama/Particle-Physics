#pragma once

#include "simulation/Simulation.h"
#include "graphics/Graphics.h"
#include "Timer.h"
#include "Input.h"

#define target_fps 100

class Life {
public:
	Life() : phys_1("Physics (Part 1)"), phys_2("Physics (Part 2)"), graphics_1("Graphics") {}
	int start(int argc, char** argv);
	Simulation *sim = nullptr;
	Graphics *gphx = nullptr;
	Input *input = nullptr;

	Timer phys_1;
	Timer phys_2;
	Timer graphics_1;
};

extern Life *instance;