// Life.cpp : Defines the entry point for the console application.
//
#include "Includes.h"
#include "Life.h"
#include <string.h>
#include <gl/freeglut.h>
#include "graphics/Graphics.h"
#include "simulation/Simulation.h"
#include "simulation/Preset.h"
//#include <windows.h>
#include <iostream>


Life *instance;

void loop() {
	auto t = std::chrono::high_resolution_clock::now();
	glutSetWindow(instance->gphx->windowId);
	instance->sim->update();
	instance->gphx->updatebuffer();
	instance->gphx->display();
	glutSetWindow(instance->gphx->gui.windowId);
	instance->gphx->gui.display();
	std::chrono::high_resolution_clock::time_point tprime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(tprime-t);
	double frametime = 1000.0 / target_fps;
	int waitduration = (int)(frametime - time_span.count() * 1000.0);
	if (waitduration < 0)
		waitduration = 0;
	Sleep(waitduration);
}

int Life::start(int argc, char** argv) {
	sim = new Simulation();
	gphx = new Graphics();
	input = new Input;

	glutInit(&argc, argv);

	gphx->initializeGraphics();

	glutDisplayFunc(loop);
	glutIdleFunc(loop);

	gphx->gui.gphx = gphx;
	gphx->gui.initializeGraphics();
	//sim->preset.startAirfoil();
	instance->input->paused = true;

	glutMainLoop();
	
	return 0;
}

int main(int argc, char** argv) {
#ifdef WINDOWS
	FreeConsole();
#endif
	instance = new Life;
	instance->start(argc, argv);
}