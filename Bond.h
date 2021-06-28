#pragma once
#include "Particle.h"
class Particle;

class Bond {
public:
	Bond(Particle* A, Particle* B, int type, float len);
	Particle* particleA;
	Particle* particleB;
	int bondtype = 0;
	int bondindex = 0;
	float length = 0.0f;
	bool close = false;
	bool variablelength = false;
};