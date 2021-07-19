#pragma once

#include <chrono>

class Timer {
public:
	Timer(char *name);
	void start();
	void end();
	std::chrono::high_resolution_clock::time_point t;
	int marks = 0;
	std::chrono::high_resolution_clock::duration tot;
	char *functionname;
};