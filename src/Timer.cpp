#include "Includes.h"
#include "Timer.h"
#include "Life.h"
#include <iostream>

Timer::Timer(char *name) {
	functionname = name;
	tot = std::chrono::high_resolution_clock::duration::zero();
}

void Timer::start() {
	t = std::chrono::high_resolution_clock::now();
}

void Timer::end() {
	std::chrono::high_resolution_clock::time_point tprime = std::chrono::high_resolution_clock::now();
	tot += (tprime - t);
	marks++;
	if (marks == 50) {
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(tot / 50);
		if (DEBUG)
			std::cout << time_span.count()*1000000.0 << " microseconds for function " << functionname << std::endl;
		marks = 0;
		tot = std::chrono::high_resolution_clock::duration::zero();
	}
}