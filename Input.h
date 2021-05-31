#pragma once

#include "Simulation.h"

#define mode_brush_move 0
#define mode_brush_add 1
#define mode_brush_replace 2
#define mode_brush_delete 3

class Input {
public:
		void mouseClick(int button, int state, int x, int y);
		void mouseMotion(int x, int y);
		void mousePassiveMotion(int x, int y);
		void keyboard(unsigned char key, int x, int y);

		bool mouseDown = false;
		bool updateMouse = true;
		int mx = 0;
		int my = 0;
		float coordX = 0.0f;
		float coordY = 0.0f;

		bool paused = false;
		bool save = false;
		bool load = false;
		bool clear = false;
		bool specialfunc = false;
		int modebrush = mode_brush_move;
		int modematerial = sim_type_water;
		int brushsize = 3;
		int density = 3;

		float densitytable[5] = {2.0f, 1.5f, 1.0f, 0.75, 0.5f};

		const char *brushnames[4] = {"Brush: move", "Brush: add", "Brush: replace", "Brush: delete"};
		const char *materialnames[3] = { "Material: water", "Material: air", "Material: barrier" };
		const char *sizenames[5] = { "Size: 1", "Size: 2", "Size: 3", "Size: 4", "Size: 5" };
		const char *densitynames[5] = { "Density: 1", "Density: 2", "Density: 3", "Density: 4", "Density: 5" };
};