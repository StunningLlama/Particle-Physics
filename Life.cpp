// Life.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Life.h"
#include <string.h>
#include <gl/freeglut.h>
#include "Graphics.h"
#include "Simulation.h"
#include "Preset.h"
#include <Windows.h>
#include <iostream>
Life *instance;

void loop() {
	auto t = std::chrono::high_resolution_clock::now();
	instance->sim->update();
	instance->gphx->updatebuffer();
	instance->gphx->display();
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

	//sim->preset.startToilet();

	glutMainLoop();
	
	return 0;
}

int main(int argc, char** argv) {
	instance = new Life;
	instance->start(argc, argv);
}